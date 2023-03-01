#include "../parser.h"
#include <optional>
#include <vector>

class PerzeptronMtx {
 public:
  PerzeptronMtx(int hidden_layers = 2): weights(new std::vector<NeuronMatrix>()) {
    int start_rows = 256;
    if(hidden_layers > 5 || hidden_layers < 1) throw MyException("incorrect input for hidden layers");
    if(start_rows > 1024 || (start_rows / hidden_layers) <= 26) throw MyException("incorrect data for first layer");
    weights->emplace_back(std::move(NeuronMatrix(768, start_rows)));
    feel_random(weights->back());
    --hidden_layers;
    while(hidden_layers--) {
      weights->emplace_back(std::move(NeuronMatrix(start_rows, start_rows / 2)));
      feel_random(weights->back());
      start_rows /= 2;
    }
    weights->emplace_back(std::move(NeuronMatrix(start_rows, 26)));
    feel_random(weights->back());
  }
  ~PerzeptronMtx() {
    delete weights;
  }

  void exp(const std::string &path) {
    Parser men;
    men.export_data(weights, path);
  }

  void imp(const std::string &path) {
    Parser men;
    men.import_data(weights, path);
  }

  void learn(const std::string &path, int8_t epochs = 1) {
    Parser temp;
    auto base = temp.getMatrix(path);
    ThreadPool pool(std::thread::hardware_concurrency());
    std::vector<std::future<int>> tasks;
    while(epochs--)
      for(auto &&el: base)
        tasks.push_back(pool.enqueue(prediction, el.second, weights, m_step, el.first));
    for(auto &&el: tasks) el.get();
  }

  double test(const std::string &path) {
    Parser temp;
    ThreadPool pool(std::thread::hardware_concurrency());
    std::vector<std::future<void>> tasks;
    auto base = temp.getMatrix(path);
    int pass = base.size();
    for(auto &&el: base) tasks.push_back(pool.enqueue(check_case, &pass, el.first, el.second, weights, m_step));
    for(auto &&el: tasks) el.get();
    return static_cast<double>(pass) / base.size();
  }

  int predict(const NeuronMatrix &layer_main) {
    return prediction(layer_main, weights, m_step);
  }

    // weights input 768 x 512 -------- 512 x 256 ---------- 256 x 26
  static int prediction(const NeuronMatrix &layer_main, std::vector<NeuronMatrix> *weights, double in_step, std::optional<int> correct = std::nullopt) {
    // layers input 1 x 768/512/256/26
    // техдолг - убрать подсчет ошибков на нулевом слое
    std::vector<NeuronMatrix> layers{layer_main};
    for(auto &el: *weights) {
      // перемножаем первый слой на матрицы весов тем самым получаем остальные слои
    //      auto temp = layers.back().mulMatrix(el, true, false);
      layers.emplace_back(std::move(layers.back().mulMatrix(el, true, false)));
    }

    double maximum = layers.back()(0, 0);
    int num = 0;
    for(int i = 0; i < layers.back().getColum(); ++i) {
      if(layers.back()(0, i) > maximum) {
        maximum = layers.back()(0, i);
        num = i;
      }
    }
    if(correct) {
      std::vector<NeuronMatrix> sum_err_weight_prev(*weights);
      std::vector<NeuronMatrix> errors;
      std::vector<NeuronMatrix> d_weight = *weights;
      for(auto &&el: layers)
        errors.emplace_back(std::move(el.transpose()));

      for(int i = 0; i < 26; ++i) {
        double o_i = layers.back()(0, i);
        double err_i = -o_i * (1 - o_i) * ((i + 1 == correct.value() ? 1. : 0.) - o_i);
        errors.back()(i, 0) = err_i;
      }

      sum_err_weight_prev.back() = weights->back() * errors.back();

      for(int i = errors.size() - 2; i > 0; --i) {
        for(int j = 0; j < errors[i].getRow(); ++j) {
          double o_j = errors[i](j, 0);
          errors[i](j, 0) = o_j * (1 - o_j) * sum_err_weight_prev[i](j, 0);
        }
        sum_err_weight_prev[i - 1] = (*weights)[i - 1] * errors[i];
      }


      int l = 0;
      for(auto &el: d_weight) {
        for(int i = 0; i < el.getRow(); ++i) {
          for(int j = 0; j < el.getColum(); ++j) {
            el(i, j) = layers[l](0, i) * errors[l + 1](j, 0) * in_step;
          }
        }
        ++l;
      }
      auto minus = d_weight.begin();
      for(auto &&el: *weights) {
        el -= *minus;
        ++minus;
      }
    }
    return num + 1;
  }


  static void check_case(int *pass, int correct, NeuronMatrix mtrx, std::vector<NeuronMatrix> *weights, double in_step) {
    auto control = prediction(mtrx, weights, in_step);
    if(correct != control) {
      *pass -= 1;
    }
  }

 private:
  std::vector<NeuronMatrix> *weights;
  double m_step = 0.15;
  void feel_random(NeuronMatrix &mtrx) {
    srand(time(0));
    for(int i = 0; i < mtrx.getRow(); ++i)
      for(int j = 0; j < mtrx.getColum(); ++j)
        mtrx(i, j) = (rand() % 256) / 255. - 0.5;
  }


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
