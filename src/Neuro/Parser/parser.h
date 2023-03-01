#pragma once
// убрать прагму
// вынести реализацию в source файл
#include <fstream>
#include <vector>
#include <iostream>
#include <mutex>

#include "neuron_matrix.h"
#include "thread_pool.h"

//using namespace victoriv;
inline constexpr double kNORMA = 255.;

class Parser {
  // приватный деструктор чтобы нельзя было создать объект в классе только со стат функциями
  ~Parser() = default;
 public:
  static std::vector<std::pair<int, NeuronMatrix>> getMatrix(const std::string &path, double in_part = 1.0) {
    // переименовать
    // бросить крафтовое исключение
    std::vector<std::string> base;
    std::ifstream input(path, std::ios::binary);
    if(!input) throw MyException("can't open this file");
    std::string line{};
    while(getline(input, line)) base.push_back(line);

    int count_case = static_cast<int>((double)base.size() * in_part);

    std::vector<std::pair<int, NeuronMatrix>> result;
    ThreadPool pool(std::thread::hardware_concurrency());
    std::vector<std::future<void>> tasks;

    std::mutex mtx;
    for(auto &&i: base) {
      if(!count_case--) break;
      tasks.push_back(pool.enqueue(one_th, i, &result, &mtx));
    }

    for(auto &&el: tasks) el.get();
    return result;
  }

  static NeuronMatrix parse_one_mtrx(const std::string& in_path) {
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

  static void export_data(std::vector<NeuronMatrix> *weight, const std::string& in_path) {
    std::ofstream input(in_path, std::ios::binary);
    if(!input) throw MyException("can't open this file");
    input << std::to_string(weight->size()) << "\n";
    std::string buffer{};
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
//    std::fstream input(in_path, std::ios::out);
//    input << std::to_string(weight->size()) << "\n";
//    std::string buffer{};
//    for(auto &&el: *weight) {
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
  }

  static void import_data(std::vector<NeuronMatrix> *weight, const std::string& in_path) {
    // проверить на утечки
    std::fstream input(in_path);
    if(!input) throw MyException("can't open this file");
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
    size_t pos = line.find(',');
    int value = std::stoi(line.substr(0, pos));
    line.erase(0, pos + 1);
    auto mtr = NeuronMatrix(1, 784);
    for(int j = 0; j < mtr.getColum() - 1; ++j) {
      pos = line.find(',');
      mtr(0, j) = std::stoi(line.substr(0, pos)) / kNORMA;
      line.erase(0, pos + 1);
    }
    mtr(0, 783) = std::stoi(line);
    {
      std::lock_guard<std::mutex> guard(*mtx);
      result->push_back(std::make_pair(value, mtr));
    }
  };
};

