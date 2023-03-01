#include "parser.h"
#include "./Timer/timer.h"

int main() {
  Parser temp;
  Timer timer;
  timer.Start();
  auto base = temp.getMatrix("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/emnist-letters/emnist-letters-train.csv");
  timer.Stop();
  timer.ShowTime();
  return 0;
}
