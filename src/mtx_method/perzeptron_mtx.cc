#include "perzeptron_mtx.h"

PerzeptronMtx::PerzeptronMtx(int hidden_layers): weights(new std::vector<NeuronMatrix>()) {
  int start_rows = 512;
  if(hidden_layers > 5 || hidden_layers < 1) throw MyException("incorrect input for hidden layers");
  if(start_rows > 1024 || (start_rows / hidden_layers) <= 26) throw MyException("incorrect data for first layer");
  weights->emplace_back(std::move(NeuronMatrix(784, start_rows)));
  feel_random(weights->back());
  --hidden_layers;
  while(hidden_layers--) {
    weights->emplace_back(std::move(NeuronMatrix(start_rows, start_rows / 2)));
    feel_random(weights->back());
    start_rows /= 2;
  }
  weights->emplace_back(std::move(NeuronMatrix(start_rows, 26)));
  feel_random(weights->back());
  for(auto el: *weights) {
//    std:: cout << el.getRow() << " " << el.getColum() << std::endl;
  }
}

PerzeptronMtx::~PerzeptronMtx() {
  delete weights;
}

void PerzeptronMtx::export_data(const std::string &path) {
  parser->export_data(weights, path);
}

void PerzeptronMtx::import_data(const std::string &path) {
  parser->import_data(weights, path);
}

void PerzeptronMtx::learn(const std::string &path, int8_t epochs) {
  auto base = parser->getMatrix(path);
  ThreadPool pool(std::thread::hardware_concurrency());
  std::vector<std::future<int>> tasks;
  while(epochs--)
    for(auto &&el: base)
      tasks.push_back(pool.enqueue(prediction, el.second, weights, m_step, el.first));
  for(auto &&el: tasks) el.get();
}

analytical_data PerzeptronMtx::test(const std::string &path, double part_of_tests) {
  analytical_data result;
  Timer timer(true);
  ThreadPool pool(std::thread::hardware_concurrency());
  std::vector<std::future<void>> tasks;
  auto base = parser->getMatrix(path, part_of_tests);
  int pass = base.size();
  for(auto &&el: base) tasks.push_back(pool.enqueue(check_case, &pass, el.first, el.second, weights, m_step));
  for(auto &&el: tasks) el.get();
  timer.Stop();
  result.time = timer.getTime();
  result.average_accuracy = static_cast<double>(pass) / base.size();
  return result;
}

int PerzeptronMtx::predict(const NeuronMatrix &layer_main) {
  return prediction(layer_main, weights, m_step);
}

int PerzeptronMtx::prediction(const NeuronMatrix &layer_main, std::vector<NeuronMatrix> *weights, double in_step, std::optional<int> correct) {
  // техдолг - убрать подсчет ошибков на нулевом слое
  // убрать std::move и протестировать
  std::vector<NeuronMatrix> layers{layer_main};
  for(auto &el: *weights) {
    layers.emplace_back(std::move(layers.back().mulMatrix(el, true, false)));
  }

//  for(auto el: *weights) {
//    std:: cout << el.getRow() << " " << el.getColum() << std::endl;
//  }
//
//  for(auto el: layers) {
//    std:: cout << el.getRow() << " " << el.getColum() << std::endl;
//  }

  double maximum = layers.back()(0, 0);
  int num = 0;
  for(int i = 0; i < layers.back().getColum(); ++i) {
    if(layers.back()(0, i) > maximum) {
      maximum = layers.back()(0, i);
      num = i;
    }
  }
  if(correct) {
    std::vector<NeuronMatrix> sum_err_weight_prev(*weights);
    std::vector<NeuronMatrix> errors;
    std::vector<NeuronMatrix> d_weight = *weights;
    for(auto &&el: layers)
      errors.emplace_back(std::move(el.transpose()));

    for(int i = 0; i < 26; ++i) {
      double o_i = layers.back()(0, i);
      double err_i = -o_i * (1 - o_i) * ((i + 1 == correct.value() ? 1. : 0.) - o_i);
      errors.back()(i, 0) = err_i;
    }

    sum_err_weight_prev.back() = weights->back() * errors.back();

    for(int i = errors.size() - 2; i > 0; --i) {
      for(int j = 0; j < errors[i].getRow(); ++j) {
        double o_j = errors[i](j, 0);
        errors[i](j, 0) = o_j * (1 - o_j) * sum_err_weight_prev[i](j, 0);
      }
      sum_err_weight_prev[i - 1] = (*weights)[i - 1] * errors[i];
    }


    int l = 0;
    for(auto &el: d_weight) {
      for(int i = 0; i < el.getRow(); ++i) {
        for(int j = 0; j < el.getColum(); ++j) {
          el(i, j) = layers[l](0, i) * errors[l + 1](j, 0) * in_step;
        }
      }
      ++l;
    }
    auto minus = d_weight.begin();
    for(auto &&el: *weights) {
      el -= *minus;
      ++minus;
    }
  }
  return num + 1;
}

void PerzeptronMtx::check_case(int *pass, int correct, NeuronMatrix mtrx, std::vector<NeuronMatrix> *weights, double in_step) {
  auto control = prediction(mtrx, weights, in_step);
  if(correct != control) {
    *pass -= 1;
  }
}

void PerzeptronMtx::feel_random(NeuronMatrix &mtrx) {
  srand(time(0));
  for(int i = 0; i < mtrx.getRow(); ++i)
    for(int j = 0; j < mtrx.getColum(); ++j)
      mtrx(i, j) = (rand() % 256) / 255. - 0.5;
}
