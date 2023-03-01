#include "perzeptron_grph.h"

using namespace s21;

PerzeptronGrph::PerzeptronGrph(int in_hidden_layers, int in_first_layer) {
  int count_layers = in_hidden_layers;
  int check_layers = in_first_layer;
  while (--count_layers) check_layers /= 2;
  if (check_layers < 26)
    throw MyException("incorrect data for first layer");

  srand(time(nullptr));

  m_layers = std::make_shared<std::vector<std::vector<GraphData>>>(
      in_hidden_layers + 2);
  (*m_layers)[0] = std::vector<GraphData>(784);
  for (auto &&el : (*m_layers)[0]) {
    el.m_connect = std::make_shared<std::vector<double>>(in_first_layer);
    for (auto &&weight : *el.m_connect) weight = (rand() % 256) / 255. - 0.5;
  }

  for (int i = 1; i <= in_hidden_layers; ++i) {
    (*m_layers)[i] = std::vector<GraphData>(in_first_layer);
    in_first_layer /= 2;
    int gabella = (i == in_hidden_layers) ? 26 : in_first_layer;
    for (auto &&el : (*m_layers)[i]) {
      el.m_connect = std::make_shared<std::vector<double>>(gabella);
      for (auto &&weight : *el.m_connect) weight = (rand() % 256) / 255. - 0.5;
    }
  }

  (*m_layers)[in_hidden_layers + 1] = std::vector<GraphData>(26);
  for (auto &&el : (*m_layers)[in_hidden_layers + 1]) {
    el.m_charge = 0.0;
    el.m_connect = nullptr;
  }
}

double PerzeptronGrph::getMiddleErr(const std::pair<int, std::vector<double>> &in_data) {
  double result = 0;
  for (int i = 0; i < in_data.second.size(); ++i)
    result +=
        (in_data.first - 1) == i ? (1 - in_data.second[i]) : in_data.second[i];
  return result / (double)in_data.second.size();
}

std::pair<int, std::vector<double>> PerzeptronGrph::learn(const std::string &in_path, int in_epochs) {
  if (in_epochs <= 0)
    throw MyException("incorrect count epoch");
  auto base = Parser::parseDataBase(in_path, 1.0);
  std::vector<double> result;
  int counter = in_epochs;
  while (counter--) {
    for (auto &&el : base) {
      result.push_back(getMiddleErr(prediction(el.second, el.first)));
    }
  }
  return std::make_pair(in_epochs, result);
}

std::pair<int, std::vector<double>> PerzeptronGrph::crossValidation(const std::string &in_path, int in_k) {
  if (in_k <= 0 || in_k > 100) throw MyException("incorrect count epoch");
  auto base = Parser::parseDataBase(in_path, 1.0);
  if (in_k > base.size())
    throw MyException("too little database for cross validation numbers");

  // делим базу данных особым образом на k кусков
  std::vector<std::vector<std::pair<int, NeuronMatrix>>> parts(in_k);
  auto start = base.begin();
  for (int i = 0; i < in_k - 1; ++i) {
    parts[i] = std::vector<std::pair<int, NeuronMatrix>>(
        start, start + (int)base.size() / in_k);
    start = start + (int)base.size() / in_k + 1;
  }
  parts.back() = std::vector<std::pair<int, NeuronMatrix>>(start, base.end());
  // учим сеть k эпох на k-1 кусках, и на одном куске сразу тестируем,
  // записываем ошибку в вектор result
  std::vector<double> result;
  for (int i = 0; i < (int)parts.size(); ++i) {
    for (int j = 0; j < (int)parts.size(); ++j) {
      if (i == j) continue;
      for (auto &&el : parts[i]) prediction(el.second, el.first);
    }

    // собираем аналитику на одном куске
    int pass = (int)parts[i].size();
    std::vector<int> analytic{0, 0, 0, 0};
    for (auto &&el : parts[i])
      analyzeCase(&pass, el.first, el.second, &analytic);
    result.push_back(
        1. - ((double)(analytic[0] + analytic[3]) /
              (double)(analytic[0] + analytic[3] + analytic[1] + analytic[2])));
  }
  return std::make_pair(in_k, result);
}

analytical_data PerzeptronGrph::test(const std::string &in_path, double in_data_part) {
  analytical_data result{};
  Timer timer(true);
  auto base = Parser::parseDataBase(in_path, in_data_part);
  int pass = (int)base.size();
  std::vector<int> analytic{0, 0, 0, 0};

  for (auto &&el : base) {
    analyzeCase(&pass, el.first, el.second, &analytic);
  }

  timer.stop();
  result.precision = double(analytic[0]) / (analytic[0] + analytic[2]);
  result.recall = double(analytic[0]) / (analytic[0] + analytic[1]);
  result.f_measure = (1 + 1 * 1) * result.precision * result.recall /
                     (1 * 1 * result.precision + result.recall);
  result.average_accuracy =
      (double)(analytic[0] + analytic[3]) /
      (analytic[0] + analytic[3] + analytic[1] + analytic[2]);
  result.time = timer.getTime();
  return result;
}

