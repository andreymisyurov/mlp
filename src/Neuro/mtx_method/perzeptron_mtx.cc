#include "perzeptron_mtx.h"

using namespace s21;

PerzeptronMtx::PerzeptronMtx(int in_hidden_layers, int in_first_layer): m_weights(new std::vector<NeuronMatrix>()) {
  if(in_hidden_layers > 5 || in_hidden_layers < 1) throw MyException("incorrect input for hidden layer");
  int count_layers = in_hidden_layers;
  int check_layers = in_first_layer;
  while(--count_layers) check_layers /= 2;
  if(in_first_layer > 1024 || check_layers < 26) throw MyException("incorrect data for first layer");

  m_weights->emplace_back(NeuronMatrix(784, in_first_layer));
  feel_random(m_weights->back());
  --in_hidden_layers;
  while(in_hidden_layers--) {
    m_weights->emplace_back(NeuronMatrix(in_first_layer, in_first_layer / 2));
    feel_random(m_weights->back());
    in_first_layer /= 2;
  }
  m_weights->emplace_back(NeuronMatrix(in_first_layer, 26));
  feel_random(m_weights->back());
}

PerzeptronMtx::PerzeptronMtx(PerzeptronMtx &&other) noexcept {
  *this = other;
}

PerzeptronMtx::PerzeptronMtx(const PerzeptronMtx &other) {
  *this = other;
}

PerzeptronMtx &PerzeptronMtx::operator=(PerzeptronMtx &&other) noexcept {
  std::swap(m_weights, other.m_weights);
  return *this;
}

PerzeptronMtx &PerzeptronMtx::operator=(const PerzeptronMtx &other) {
  m_weights = new std::vector<NeuronMatrix>(*other.m_weights);
  return *this;
}

PerzeptronMtx::~PerzeptronMtx() {
  delete m_weights;
}

void PerzeptronMtx::exportDataBase(const std::string &in_path) {
  std::ofstream input(in_path, std::ios::binary);
  if(!input) throw MyException("can't open this file");
  input << std::to_string(m_weights->size()) << "\n";
  std::string buffer{};
  for(auto &&el: *m_weights) {
    input << std::to_string(el.getRow()) << " " << std::to_string(el.getColum()) << "\n";
    buffer = "";
    for(int i = 0; i < el.getRow(); ++i) {
      for(int j = 0; j < el.getColum(); ++j) {
        buffer += std::to_string(el(i, j)) + " ";
      }
    }
    *(--buffer.end()) = '\n';
    input << buffer;
  }
  input.close();
}

void PerzeptronMtx::importDataBase(const std::string &in_path) {
  std::fstream input(in_path);
  if(!input) throw MyException("can't open this file");
  int loops;
  input >> loops;
  auto *temp_weight = new std::vector<NeuronMatrix>;
  while(loops--) {
    int m, n;
    input >> m >> n;
    NeuronMatrix mtrx(m, n);
    double buff{};
    for(int i = 0; i < m; ++i) {
      for(int j = 0; j < n; ++j) {
        input >> buff;
        mtrx(i, j) = buff;
      }
    }
    temp_weight->emplace_back(mtrx);
  }
  std::swap(*temp_weight, *m_weights);
  temp_weight->clear();
  delete temp_weight;
  input.close();
}

double PerzeptronMtx::getMiddleErr(const std::pair<int, NeuronMatrix> &in_data) {
  double result = 0;
  for(int i = 0; i < in_data.second.getColum(); ++i)
    result += in_data.first - 1 == i ? (1 - in_data.second(0, i)) : in_data.second(0, i);
  return result / in_data.second.getColum();
}

std::pair<int, std::vector<double>> PerzeptronMtx::learn(const std::string &in_path, int in_epochs) {
  if(in_epochs <= 0 || in_epochs > 100) throw MyException("incorrect count epoch");

  auto base = Parser::parseDataBase(in_path, 1.0);
  ThreadPool pool(std::thread::hardware_concurrency());
  std::vector<std::future<std::pair<int, NeuronMatrix>>> tasks;
  std::vector<double> result;
  int counter = in_epochs;
  while(counter--) {
    for(auto &&el: base)
      tasks.push_back(pool.enqueue(prediction, el.second, m_weights, k_step, el.first));
  }
  for(auto &&el: tasks) result.push_back(getMiddleErr(el.get()));

  return std::make_pair(in_epochs, result);
}

std::pair<int, std::vector<double>> PerzeptronMtx::crossValidation(const std::string &in_path, int in_k) {
  if(in_k <= 0 || in_k > 100) throw MyException("incorrect count epoch");
  auto base = Parser::parseDataBase(in_path, 1.0);
  if(in_k > base.size()) throw MyException("too little database for cross validation numbers");

  // делим базу данных особым образом на k кусков
  std::vector<std::vector<std::pair<int, NeuronMatrix>>> parts(in_k);
  auto start = base.begin();
  for(int i = 0; i < in_k - 1; ++i) {
    parts[i] = std::vector<std::pair<int, NeuronMatrix>>(start, start + (int)base.size() / in_k);
    start = start + (int)base.size() / in_k + 1;
  }
  parts.back() = std::vector<std::pair<int, NeuronMatrix>>(start, base.end());
  ThreadPool pool(std::thread::hardware_concurrency());
  std::vector<double> result;
  for(int i = 0; i < (int)parts.size(); ++i) {
    std::vector<std::future<std::pair<int, NeuronMatrix>>> tasks;
    for(int j = 0; j < (int)parts.size(); ++j) {
      if(i == j) continue;
      for(auto &&el: base)
        tasks.push_back(pool.enqueue(prediction, el.second, m_weights, k_step, el.first));
    }
    for(auto &&el: tasks) el.get();

    std::vector<std::future<void>> test_tasks;
    int pass = (int)parts[i].size();
    std::vector<int> analytic{0, 0, 0, 0};
    std::mutex mute;
    for(auto &&el: parts[i])
      test_tasks.push_back(pool.enqueue(analyzeCase, &pass, el.first, el.second, m_weights, k_step, &analytic, &mute));
    for(auto &&el: test_tasks) el.get();
    result.push_back(
        1. - ((double)(analytic[0] + analytic[3]) / (double)(analytic[0] + analytic[3] + analytic[1] + analytic[2])));
  }
  return std::make_pair(in_k, result);
}

