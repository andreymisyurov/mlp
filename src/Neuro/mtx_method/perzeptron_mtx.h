#ifndef SRC_MTX_METHOD_PERZEPTRON_MTX_H_
#define SRC_MTX_METHOD_PERZEPTRON_MTX_H_

#include <optional>
#include <vector>

#include "parser.h"
#include "perceptron_abstract.h"
#include "timer.h"

namespace s21 {

/** КЛАСС PerceptronMtx
 * Реализует структуру нейронной сети, в которой веса хранятся в матрицах весов.
 * Наследован от абстрактного класса Perceptron
 */
class PerzeptronMtx: public Perceptron {
 public:
  explicit PerzeptronMtx(int in_hidden_layers = 2, int in_first_layer = 256);
  PerzeptronMtx(const PerzeptronMtx &other);
  PerzeptronMtx(PerzeptronMtx &&other) noexcept;
  PerzeptronMtx &operator=(PerzeptronMtx &&other) noexcept;
  PerzeptronMtx &operator=(const PerzeptronMtx &other);
  ~PerzeptronMtx() override;

 public:
  auto learn(const std::string &in_path, int in_epochs)           -> std::pair<int, std::vector<double>> override;
  auto crossValidation(const std::string &in_path, int in_k)      -> std::pair<int, std::vector<double>> override;
  auto test(const std::string &in_path, double in_data_part)      -> analytical_data override;
  auto predict(const NeuronMatrix &in_layer)                      -> int override;
  auto exportDataBase(const std::string &in_path)                 -> void override;
  auto importDataBase(const std::string &in_path)                 -> void override;

// private:
 public:
/** СТАТИЕСКИЙ МЕТОД prediction. Является основным методом класса, задействован в методах learn, test, predict.
 * Реализован с использованием многопоточности.
 *
 * @param in_layer матрица значений входного слоя вида 1x784
 * @param inout_weights указатель матрицу значений весов, веса меняются при обучении
 * @param in_step шаг обучения
 * @param in_correct ожидаемый ответ, значение int от 0 до 25 включительно
 * @return возвращает пару, где first - это значение от 1 до 26 включительно.\n
 * Предсказание сети(номер нейрона выходного слоя с наибольшим зарядом),\n
 * а second - матрица всех зарядов нейронов выходного слоя(необходимо для аналитики)
 */
  auto static prediction(const NeuronMatrix &in_layer,
      std::vector<NeuronMatrix> *inout_weights,
      double in_step,
      std::optional<int> in_correct = std::nullopt)               -> std::pair<int, NeuronMatrix>;
  /** СТАТИЕСКИЙ МЕТОД analyzeCase - анализирует поведение нейронной сети, собирает аналитику.
   *
   * @param inout_count результирующи аргумент, изначально равен количеству тест-кейсов. Изменяется в потоках
   * @param in_correct ожидаемый ответ сети
   * @param in_mtrx матрица 26ти зарядов нейронов выходного слоя
   * @param in_weights указатель на матрицу весов
   * @param in_step шаг обучения
   * @param out_analytic вектор с аналитическими данными, который в потоках накапливает следующие значения:\n
   * *[0] - True Positive когда заряд больше 0,85 у правильного нейрона\n
   * *[1] - False Negative когда заряд больше 0,85 у НЕ правильного нейрона\n
   * *[2] - False Positive когда заряд меньше 0,85 у НЕ правильного нейрона\n
   * *[3] - True Negative когда заряд меньше 0,85 у правильного нейрона
   * @param in_mutex мьютекс для контроля доступа к аргументам inout_pass и inout_analytics
   */
  auto static analyzeCase(int *inout_count,
      int in_correct, const NeuronMatrix &in_mtrx,
      const std::vector<NeuronMatrix> *in_weights,
      double in_step, std::vector<int> *out_analytic,
      std::mutex *in_mutex)                                       -> void;
  /** СТАТИЕСКИЙ МЕТОД feel_random заполняет матрицу случайными значениями от -0.5 до 0.5
   *
   * @param out_mtrx результирующий аргумент, матрица которую нужно заполнить случайными значениями
   */
  auto static feel_random(NeuronMatrix &out_mtrx)                 -> void;
  /** СТАТИЕСКИЙ МЕТОД getMiddleErr высчитывает и усредняет ошибку на выходном слое
   *
   * @param in_data матрица выходного слоя состоящая из 26 значений зарядов нейронов
   * @return возвращает значение средней ошибки конкретного одного предсказания
   */
  auto static getMiddleErr(const std::pair<int,
                            NeuronMatrix> &in_data)               -> double;

 private:
  std::vector<NeuronMatrix> *m_weights = nullptr;
  const double k_step = 0.15;
};

} // namespace s21

#endif // SRC_MTX_METHOD_PERZEPTRON_MTX_H_
