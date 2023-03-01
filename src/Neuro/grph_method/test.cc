#include "perzeptron_grph.h"
#include "perzeptron_mtx.h"
#include "parser.h"

int main() {
  Perceptron *gabella = new PerzeptronGrph(2, 64);
  int* s = new int(15);
  std::cout << gabella->predict(Parser::parse_one_mtrx("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/one_al.txt"));
//  gabella->learn("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/emnist-letters/emnist-letters-train.csv", 1);
//  gabella->exportDataBase("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/tess_weights.bin");
//  gabella->prediction(Parser::parse_one_mtrx("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/one_al.txt"), 1);
//  gabella->prediction(Parser::parse_one_mtrx("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/one_al.txt"), 1);
//  gabella->prediction(Parser::parse_one_mtrx("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/one_al.txt"), 1);
//  gabella->prediction(Parser::parse_one_mtrx("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/one_al.txt"), 1);
//gabella->prediction(Parser::parse_one_mtrx("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/one_al.txt"), 14);
//gabella->prediction(Parser::parse_one_mtrx("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/one_al.txt"), 14);

//  std::cout << std::endl;
//  std::cout << std::endl;

//  PerzeptronMtx *gg = new PerzeptronMtx(2, 128);
//  gg->importDataBase("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/temp_weights.bin");
//  gg->predict(Parser::parse_one_mtrx("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/one_al.txt"));
//  gg->predict(Parser::parse_one_mtrx("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/one_al.txt"));
//  gg->predict(Parser::parse_one_mtrx("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/one_al.txt"));
//  gg->predict(Parser::parse_one_mtrx("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/one_al.txt"));
//  gg->predict(Parser::parse_one_mtrx("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/one_al.txt"));
//  gg->predict(Parser::parse_one_mtrx("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/one_al.txt"));

  return 0;
}
