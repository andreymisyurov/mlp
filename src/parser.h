#ifndef PARSER_PERC_
#define PARSER_PERC_

#include <fstream>
#include "./Matrix/neuron_matrix.h"
#include <vector>
#include <iostream>
#include "thread_pool.h"
#include <mutex>

using namespace victoriv;
inline constexpr double kNORMA = 255.;

class Parser {
 public:
  std::vector<std::pair<int, NeuronMatrix>> getMatrix(const std::string &path) {
    std::vector<std::string> base;
    std::ifstream input(path);
    if(!input) throw std::exception();
    std::string line = "";
    while(getline(input, line)) base.push_back(line);

    std::vector<std::pair<int, NeuronMatrix>> result;
    ThreadPool pool(std::thread::hardware_concurrency());
    std::vector<std::future<void>> tasks;

    std::mutex mtx;
    for(auto &&i: base) tasks.push_back(pool.enqueue(one_th, i, &result, &mtx));

    for(auto &&el: tasks) el.get();
    return result;
  }

  NeuronMatrix parse_one_mtrx(std::string in_path) {
    std::vector<std::string> base;
    std::ifstream input(in_path);
    if(!input) throw std::exception();
    std::string line = "";
    getline(input, line);
    NeuronMatrix mtrx(1, 768);
    for(int j = 0; j < 768; ++j) {
      size_t pos = line.find(",");
      mtrx(0, j) = std::stoi(line.substr(0, pos)) / kNORMA;
      line.erase(0, pos + 1);
    }
    return mtrx;
  }

  static void export_data(std::vector<NeuronMatrix> *weight, std::string in_path) {
    std::fstream input(in_path, std::ios::out);
    input << std::to_string(weight->size()) << "\n";
    std::string buffer;
    for(auto &&el: *weight) {
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

  static void import_data(std::vector<NeuronMatrix> *weight, std::string in_path) {
    // проверить на утечки
    std::fstream input(in_path);
    int loops;
    input >> loops;
    std::vector<NeuronMatrix> *out_weight = new std::vector<NeuronMatrix>;
    while(loops--) {
      int m, n;
      input >> m >> n;
      NeuronMatrix mtrx(m, n);
      double buff;
      for(int i = 0; i < m; ++i) {
        for(int j = 0; j < n; ++j) {
          input >> buff;
          mtrx(i, j) = buff;
        }
      }
      out_weight->emplace_back(mtrx);
    }
    std::swap(*out_weight, *weight);
    out_weight->clear();
    delete out_weight;
    input.close();
  }

 private:
  static void one_th(std::string &line, std::vector<std::pair<int, NeuronMatrix>> *result, std::mutex *mtx) {
    size_t pos = line.find(",");
    int value = std::stoi(line.substr(0, pos));
    line.erase(0, pos + 1);
    auto mtr = NeuronMatrix(1, 768);
    for(int j = 0; j < 768; ++j) {
      pos = line.find(",");
      mtr(0, j) = std::stoi(line.substr(0, pos)) / kNORMA;
      line.erase(0, pos + 1);
    }
    {
      std::lock_guard<std::mutex> guard(*mtx);
      result->push_back(std::make_pair(value, mtr));
    }
  };
};

//class Parser {
// public:
//  std::vector<std::pair<int, Matrix<double>>> getMatrix(const std::string &path) {
//    std::vector<std::pair<int, Matrix<double>>> result;
//    std::ifstream input(path);
//    if(!input) throw std::exception();
//    std::string line;
//    while(getline(input, line)) {
//      size_t pos = line.find(",");
//      int value = std::stoi(line.substr(0, pos));
//      line.erase(0, pos + 1);
//      auto mtr = Matrix<double>(768, 1);
//      for(int i = 0; i < 768; ++i) {
//        pos = line.find(",");
//        mtr(i, 0) = std::stoi(line.substr(0, pos))/NORMA;
//        line.erase(0, pos + 1);
//      }
//      result.push_back(std::make_pair(value, mtr));
//    }
//    return result;
//  }
//};

#endif // PARSER_PERC_