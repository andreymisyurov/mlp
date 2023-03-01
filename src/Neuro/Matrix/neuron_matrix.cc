#include "neuron_matrix.h"

using namespace s21;

NeuronMatrix::NeuronMatrix() = default;
NeuronMatrix::NeuronMatrix(int value) : Matrix<double>(value){}
NeuronMatrix::NeuronMatrix(int value_row, int value_column) : Matrix<double>(value_row, value_column){}
NeuronMatrix::NeuronMatrix(NeuronMatrix &&other) noexcept { *this = other; }
NeuronMatrix::NeuronMatrix(const NeuronMatrix &other)  : Matrix(other) { *this = other; }

NeuronMatrix &NeuronMatrix::operator=(const NeuronMatrix &other) {
  NeuronMatrix temp(other.m_row, other.m_column);
  copyMatrix(other.m_row, other.m_column, temp, other);

  std::swap(m_row, temp.m_row);
  std::swap(m_column, temp.m_column);
  std::swap(m_matrix, temp.m_matrix);
  return *this;
}

NeuronMatrix & NeuronMatrix::operator=(NeuronMatrix &&other) noexcept {
  if (this != &other) {
    std::swap(m_column, other.m_column);
    std::swap(m_row, other.m_row);
    std::swap(m_matrix, other.m_matrix);
  }
  return *this;
}

NeuronMatrix NeuronMatrix::operator*(const NeuronMatrix &other) {
  NeuronMatrix temp(*this);
  temp.mulMatrix(other);
  return temp;
}

NeuronMatrix NeuronMatrix::mulMatrix(const NeuronMatrix &other, bool sigmoid,
                                     bool swap) {
  if (!sigmoid) {
    return mulMatrix(other, swap);
  }
  checkForMult(other);
  NeuronMatrix result(m_row, other.m_column);
  for (int i = 0; i < result.m_row; ++i) {
    for (int j = 0; j < result.m_column; ++j) {
      result.m_matrix[i][j] = 0;
      for (int k = 0; k < m_column; ++k) {
        result.m_matrix[i][j] += m_matrix[i][k] * other.m_matrix[k][j];
      }
      result.m_matrix[i][j] = 1. / (1 + exp(-result.m_matrix[i][j]));
    }
  }
  if (swap) {
    std::swap(*this, result);
    return *this;
  }
  return result;
}

NeuronMatrix NeuronMatrix::mulMatrix(const NeuronMatrix &other, bool swap) {
  checkForMult(other);
  NeuronMatrix result(m_row, other.m_column);
  for (int i = 0; i < result.m_row; ++i) {
    for (int j = 0; j < result.m_column; ++j) {
      result.m_matrix[i][j] = 0;
      for (int k = 0; k < m_column; ++k) {
        result.m_matrix[i][j] += m_matrix[i][k] * other.m_matrix[k][j];
      }
    }
  }
  if (swap) {
    std::swap(*this, result);
    return *this;
  }
  return result;
}

std::shared_ptr<NeuronMatrix> NeuronMatrix::transport() {
  return std::static_pointer_cast<NeuronMatrix>(Matrix<double>::trans());
}
