#include "parser.h"

using namespace s21;

Parser::~Parser() = default;
std::vector<std::pair<int, NeuronMatrix>> Parser::parseDataBase(
    const std::string &in_path, double in_data_part) {
  std::vector<std::string> base;
  std::ifstream input(in_path, std::ios::binary);
  if (!input) throw MyException("can't open this file");
  std::string line{};
  while (getline(input, line)) base.push_back(line);

  int count_case = static_cast<int>((double)base.size() * in_data_part);

  std::vector<std::pair<int, NeuronMatrix>> result;
  ThreadPool pool(std::thread::hardware_concurrency());
  std::vector<std::future<void>> tasks;

  std::mutex mtx;
  for (auto &&i : base) {
    if (!count_case--) break;
    tasks.push_back(pool.enqueue(parseOneLine, i, &result, &mtx));
  }

  for (auto &&el : tasks) el.get();
  return result;
}

void Parser::parseOneLine(
    std::string &in_line,
    std::vector<std::pair<int, NeuronMatrix>> *inout_result,
    std::mutex *in_mtx) {
  size_t pos = in_line.find(',');
  int value = std::stoi(in_line.substr(0, pos));
  in_line.erase(0, pos + 1);
  auto mtr = NeuronMatrix(1, 784);
  for (int j = 0; j < mtr.getColum() - 1; ++j) {
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
