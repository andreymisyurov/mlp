#include "Parser/parser.h"
#include "./Timer/timer.h"
#include "./mtx_method/perzeptron_mtx.h"
#include "./Matrix/neuron_matrix.h"
#include <iostream>

// just multiple: start = 256, layers = 2, step = 0.15
// Time of execute: 888.986s, accuracy = 0.790878%

// emplace and std::move: start = 256, layers = 2, step = 0.15
// Time of execute: 827.604s, accuracy = 0.787568%

int main() {
  PerzeptronMtx alfa(2);
  Timer timer_test;
  timer_test.Start();


//  alfa.learn("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/emnist-letters/emnist-letters-train.csv", 2);
//  alfa.learn("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/one_al.txt", 1);
  alfa.import_data("/Users/gonzo/program/school21/general/CPP7_MLP-0/src/data_weights/data_2e_3l_256.txt");
//  alfa.export_data("/Users/gonzo/program/school21/general/CPP7_MLP-0/src/data_weights/data_1e_2l_256.txt");
//  alfa.export_data("/Users/gonzo/program/school21/general/CPP7_MLP-0/src/data_weights/data_2e_3l_256.txt");
//  auto temp = Parser::parse_one_mtrx("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/one_al.txt");
  auto data = alfa.test("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/emnist-letters/emnist-letters-test.csv", 1.0);

  timer_test.Stop();
  timer_test.ShowTime();

  std::cout << "accuracy = " << data.average_accuracy << " time = " << data.time;


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
