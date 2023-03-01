#include "Perceptron/perceptron_abstract.h"
#include "mtx_method/perzeptron_mtx.h"
#include "perzeptron_grph.h"
#include "Parser/parser.h"
#include "Timer/timer.h"

using namespace s21;

int main() {
  Perceptron *first = new PerzeptronMtx(2, 256);
  Perceptron *second = new PerzeptronGrph(2, 256);
  first->importDataBase("/Users/gonzo/program/school21/general/CPP7_MLP-0/src/Neuro/resources/best_weights.bin");
  second->importDataBase("/Users/gonzo/program/school21/general/CPP7_MLP-0/src/Neuro/resources/best_weights.bin");
  int count = 10;
  Timer timer_m_10(true);
  while(count--) {
    first->test("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/emnist-letters/emnist-letters-test.csv",
        1.0);
  }
  timer_m_10.stop();
  timer_m_10.showTime("matrix struct MLP called tests 10 times for ");

  count = 10;
  Timer timer_g_10(true);
  while(count--) {
    second->test("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/emnist-letters/emnist-letters-test.csv",
        1.0);
  }
  timer_g_10.stop();
  timer_g_10.showTime("graph struct MLP calls tested 10 times for ");
  return 0;
}

//matrix struct MLP called tests 10 times for 78.2126 sec
//graph struct MLP calls tested 10 times for 723.99 sec
//matrix struct MLP called tests 100 times for 876.835 sec
//graph struct MLP calls tested 100 times for 6975.95 sec
//matrix struct MLP called tests 1000 times for 7660.74 sec
