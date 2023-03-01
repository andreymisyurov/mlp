#ifndef SRC_MATRIX_NEURON_MATRIX_H
#define SRC_MATRIX_NEURON_MATRIX_H

#include "my_matrix.h"

namespace s21 {

class NeuronMatrix : public Matrix<double> {
 private:
  NeuronMatrix();

 public:
  explicit NeuronMatrix(int value);
  NeuronMatrix(int value_row, int value_column);
  NeuronMatrix(NeuronMatrix &&other) noexcept;
  NeuronMatrix(const NeuronMatrix &other);

 public:
  auto operator=(const NeuronMatrix &other)                           -> NeuronMatrix &;
  auto operator=(NeuronMatrix &&other) noexcept                       -> NeuronMatrix &;
  auto operator*(const NeuronMatrix &other)                           -> NeuronMatrix;
  auto mulMatrix(const NeuronMatrix &other, bool sigmoid, bool swap)  -> NeuronMatrix;
  auto mulMatrix(const NeuronMatrix &other, bool swap = true)         -> NeuronMatrix;
  NeuronMatrix transpose() = delete;
  std::shared_ptr<NeuronMatrix> transport();
};

}  // namespace s21

#endif  // SRC_MATRIX_NEURON_MATRIX_H
