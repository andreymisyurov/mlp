#include "perzeptron_mtx.h"

PerzeptronMtx::PerzeptronMtx(int hidden_layers, int first_layer): weights(new std::vector<NeuronMatrix>()) {
  if(hidden_layers > 5 || hidden_layers < 1) throw MyException("incorrect input for hidden layers");
  int count_layers = hidden_layers;
  int check_layers = first_layer;
  while(--count_layers) check_layers /= 2;
  if(first_layer > 1024 || check_layers <= 26) throw MyException("incorrect data for first layer");

  weights->emplace_back(std::move(NeuronMatrix(784, first_layer)));
  feel_random(weights->back());
  --hidden_layers;
  while(hidden_layers--) {
    weights->emplace_back(std::move(NeuronMatrix(first_layer, first_layer / 2)));
    feel_random(weights->back());
    first_layer /= 2;
  }
  weights->emplace_back(std::move(NeuronMatrix(first_layer, 26)));
  feel_random(weights->back());
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

double min_square(const std::pair<int, NeuronMatrix>& in_data) {
//  double result = 0;
//  for(int i = 0; i < in_data.second.getColum(); ++i) {
//    result += in_data.first - 1 == i ? pow(1 - in_data.second(0, i), 2) : pow(0 - in_data.second(0, i), 2);
//  }
//  return result / in_data.second.getColum();
  double result = 0;
  for(int i = 0; i < in_data.second.getColum(); ++i) {
    result += in_data.first - 1 == i ? (1 - in_data.second(0, i)) : in_data.second(0, i);
  }
  return result / in_data.second.getColum();
}

std::pair<int, std::vector<double>> PerzeptronMtx::learn(const std::string &path, int epochs) {
  if(epochs <= 0 || epochs > 100) throw MyException("incorrect count epoch");
  auto base = parser->getMatrix(path);
  ThreadPool pool(std::thread::hardware_concurrency());
  std::vector<std::future<std::pair<int, NeuronMatrix>>> tasks;
  std::vector<double> result;
  int counter = epochs;
  while(counter--) {

    for(auto &&el: base)
      tasks.push_back(pool.enqueue(prediction, el.second, weights, m_step, el.first));

  }
    for(auto &&el: tasks) result.push_back(min_square(el.get()));
//    result.push_back(temp);

  return std::make_pair(epochs, result);
}

analytical_data PerzeptronMtx::test(const std::string &path, double part_of_tests) {
  analytical_data result;
  Timer timer(true);
  ThreadPool pool(std::thread::hardware_concurrency());
  std::vector<std::future<void>> tasks;
  auto base = parser->getMatrix(path, part_of_tests);
  int pass = (int)base.size();
  std::vector<int> analytic{0, 0, 0, 0};
  std::mutex mute;
  for(auto &&el: base)
    tasks.push_back(pool.enqueue(check_case, &pass, el.first, el.second, weights, m_step, &analytic, &mute));
  for(auto &&el: tasks) el.get();
  timer.Stop();
  std::cout << analytic[0] << " " << analytic[1] << " " << analytic[2] << " " << analytic[3] << std::endl;
  // убедиться что формулы верны, а они гавеные
  result.precision = double(analytic[0]) / (analytic[0] + analytic[2]);
  result.recall = double(analytic[0]) / (analytic[0] + analytic[1]);
  result.f_measure = (1 + 1*1) * result.precision * result.recall / (1*1*result.precision + result.recall);
//  result.average_accuracy = static_cast<double>(pass) / base.size();
  result.average_accuracy = (double)(analytic[0] + analytic[3]) / (analytic[0] + analytic[3] + analytic[1] + analytic[2]);
//  std::cout << result.precision << " " << result.recall << " " << result.f_measure << " " << result.average_accuracy << std::endl;
  result.time = timer.getTime();
  return result;
}

int PerzeptronMtx::predict(const NeuronMatrix &layer_main) {
  return prediction(layer_main, weights, m_step).first;
}

std::pair<int, NeuronMatrix> PerzeptronMtx::prediction(const NeuronMatrix &layer_main, std::vector<NeuronMatrix> *weights, double in_step, std::optional<int> correct) {
  // техдолг - убрать подсчет ошибков на нулевом слое
  // убрать std::move и протестировать
  std::vector<NeuronMatrix> layers{layer_main};
  for(auto &el: *weights) {
    layers.emplace_back(std::move(layers.back().mulMatrix(el, true, false)));
  }

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
      errors.emplace_back(std::move(*(el.transport())));

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
  return std::make_pair(num + 1, layers.back());
}

void PerzeptronMtx::check_case(int *pass, int correct, NeuronMatrix mtrx, std::vector<NeuronMatrix> *weights, double in_step, std::vector<int> *analytic, std::mutex *mute) {
  // [0] - True Positive когда заряд больше 0,85 у правильного нейрона
  // [1] - False Negative когда заряд больше 0,85 у НЕ правильного нейрона
  // [2] - False Positive когда заряд меньше 0,85 у НЕ правильного нейрона
  // [3] - True Negative когда заряд меньше 0,85 у правильного нейрона
  auto control = prediction(mtrx, weights, in_step);
  std::lock_guard<std::mutex> guard(*mute);
  for(int i = 0; i < control.second.getRow(); ++i) {
    // оптимизировать это говно
    if(control.second(0, i) > 0.85) {
      if(control.first == correct) {
        ++(*analytic)[0];
      } else {
        ++(*analytic)[1];
      }
    } else {
      if(control.second(0, i) < 0.85) {
        if(control.first != correct) {
          ++(*analytic)[2];
        } else {
          ++(*analytic)[3];
        }
      }
    }
  }
  if(correct != control.first) {
    *pass -= 1;
  }
}

void PerzeptronMtx::feel_random(NeuronMatrix &mtrx) {
  srand(time(0));
  for(int i = 0; i < mtrx.getRow(); ++i)
    for(int j = 0; j < mtrx.getColum(); ++j)
      mtrx(i, j) = (rand() % 256) / 255. - 0.5;
}
