#pragma once
#include "my_matrix.h"
// транспонс

//using namespace victoriv;

class NeuronMatrix: public Matrix<double> {
 private:
  NeuronMatrix();

 public:
  NeuronMatrix(int value);
  NeuronMatrix(int value_row, int value_column);
  NeuronMatrix(NeuronMatrix &&other) noexcept;
  NeuronMatrix(const NeuronMatrix &other);
  auto operator=(const NeuronMatrix &other) -> NeuronMatrix &;
  auto operator=(NeuronMatrix &&other) noexcept -> NeuronMatrix &;
  NeuronMatrix operator*(const NeuronMatrix &other);
  NeuronMatrix mulMatrix(const NeuronMatrix &other, bool sigmoid, bool swap);
  NeuronMatrix mulMatrix(const NeuronMatrix &other, bool swap = true);
  NeuronMatrix transpose();

  std::shared_ptr<NeuronMatrix> transport() {
//    std::shared_ptr<NeuronMatrix> result = std::static_pointer_cast<NeuronMatrix>(Matrix<double>::trans());
//    return Matrix<double>::trans();
//    return result;
    return std::static_pointer_cast<NeuronMatrix>(Matrix<double>::trans());
  }

//  NeuronMatrix* trans() override {
//    return static_cast<NeuronMatrix*>(Matrix<double>::trans());
//  }
};
