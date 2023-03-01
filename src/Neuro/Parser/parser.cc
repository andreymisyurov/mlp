#include "parser.h"

Parser::~Parser() = default;
std::vector<std::pair<int, NeuronMatrix>> Parser::parseDataBase(const std::string &in_path, double in_data_part) {
  std::vector<std::string> base;
  std::ifstream input(in_path, std::ios::binary);
  if(!input) throw MyException("can't open this file");
  std::string line{};
  while(getline(input, line)) base.push_back(line);

  int count_case = static_cast<int>((double)base.size() * in_data_part);

  std::vector<std::pair<int, NeuronMatrix>> result;
  ThreadPool pool(std::thread::hardware_concurrency());
  std::vector<std::future<void>> tasks;

  std::mutex mtx;
  for(auto &&i: base) {
    if(!count_case--) break;
    tasks.push_back(pool.enqueue(parseOneLine, i, &result, &mtx));
  }

  for(auto &&el: tasks) el.get();
  return result;
}

NeuronMatrix Parser::parse_one_mtrx(const std::string &in_path) {
  // временная функция чисто для тестов
  std::ifstream input(in_path);
  if(!input) throw MyException("can't open this file");

  std::string line{};
  getline(input, line);

  size_t pos = line.find(',');
  int value = std::stoi(line.substr(0, pos));
  line.erase(0, pos + 1);

  NeuronMatrix mtrx(1, 784);
  for(int j = 0; j < mtrx.getColum() - 1; ++j) {
    size_t pos = line.find(',');
    mtrx(0, j) = std::stoi(line.substr(0, pos)) / kNORMA;
    line.erase(0, pos + 1);
  }
  mtrx(0, 783) = std::stoi(line);
  return mtrx;
}

//void Parser::export_data(const std::string& in_path, const std::vector<NeuronMatrix> *in_weight) {
//    std::ofstream input(in_path, std::ios::binary);
//    if(!input) throw MyException("can't open this file");
//    input << std::to_string(in_weight->size()) << "\n";
//    std::string buffer{};
//    for(auto &&el: *in_weight) {
//      input << std::to_string(el.getRow()) << " " << std::to_string(el.getColum()) << "\n";
//      buffer = "";
//      for(int i = 0; i < el.getRow(); ++i) {
//        for(int j = 0; j < el.getColum(); ++j) {
//          buffer += std::to_string(el(i, j)) + " ";
//        }
//      }
//      *(--buffer.end()) = '\n';
//      input << buffer;
//    }
//    input.close();
//  }
//
//void Parser::import_data(const std::string& in_path, std::vector<NeuronMatrix> *inout_weight) {
//  // проверить на утечки
//  std::fstream input(in_path);
//  if(!input) throw MyException("can't open this file");
//  int loops;
//  input >> loops;
//  auto *temp_weight = new std::vector<NeuronMatrix>;
//  while(loops--) {
//    int m, n;
//    input >> m >> n;
//    NeuronMatrix mtrx(m, n);
//    double buff{};
//    for(int i = 0; i < m; ++i) {
//      for(int j = 0; j < n; ++j) {
//        input >> buff;
//        mtrx(i, j) = buff;
//      }
//    }
//    temp_weight->emplace_back(mtrx);
//  }
//
//  std::swap(*temp_weight, *inout_weight);
//  temp_weight->clear();
//  delete temp_weight;
//  input.close();
//}

void Parser::parseOneLine(std::string &in_line, std::vector<std::pair<int, NeuronMatrix>> *inout_result, std::mutex *in_mtx) {
  size_t pos = in_line.find(',');
  int value = std::stoi(in_line.substr(0, pos));
  in_line.erase(0, pos + 1);
  auto mtr = NeuronMatrix(1, 784);
  for(int j = 0; j < mtr.getColum() - 1; ++j) {
    pos = in_line.find(',');
    mtr(0, j) = std::stoi(in_line.substr(0, pos)) / kNORMA;
    in_line.erase(0, pos + 1);
  }
  mtr(0, 783) = std::stoi(in_line);
  {
    std::lock_guard<std::mutex> guard(*in_mtx);
    inout_result->push_back(std::make_pair(value, mtr));
  }
};
