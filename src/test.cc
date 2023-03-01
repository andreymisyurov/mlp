#include "parser.h"
#include "./Timer/timer.h"
#include "./mtx_method/perzeptron_mtx.h"
#include "./Matrix/neuron_matrix.h"
#include <iostream>

// just multiple: start = 256, layers = 2, step = 0.15
// Time of execute: 888.986s, accuracy = 0.790878%

// emplace and std::move: start = 256, layers = 2, step = 0.15
// Time of execute: 827.604s, accuracy = 0.787568%

int main() {
  PerzeptronMtx alfa = PerzeptronMtx(2);
  Timer timer_test;
  timer_test.Start();


//  alfa.learn("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/emnist-letters/emnist-letters-train.csv");
//  alfa.learn("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/one_al.txt", 1);
  alfa.imp("/Users/gonzo/program/school21/general/CPP7_MLP-0/data.txt");

  auto accuracy = alfa.test("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/emnist-letters/emnist-letters-test.csv");

  timer_test.Stop();
  timer_test.ShowTime();

  std::cout << accuracy;

//  PerzeptronMtx tests = PerzeptronMtx(2);
//  Timer timer_test1;
//  timer_test1.Start();

//  tests.learn("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/emnist-letters/emnist-letters-train.csv");
//  alfa.learn("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/one_al.txt", 1);
//  tests.exp();
//  tests.imp();

//  auto accuracy1 = tests.test("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/emnist-letters/emnist-letters-test.csv");

//  timer_test1.Stop();
//  timer_test1.ShowTime();

//  std::cout << accuracy1;

  return 0;
}
