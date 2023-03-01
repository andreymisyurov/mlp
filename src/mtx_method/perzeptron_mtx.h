#include "../parser.h"
#include <optional>
#include <vector>

class PerzeptronMtx {
 public:
  PerzeptronMtx(): weight_first(Matrix<double>(768, 512)),
      weight_extra(Matrix<double>(512, 256)),
      weight_second(Matrix<double>(256, 26)) {
    feel_random(weight_first);
    feel_random(weight_extra);
    feel_random(weight_second);
  }

  void learn(const std::string &path, int8_t epochs = 1) {
    Parser temp;
    auto base = temp.getMatrix(path);
    while(epochs) {
      for(auto &&el: base) {
        predict(el.second, el.first);
      }
      std::cout << "прошла " << (int)epochs << " эпоха" << std::endl;
      --epochs;
    }
  }

  double test(const std::string &path) {
    Parser temp;
    auto base = temp.getMatrix(path);
    int pass = base.size();
    for(auto &&el: base) {
      auto control = predict(el.second);
      if(el.first != control) --pass;
    }
    return static_cast<double>(pass) / base.size();
  }

  int predict(const Matrix<double> &layer_main, std::optional<int> correct = std::nullopt) {
    Matrix<double> layer_copy = layer_main; // 768

    Matrix<double> layer_first = layer_main; // 512
    layer_first.mulMatrix(weight_first, true);

    Matrix<double> layer_extra = layer_first; // 256
    layer_extra.mulMatrix(weight_extra, true);

    Matrix<double> layer_second = layer_extra; // 26
    layer_second.mulMatrix(weight_second, true);

    double maximum = layer_second(0, 0);
    int num = 0;
    for(int i = 0; i < layer_second.getColum(); ++i) {
//      std::cout << i + 1 << " " << layer_second(0, i) << std::endl;
      if(layer_second(0, i) > maximum) {
        maximum = layer_second(0, i);
        num = i;
      }
    }

    if(correct) {
      Matrix<double> errors_1(26, 1);
      for(int i = 0; i < 26; ++i) {
        double o_i = layer_second(0, i);
        double err_i = -o_i * (1 - o_i) * ((i + 1 == correct.value() ? 1. : 0.) - o_i);
        errors_1(i, 0) = err_i;
      }

      Matrix<double> bw_error_2 = weight_second * errors_1; // 256 x 1

      Matrix<double> errors_2(256, 1);
      for(int i = 0; i < 256; ++i) {
        double o_j = layer_extra(0, i);
        errors_2(i, 0) = o_j * (1 - o_j) * bw_error_2(i, 0);
      }

      Matrix<double> bw_error_3 = weight_extra * errors_2; // 512 x 1

      Matrix<double> errors_3(512, 1);
      for(int i = 0; i < 512; ++i) {
        double o_j = layer_first(0, i);
        errors_3(i, 0) = o_j * (1 - o_j) * bw_error_3(i, 0);
      }

      Matrix<double> d_weight_2_3(256, 26);
      for(int i = 0; i < 256; ++i) {
        for(int j = 0; j < 26; ++j) {
          d_weight_2_3(i, j) = layer_extra(0, i) * errors_1(j, 0) * 0.1;
        }
      }

      Matrix<double> d_weight_1_2(512, 256);
      for(int i = 0; i < 512; ++i) {
        for(int j = 0; j < 256; ++j) {
          d_weight_1_2(i, j) = layer_first(0, i) * errors_2(j, 0) * 0.1;
        }
      }

      Matrix<double> d_weight_0_1(768, 512);
      for(int i = 0; i < 768; ++i) {
        for(int j = 0; j < 512; ++j) {
          d_weight_0_1(i, j) = layer_copy(0, i) * errors_3(j, 0) * 0.1;
        }
      }

      weight_second -= d_weight_2_3;
      weight_extra -= d_weight_1_2;
      weight_first -= d_weight_0_1;
    }
    return num + 1;
  }

 private:
  Matrix<double> weight_first; // 768 x 512
  Matrix<double> weight_extra; // 512 x 256
  Matrix<double> weight_second; // 256 x 26
  void feel_random(Matrix<double> &mtrx) {
    srand(time(0));
    for(int i = 0; i < mtrx.getRow(); ++i) {
      for(int j = 0; j < mtrx.getColum(); ++j) {
        mtrx(i, j) = (rand() % 256) / 255. - 0.5;
      }
    }
  }

};
