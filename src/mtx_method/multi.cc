#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>

#include <SDL/SDL_thread.h>
#include <vector>

using namespace std;
const int Nrow = 4;
const int Ncols = 4;

vector<double> row[Nrow];
vector<double> col[Ncols];
double C[Nrow][Ncols];

int dotProduct(void *data) {
  int i, j;
  sscanf((char *)data, "%d %d", &i, &j);

  int len, na, nb;
  na = row[i].size();
  nb = col[j].size();
  if(na > nb) len = nb;
  else len = na;

  double sum = 0.0;
  for(int k = 0; k < len; ++k) {
    sum += row[i][k] * col[j][k];
  }
  C[i][j] = sum;
  return 0;
}

void printMat(double *m, int nRow, int nCol) {
  for(int i = 0; i < nRow; ++i) {
    std::cout << endl;
    for(int j = 0; j < nCol; ++j) {
      printf("%3.2f\t", *(m+i*nCol+j));
    }
  }
}

int main() {
  SDL_Thread *sumThread;

  double m1[][3] = {{0.8, -0, 5},
                 {0.8, -0, 5},
                 {0.8, -0, 5},
                 {0.8, -0, 5}};
  double m2[][4] = {{0.8,  -0.5, 0.8, 0.2},
                 {-0.5, 0.1,  0.8, -1.4},
                 {-0.5, 0.1,  0.8, -0.5}};

  for(int i = 0; i < Nrow; ++i) {
    for(int j = 0; j < 3; ++j) {
      row[i].push_back(m1[i][j]);
    }
  }

  for(int j = 0; j < Ncols; ++j) {
    for(int i = 0; i < 3; ++i) {
      col[j].push_back(m2[i][j]);
    }
  }

  char buffer[100];

  for(int i = 0; i < Nrow; ++i) {
    for(int j = 0; j < Ncols; ++j) {
      snprintf(buffer, 100, "%d %d", i, j);
      sumThread = SDL_CreateThread(dotProduct, (void*) buffer);

      if(sumThread == NULL) {
        std::cout << "Gabella" << endl;
      } else {
        SDL_WaitThread(sumThread, NULL);
      }
    }
  }

  printMat(&m1[0][0], Nrow, 3);
  std::cout <<endl;
  std::cout <<endl;
  printMat(&m2[0][0], Ncols, 3);
  std::cout <<endl;
  std::cout <<endl;
  printMat(&C[0][0], Nrow, Ncols);
  return 0;
}
