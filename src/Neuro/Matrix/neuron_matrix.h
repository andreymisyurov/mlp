#ifndef SRC_MATRIX_NEURON_MATRIX_H
#define SRC_MATRIX_NEURON_MATRIX_H

#include "my_matrix.h"

// транспонс

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
  NeuronMatrix transpose() = delete;
  std::shared_ptr<NeuronMatrix> transport();
};

#endif // SRC_MATRIX_NEURON_MATRIX_H
