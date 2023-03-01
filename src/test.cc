#include "parser.h"
#include "./Timer/timer.h"
#include "./mtx_method/perzeptron_mtx.h"
#include <time.h>
#include <unistd.h>


int main() {
//  Parser temp;
//  Matrix<double> test = temp.parse_one_mtrx("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/one_al.txt");
  PerzeptronMtx alfa;
//  std::cout << alfa.predict(test, 3);
  Timer timer_learn;
  timer_learn.Start();
  alfa.learn("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/emnist-letters/emnist-letters-train.csv");
//  alfa.learn("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/emnist-letters/emnist-letters-train.csv");
  timer_learn.Stop();
  timer_learn.ShowTime();

std::cout << std::endl;

  Timer timer_test;
  timer_test.Start();
  std:: cout << alfa.test("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/emnist-letters/emnist-letters-test.csv") << std::endl;
  timer_test.Stop();
  timer_test.ShowTime();




//  Matrix<int> t1(7, 6);
//  Matrix<int> t2(6, 11);
  return 0;
}