void PerzeptronGrph::analyzeCase(int *out_count, int in_correct,
                                       const NeuronMatrix &in_mtrx, std::vector<int> *out_analytic) {
  auto control = prediction(in_mtrx, std::nullopt);
  for (int i = 0; i < control.second.size(); ++i) {
    if (control.second[i] >= 0.85) {
      control.first == in_correct ? ++(*out_analytic)[0] : ++(*out_analytic)[1];
    } else {
      control.first != in_correct ? ++(*out_analytic)[2] : ++(*out_analytic)[3];
    }
  }
  if (in_correct != control.first) {
    *out_count -= 1;
  }
}

int PerzeptronGrph::predict(const NeuronMatrix &in_layer) {
  return prediction(in_layer, 1).first;
}

void PerzeptronGrph::exportDataBase(const std::string &in_path) {
  std::ofstream input(in_path, std::ios::binary);
  if (!input) throw MyException("can't open this file");
  int l = (int)m_layers->size() - 1;
  input << std::to_string(l) << "\n";
  std::string buffer{};

  for (int i = 0; i < l; ++i) {
    int m = (int)(*m_layers)[i].size();
    int n = (int)(*m_layers)[i + 1].size();
    input << m << " " << n << "\n";
    buffer = "";
    for (int j = 0; j < m; ++j) {
      for (int k = 0; k < n; ++k) {
        buffer += std::to_string((*(*m_layers)[i][j].m_connect)[k]) + " ";
      }
    }
    *(--buffer.end()) = '\n';
    input << buffer;
  }
  input.close();
}

void PerzeptronGrph::importDataBase(const std::string &in_path) {
  std::fstream input(in_path);
  if (!input) throw MyException("can't open this file");
  int loops;
  input >> loops;
  int m, n;
  input >> m >> n;
  bool check = false;
  PerzeptronGrph temp(loops - 1, n);
  for (int l = 0; l < loops; ++l) {
    if (check) input >> m >> n;
    double buff{};
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        input >> buff;
        (*(*temp.m_layers)[l][i].m_connect)[j] = buff;
      }
    }
    check = true;
  }
  std::swap(m_layers, temp.m_layers);
  input.close();
}

double PerzeptronGrph::getErrorRow( const std::shared_ptr<std::vector<double>> &in_weights,
                                                                        const std::vector<double> &in_err) {
  double result = 0.0;
  for (int l = 0; l < in_err.size(); ++l) {
    result += (*in_weights)[l] * in_err[l];
  }
  return result;
}

void PerzeptronGrph::backPropagation(int in_correct) {
  // create and fill container for keeping predict error
  std::vector<std::vector<double>> errors(m_layers->size());
  auto iter = m_layers->begin();
  for (auto &er : errors) {
    er = std::vector<double>(iter->size());
    ++iter;
  }

  // figure out errors last neuron layer
  for (int i = 0; i < (*m_layers).back().size(); ++i) {
    double o_i = (*m_layers).back()[i].m_charge;
    errors.back()[i] =
        -o_i * (1 - o_i) * ((in_correct == i + 1 ? 1. : 0.) - o_i);
  }

  // вычисляем ошибки остальных слоев, кроме последнего
  for (int i = (int)errors.size() - 2; i > 0; --i) {
    for (int j = 0; j < errors[i].size(); ++j) {
      errors[i][j] = (*m_layers)[i][j].m_charge *
                     (1 - (*m_layers)[i][j].m_charge) *
                     getErrorRow((*m_layers)[i][j].m_connect, errors[i + 1]);
    }
  }
  // идем как обычно с переда до предпоследнего слоя, берем ошибки следующего
  // слоя (l+1) корректируем веса вычитая заряд нейрона помноженный на шаг и на
  // ошибку следующего слое
  int l = 0;
  for (auto &&layer : *m_layers) {
    if (l == m_layers->size() - 1) break;
    for (auto &&data : layer) {
      for (int j = 0; j < data.m_connect->size(); ++j) {
        (*data.m_connect)[j] -=
            (double)data.m_charge * errors[l + 1][j] * k_step;
      }
    }
    ++l;
  }
}

std::pair<int, std::vector<double>> PerzeptronGrph::prediction(const NeuronMatrix &in_layer,
                                                                          std::optional<int> in_correct) {
  // convert NeuronMatrix to std::vector
  for (int i = 0; i < 784; ++i) {
    (*m_layers)[0][i].m_charge = in_layer(0, i);
  }

  // count output layer
  for (int i = 0; i < m_layers->size() - 1; ++i) {
    for (int j = 0; j < (*m_layers)[i + 1].size(); ++j) {
      (*m_layers)[i + 1][j].m_charge = figureOutCharge((*m_layers)[i], j);
    }
  }

  // find maximum charge
  int num = 0;
  int count = 0;
  double max = (*m_layers).back()[0].m_charge;
  for (auto &&el : (*m_layers).back()) {
    if (el.m_charge > max) {
      max = el.m_charge;
      num = count;
    }
    ++count;
  }

  if (in_correct) {
    backPropagation(in_correct.value());
  }

  std::vector<double> result;
  for (auto &&el : m_layers->back()) {
    result.push_back(el.m_charge);
  }
  return std::make_pair(num + 1, result);
}

double PerzeptronGrph::figureOutCharge(const std::vector<GraphData> &in_layer, int in_num) {
  double result = 0.0;
  for (auto &&el : in_layer) {
    result += el.m_charge * (*el.m_connect)[in_num];
  }
  result = 1. / (1 + exp(-result));
  return result;
}
