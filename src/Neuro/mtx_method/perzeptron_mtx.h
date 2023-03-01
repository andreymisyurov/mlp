#include "../Parser/parser.h"
#include <optional>
#include <vector>
#include "../Perceptron/perceptron_abstract.h"
#include "./../Timer/timer.h"

class PerzeptronMtx: public Perceptron {
 public:
  // допилить правило пяти
  explicit PerzeptronMtx(int hidden_layers = 2, int first_layer = 256);
  ~PerzeptronMtx() override;

  void export_data(const std::string &path) override;
  void import_data(const std::string &path) override;
  void learn(const std::string &path, int8_t epochs = 1) override;
  analytical_data test(const std::string &path, double part_of_tests = 1.0) override;
  int predict(const NeuronMatrix &layer_main) override;

 private:
  static int prediction(const NeuronMatrix &layer_main, std::vector<NeuronMatrix> *weights, double in_step, std::optional<int> correct = std::nullopt);
  static void check_case(int *pass, int correct, NeuronMatrix mtrx, std::vector<NeuronMatrix> *weights, double in_step);
  void feel_random(NeuronMatrix &mtrx);

  std::vector<NeuronMatrix> *weights;
  double m_step = 0.15;
  Parser *parser = nullptr;

// single thread realization
//  void learn(const std::string &path, int8_t epochs = 1) {
//    Parser temp;
//    auto base = temp.getMatrix(path);
//    while(epochs) {
//      for (auto &&el:base)
//        prediction(el.second, el.first);
//      --epochs;
//    }
//  }

//  int prediction(const NeuronMatrix &layer_main, std::optional<int> correct = std::nullopt) {
//    std::vector<NeuronMatrix> layers;
//    layers.push_back(layer_main);
//    for(auto &el: *weights) layers.push_back(layers.back().mulMatrix(el, false));
//
//    double maximum = layers.back()(0, 0);
//    int num = 0;
//    for(int i = 0; i < layers.back().getColum(); ++i) {
//      if(layers.back()(0, i) > maximum) {
//        maximum = layers.back()(0, i);
//        num = i;
//      }
//    }
//
//    if(correct) {
//      std::vector<NeuronMatrix> sum_err_weight_prev(*weights);
//      std::vector<NeuronMatrix> errors;
//      for(auto &&el: layers) errors.push_back(el.transpose());
//
//      for(int i = 0; i < layers.back().getColum(); ++i) {
//        double o_i = layers.back()(0, i);
//        double err_i = -o_i * (1 - o_i) * ((i + 1 == correct.value() ? 1. : 0.) - o_i);
//        errors.back()(i, 0) = err_i;
//      }
//
//      sum_err_weight_prev.back() = weights->back() * errors.back();
//      for(int i = errors.size() - 2; i > 0; --i) {
//        for(int j = 0; j < errors[i].getRow(); ++j) {
//          double o_j = errors[i](j, 0);
//          errors[i](j, 0) = o_j * (1 - o_j) * sum_err_weight_prev[i](j, 0);
//        }
//        sum_err_weight_prev[i - 1] = (*weights)[i - 1] * errors[i];
//      }
//      std::vector<NeuronMatrix> d_weight = *weights;
//
//      int l = 0;
//      for(auto &el: d_weight) {
//        for(int i = 0; i < el.getRow(); ++i)
//          for(int j = 0; j < el.getColum(); ++j) {
//            el(i, j) = layers[l](0, i) * errors[l + 1](j, 0) * 0.1;
//          }
//        ++l;
//      }
//
//      auto minus = d_weight.begin();
//      for(auto &el: *weights) {
//        el -= *minus;
//        ++minus;
//      }
//    }
//    return num + 1;
//  }

//  double test(const std::string &path) {
//    Parser temp;
//    auto base = temp.getMatrix(path);
//    int pass = 0;
//    for(auto el: base)
//      pass += check_case(el.first, el.second);
//    return static_cast<double>(pass) / base.size();
//  }

//  int check_case(int correct, NeuronMatrix mtrx) {
//    auto control = prediction(mtrx);
//    if(correct != control) {
//      return 0;
//    }
//    return 1;
//  }
//

};
