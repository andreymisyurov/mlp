#pragma once
#include "my_matrix.h"
#include <vector>
#include "../ThreadPool/thread_pool.h"
#include "./../Timer/timer.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <vector>

using namespace victoriv;
using vec_iter = std::vector<int>::iterator;

class NeuronMatrix: public Matrix<double> {
 private:
  NeuronMatrix();

 public:
  explicit NeuronMatrix(int value);
  NeuronMatrix(int value_row, int value_column);
  NeuronMatrix(NeuronMatrix &&other) noexcept;
  NeuronMatrix(const NeuronMatrix &other);
  auto operator=(const NeuronMatrix &other) -> NeuronMatrix &;
  auto operator=(NeuronMatrix &&other) noexcept -> NeuronMatrix &;
  NeuronMatrix operator*(const NeuronMatrix &other);
  NeuronMatrix mulMatrix(const NeuronMatrix &other, bool sigmoid, bool swap);
  NeuronMatrix mulMatrix(const NeuronMatrix &other, bool swap = true);
  NeuronMatrix transpose();
};
