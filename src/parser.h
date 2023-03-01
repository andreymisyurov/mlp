#include <fstream>
#include "my_matrix.h"
#include <vector>
#include <iostream>
#include "thread_pool.h"
#include <mutex>

using namespace victoriv;
inline constexpr double NORMA = 255.;

class Parser {
 public:
  std::vector<std::pair<int, Matrix<double>>> getMatrix(const std::string &path) {
    std::vector<std::string> base;
    std::ifstream input(path);
    if(!input) throw std::exception();
    std::string line = "";
    while(getline(input, line)) base.push_back(line);

    std::vector<std::pair<int, Matrix<double>>> result;
    ThreadPool pool(std::thread::hardware_concurrency());
    std::vector<std::future<void>> tasks;

    std::mutex mtx;
    for(auto &&i: base) tasks.push_back(pool.enqueue(one_th, i, &result, &mtx));

    for(auto &&el: tasks) el.get();
    return result;
  }

 private:
  static void one_th(std::string &line, std::vector<std::pair<int, Matrix<double>>> *result, std::mutex *mtx) {
    size_t pos = line.find(",");
    int value = std::stoi(line.substr(0, pos));
    line.erase(0, pos + 1);
    auto mtr = Matrix<double>(768, 1);
    for(int i = 0; i < 768; ++i) {
      pos = line.find(",");
      mtr(i, 0) = std::stoi(line.substr(0, pos)) / NORMA;
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