analytical_data PerzeptronMtx::test(const std::string &in_path, double in_data_part) {
  analytical_data result;
  Timer timer(true);
  ThreadPool pool(std::thread::hardware_concurrency());
  std::vector<std::future<void>> tasks;
  auto base = Parser::parseDataBase(in_path, in_data_part);
  int pass = (int)base.size();
  std::vector<int> analytic{0, 0, 0, 0};
  std::mutex mute;
  for(auto &&el: base)
    tasks.push_back(pool.enqueue(analyzeCase, &pass, el.first, el.second, m_weights, k_step, &analytic, &mute));
  for(auto &&el: tasks) el.get();
  timer.stop();
  result.precision = double(analytic[0]) / (analytic[0] + analytic[2]);
  result.recall = double(analytic[0]) / (analytic[0] + analytic[1]);
  result.f_measure = (1 + 1 * 1) * result.precision * result.recall / (1 * 1 * result.precision + result.recall);
  result.average_accuracy =
      (double)(analytic[0] + analytic[3]) / (analytic[0] + analytic[3] + analytic[1] + analytic[2]);
  result.time = timer.getTime();
  return result;
}

int PerzeptronMtx::predict(const NeuronMatrix &in_layer) {
  return prediction(in_layer, m_weights, k_step).first;
}

std::pair<int, NeuronMatrix> PerzeptronMtx::prediction(const NeuronMatrix &in_layer,
                            std::vector<NeuronMatrix> *inout_weights, double in_step, std::optional<int> in_correct) {
  std::vector<NeuronMatrix> layers{in_layer};
  for(auto &el: *inout_weights)
    layers.emplace_back(std::move(layers.back().mulMatrix(el, true, false)));
  double maximum = layers.back()(0, 0);
  int num = 0;
  for(int i = 0; i < layers.back().getColum(); ++i)
    if(layers.back()(0, i) > maximum) {
      maximum = layers.back()(0, i);
      num = i;
    }
  if(in_correct) {
    std::vector<NeuronMatrix> sum_err_weight_prev(*inout_weights);
    std::vector<NeuronMatrix> errors;
    std::vector<NeuronMatrix> d_weight = *inout_weights;
    for(auto &&el: layers)
      errors.emplace_back(std::move(*(el.transport())));
    for(int i = 0; i < 26; ++i) {
      double o_i = layers.back()(0, i);
      double err_i = -o_i * (1 - o_i) * ((i + 1 == in_correct.value() ? 1. : 0.) - o_i);
      errors.back()(i, 0) = err_i;
    }
    sum_err_weight_prev.back() = inout_weights->back() * errors.back();
    for(size_t i = errors.size() - 2; i > 0; --i) {
      for(int j = 0; j < errors[i].getRow(); ++j) {
        double o_j = errors[i](j, 0);
        errors[i](j, 0) = o_j * (1 - o_j) * sum_err_weight_prev[i](j, 0);
      }
      sum_err_weight_prev[i - 1] = (*inout_weights)[i - 1] * errors[i];
    }
    int l = 0;
    for(auto &&el: d_weight) {
      for(int i = 0; i < el.getRow(); ++i)
        for(int j = 0; j < el.getColum(); ++j)
          el(i, j) = layers[l](0, i) * errors[l + 1](j, 0) * in_step;
      ++l;
    }
    auto minus = d_weight.begin();
    for(auto &&el: *inout_weights)
      el -= *(minus++);
  }
  return std::make_pair(num + 1, layers.back());
}

void PerzeptronMtx::analyzeCase(int *inout_count, int in_correct,
          const NeuronMatrix &in_mtrx, const std::vector<NeuronMatrix> *in_weights, double in_step,
                                                              std::vector<int> *out_analytic, std::mutex *in_mutex) {
  auto control = prediction(in_mtrx, const_cast<std::vector<NeuronMatrix> *>(in_weights), in_step);
  std::lock_guard<std::mutex> guard(*in_mutex);
  for(int i = 0; i < control.second.getRow(); ++i) {
    if(control.second(0, i) >= 0.85) {
      control.first == in_correct ? ++(*out_analytic)[0] : ++(*out_analytic)[1];
    } else {
      control.first != in_correct ? ++(*out_analytic)[2] : ++(*out_analytic)[3];
    }
  }
  if(in_correct != control.first) {
    *inout_count -= 1;
  }
}

void PerzeptronMtx::feel_random(NeuronMatrix &out_mtrx) {
  srand(time(nullptr));
  for(int i = 0; i < out_mtrx.getRow(); ++i)
    for(int j = 0; j < out_mtrx.getColum(); ++j)
      out_mtrx(i, j) = (rand() % 256) / 255. - 0.5;
}
