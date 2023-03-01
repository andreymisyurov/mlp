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
  PerzeptronMtx alfa(5);
  int count = 1;
  analytical_data data;
  analytical_data prev_data;
  prev_data.average_accuracy = -1.;
  while(data.average_accuracy > prev_data.average_accuracy) {
    alfa.import_data("/Users/gonzo/program/school21/general/CPP7_MLP-0/src/data_weights/data_" + std::to_string(count - 1) + "e_3l_512.txt");
    prev_data = data;
    data =
        alfa.test("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/emnist-letters/emnist-letters-test.csv", 1.0);
    std::cout << "accuracy = " << data.average_accuracy << std::endl;
    alfa.learn("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/emnist-letters/emnist-letters-train.csv");
    alfa.export_data("/Users/gonzo/program/school21/general/CPP7_MLP-0/src/data_weights/data_" + std::to_string(count++) + "e_3l_512.txt");
  }
  return 0;
}

// 3l 256
//accuracy = 0.0594595
//accuracy = 0.793041
//accuracy = 0.829189
//accuracy = 0.843784
//accuracy = 0.851486
//accuracy = 0.852973
//accuracy = 0.862162
//accuracy = 0.867703
//accuracy = 0.876419
//accuracy = 0.878446

// 5l 256
//accuracy = 0.00101351
//accuracy = 0.593784
//accuracy = 0.763649
//accuracy = 0.795405
//accuracy = 0.831824
//accuracy = 0.840541
//accuracy = 0.846486
//accuracy = 0.849122
//accuracy = 0.853581
//accuracy = 0.846824

// 5l 512
//accuracy = 0.0764865
//accuracy = 0.741149
//accuracy = 0.817973
//accuracy = 0.844392
//accuracy = 0.854324
//accuracy = 0.866014
//accuracy = 0.868378
