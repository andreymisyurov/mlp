#ifndef SRC_PARSER_PARSER_H_
#define SRC_PARSER_PARSER_H_

#include <fstream>
#include <vector>
#include <iostream>
#include <mutex>

#include "neuron_matrix.h"
#include "thread_pool.h"

// константа для нормализации значений матрицы
inline constexpr double kNORMA = 255.;

class Parser {
  /** Клас Parser реализован с использованием многопоточности.\n
   * Методы данного класса статические, могут парсить данные из файла для входного слоя нейронной сети, также импортировать
   * и экспортировать матрицы весов
   */
 public:
  /** СТАТИЧЕСКИЙ МЕТОД parseDataBase. Парсит базу данных из файла.\n
   * Первое значение - это ожидаемый правильный ответ, далее через запятую и пробел 784(матрица 28х28)\n
   * значения от 0 до 256, что соответствует градации серых оттенков.\n
   * @param in_path абсолютный путь к файлу базы данных
   * @param in_data_part Принимает значение от 0.0 до 1.0. Этот аргумент указывает функции
   * какую часть выборки необходимо спарсить, Где 0 - это пустая выборка, 1 - вся база целиком
   * @return возвращает упорядоченный данные - вектор из пары,\n
   * где first - ожидаемое значение,\n
   * а second - матрица входного слоя прошедшая нормализацию(с вещественными значениями от 0 до 1)
   */
  static auto parseDataBase(const std::string &in_path, double in_data_part) -> std::vector<std::pair<int, NeuronMatrix>>;

  /** СТАТИЧЕСКИЙ МЕТОД exportDataBase. Выгружает данные из матрицы весов по определенному пути на локальный диск.\n
   * Если файла не существует - создаст. Если существует - перезапишет.
   *
   * @param in_path абсолютный путь для сохранения данных.
   * @param in_weight вектор матриц со значениями весов.
   */
  static auto export_data(const std::string &in_path, const std::vector<NeuronMatrix> *in_weight) -> void;

  /** СТАТИЧЕСКИЙ МЕТОД exportDataBase. Загружает данные из файла на жестком диске в матрицу весов.
   *
   * @param in_path абсолютный путь к файлу с базой данных
   * @param inout_weight результирующий аргумент, указатель на матрицу весов
   */
  static auto import_data(const std::string &in_path, std::vector<NeuronMatrix> *inout_weight) -> void;
  // функция исключительно для тестов, утилизировать
  static NeuronMatrix parse_one_mtrx(const std::string &in_path);

 private:
  /** СТАТИЧЕСКИЙ МЕТОД parseOneLine. Приватная функция для реализации многопоточности.
   * Парсит только входящую строку. Обратите внимание что строка заходит по не константной ссылке и это не ошибка.
   * Функция перетирает входящую строчку.
   *
   * @param in_line входящая строка для парсинга. Заходит по не константной ссылке ссылке, обрезается внутри функции.
   * @param inout_result результирующий аргумент. Указатель на вектор
   * состоящий из пар(правильный ответ, матрица). Для работы с ним используется мьютекс
   * @param in_mtx указатель на мьютекс для работы с общедоступным указателем на вектор в потоках
   */
  static auto parseOneLine(std::string &in_line, std::vector<std::pair<int, NeuronMatrix>> *inout_result, std::mutex *in_mtx) -> void;
  ~Parser();
};

#endif //SRC_PARSER_PARSER_H_