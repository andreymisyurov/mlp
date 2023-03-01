#pragma once
#include "my_matrix.h"
#include <vector>
#include "./../thread_pool.h"
#include "./../Timer/timer.h"
#include <SDL/SDL_thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <vector>

using namespace victoriv;
using vec_iter = std::vector<int>::iterator;

class NeuronMatrix: public Matrix<double> {
 public:
  NeuronMatrix() = delete;
  explicit NeuronMatrix(int value): Matrix<double>(value) {
  };
  NeuronMatrix(int value_row, int value_column): Matrix<double>(value_row, value_column) {
  };
  NeuronMatrix(NeuronMatrix &&other): NeuronMatrix(1) {
    *this = other;
  };
  NeuronMatrix(const NeuronMatrix &other): NeuronMatrix(1) {
    *this = other;
  };

  auto operator=(const NeuronMatrix &other) -> NeuronMatrix & {
    NeuronMatrix temp(other.m_row, other.m_column);
    copyMatrix(other.m_row, other.m_column, temp, other);

    std::swap(m_row, temp.m_row);
    std::swap(m_column, temp.m_column);
    std::swap(m_matrix, temp.m_matrix);
    return *this;
  };

  auto operator=(NeuronMatrix &&other) -> NeuronMatrix & {
    if(this != &other) {
      std::swap(m_column, other.m_column);
      std::swap(m_row, other.m_row);
      std::swap(m_matrix, other.m_matrix);
    }
    return *this;
  };

  NeuronMatrix operator*(const NeuronMatrix &other) {
    NeuronMatrix temp(*this);
    temp.mulMatrix(other);
    return temp;
  }

  NeuronMatrix mulMatrix(const NeuronMatrix &other, bool sigmoid, bool swap) {
    if(!sigmoid) {
      return mulMatrix(other, swap);
    }
    checkForMult(other);
    NeuronMatrix result(m_row, other.m_column);
    for(int i = 0; i < result.m_row; ++i) {
      for(int j = 0; j < result.m_column; ++j) {
        result.m_matrix[i][j] = 0;
        for(int k = 0; k < m_column; ++k) {
          result.m_matrix[i][j] += m_matrix[i][k] * other.m_matrix[k][j];
        }
        result.m_matrix[i][j] = 1. / (1 + exp(-result.m_matrix[i][j]));
      }
    }
    if(swap) {
      std::swap(*this, result);
      return *this;
    }
    return result;
  }

  NeuronMatrix mulMatrix(const NeuronMatrix &other, bool swap = true) {
    checkForMult(other);
    NeuronMatrix result(m_row, other.m_column);
    for(int i = 0; i < result.m_row; ++i) {
      for(int j = 0; j < result.m_column; ++j) {
        result.m_matrix[i][j] = 0;
        for(int k = 0; k < m_column; ++k) {
          result.m_matrix[i][j] += m_matrix[i][k] * other.m_matrix[k][j];
        }
      }
    }
    if(swap) {
      std::swap(*this, result);
      return *this;
    }
    return result;

  }

  NeuronMatrix transpose() {
    NeuronMatrix result(m_column, m_row);
    for(int i = 0; i < result.m_row; ++i) {
      for(int j = 0; j < result.m_column; ++j) {
        result.m_matrix[i][j] = m_matrix[j][i];
      }
    }
    return result;
  }
};
