#include "../Matrix/neuron_matrix.h"

typedef struct analytical_data {
  // средняя точность предсказаний
  double average_accuracy;
  // прецизионность(пока что хз что это такое)
  double precision;
  // полнота, пока что хз чо это
  double recall;
  // f-мера хз что это
  double f_measure;
  // полное время затраченное на тестирование
  double time;
} analytical_data;

class Perceptron {
 public:
  /**
   *
   * @param path путь к файлу с обучающим материалом
   * @param epochs количество эпох обучения по умолчанию одна
   */
  virtual void learn(const std::string &path, int8_t epochs = 1) = 0;

  /**
   *
   * @param path путь к файлу с тестовым материалом
   * @param part_of_tests значение в диапазоне от 0.0 до 1.0. какую часть тестов прогонять 0.0 нулевая выборка, 1.0 - полная выборка.
   * @return структура с аналитическими данными analytical_data описано отдельно в хедере
   */
  virtual analytical_data test(const std::string &path, double part_of_tests = 1.0) = 0;

  /**
   *
   * @param layer_main закидывает NeuronMatrix
   * @return возвращает номер предсказанной буквы от 1 до 26 !!! (не от 0 до 25)
   */
  virtual int predict(const NeuronMatrix &layer_main) = 0;

  /**
   *
   * @param path путь куда экспортировать(откуда импортировать) данные
   * образец для хранения данных таков такой:
   * 2 - число слоев. Первая строка только количество внутренних слоев одной цифрой
   * 768 128 - дальше пошли матрицы, сначала размерность
   * d a t a .... - дальше сами данные в одну строчку с этого слоя(!) все данные через пробел
   * 128 26 - размер матрицы второго скрытого слоя
   * d a t a .... - все данные весов второго слоя
   * таких пар строк столько сколько слоев. необходимо приспособить экспорт/импорт под такую структуру
   */
  virtual void export_data(const std::string &path) = 0;
  virtual void import_data(const std::string &path) = 0;
  virtual ~Perceptron() {
  };
};