#ifndef SRC_GRPH_METHOD_PERZEPTRON_GRPH_H_
#define SRC_GRPH_METHOD_PERZEPTRON_GRPH_H_

#include <optional>
#include <vector>

#include "parser.h"
#include "perceptron_abstract.h"
#include "timer.h"

namespace s21 {

/** КЛАСС PerceptronGrph
 * Реализует структуру нейронной сети, в которой веса хранятся в графах.
 * Наследован от абстрактного класса Perceptron
 */
class PerzeptronGrph: public Perceptron {
  struct GraphData;
 public:
  explicit PerzeptronGrph(int in_hidden_layers = 2, int in_first_layer = 256);
  ~PerzeptronGrph() override = default;

 public:
  auto learn(const std::string &in_path, int in_epochs) -> std::pair<int, std::vector<double>> override;
  auto crossValidation(const std::string &in_path, int in_k) -> std::pair<int, std::vector<double>> override;
  auto test(const std::string &in_path, double in_data_part) -> analytical_data override;
  auto predict(const NeuronMatrix &in_layer) -> int override;
  auto exportDataBase(const std::string &in_path) -> void override;
  auto importDataBase(const std::string &in_path) -> void override;

 private:

  /** МЕТОД backPropagation нужен для корректировки весов методом обратного распространения ошибки
   *
   * @param in_correct корректное(ожидаемое) значение
   */
  auto backPropagation(int in_correct) -> void;

  /** МЕТОД prediction - делает предсказания и корректировки весов(методом обратного распространения ошибки).\n
   * Содержит в себе forward-propagation и back-propagation.\n
   * Основной метод класса, используется в функциях learn, test, predict
   *
   * @param in_layer входящий слой из 784 значений нейронов в диапазоне от 0 до 1.
   * @param in_correct ожидаемый ответ сети, значение int от 0 до 25 включительно
   * @return возвращает пару, где first - это предсказание сети(номер нейрона выходного слоя с наибольшим\n
   * зарядом от 0 до 25), second - матрица всех зарядов нейронов выходного слоя(необходимо для отчета об обучении)
   */
  auto prediction(const NeuronMatrix &in_layer, std::optional<int> in_correct) -> std::pair<int, std::vector<double>>;
  /** МЕТОД analyzeCase - анализирует поведение нейронной сети, собирает аналитику.
   *
   * @param inout_count результирующи аргумент, изначально равен количеству тест-кейсов.
   * @param in_correct ожидаемый ответ сети
   * @param in_mtrx матрица 26ти зарядов нейронов выходного слоя
   * @param out_analytic результирующи аргумент - вектор с аналитическими данными,
   * который накапливает следующие значения:\n
   * *[0] - True Positive когда заряд больше 0,85 у правильного нейрона\n
   * *[1] - False Negative когда заряд больше 0,85 у НЕ правильного нейрона\n
   * *[2] - False Positive когда заряд меньше 0,85 у НЕ правильного нейрона\n
   * *[3] - True Negative когда заряд меньше 0,85 у правильного нейрона
   */
  auto analyzeCase(int *inout_count, int in_correct, const NeuronMatrix &in_mtrx,
                                                                            std::vector<int> *out_analytic) -> void;
  /** СТАТИЧЕСКИЙ МЕТОД figureOutCharge вычисляет заряд исходя из зарядов и значений весов предыдущего слоя
 *
 * @param in_layer входящий слой
 * @param in_num порядковый номер нейрона на выходящем слое
 * @return заряд нейрона под номером num выходного слоя, вещественное число
 */
  auto static figureOutCharge(const std::vector<GraphData> &in_layer, int in_num) -> double;
  /** СТАТИЧЕСКИЙ МЕТОД находит произведение строки весов входного нейрона на столбец ошибок выходных нейронов\n
   * используется в методе обратного распространения ошибки
   *
   * @param in_weights вектор с весами нейрона входного слоя
   * @param in_err вектор ошибок выходного слоя
   * @return возвращает сумму произведений пар векторов
   */
  auto static getErrorRow(const std::shared_ptr<std::vector<double>> &in_weights,
                                                                      const std::vector<double> &in_err) -> double;
  /** СТАТИЕСКИЙ МЕТОД getMiddleErr высчитывает и усредняет ошибку на выходном слое
   *
   * @param in_data пара из правильного ответа и вектора выходного слоя состоящая из 26 значений зарядов нейронов
   * @return возвращает значение средней ошибки конкретного одного предсказания
   */
  auto static getMiddleErr(const std::pair<int, std::vector<double>> &in_data) -> double;

 private:
  std::shared_ptr<std::vector<std::vector<GraphData>>> m_layers = nullptr;
  const double k_step = 0.15;

  /** СТРУКТУРА GraphData является вложенной и используется для хранения данных исключительно внутри класса.
   * Содержит заряд нейрона и вектор с весами и связями
   */
  struct GraphData {
    double m_charge;
    std::shared_ptr<std::vector<double>> m_connect = nullptr;
  };
};

#endif // SRC_MTX_METHOD_PERZEPTRON_MTX_H_

} // namespace s21
