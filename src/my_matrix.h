#ifndef MY_MATRIX_H_
#define MY_MATRIX_H_

#include <cmath>
#include <exception>
#include <typeinfo>
#include <iostream>

namespace victoriv {

inline constexpr auto EPS = 1e-6;

class MyException : std::exception {
 public:
  explicit MyException(std::string &&ex_text) noexcept : m_text(std::move(ex_text)) {}
  explicit MyException(const std::string &ex_text) noexcept : m_text(ex_text) {}
  ~MyException() noexcept = default;
  const char *what() const noexcept override { return m_text.c_str(); }

 private:
  std::string m_text;
};

template <typename T>
class Matrix {
 public:
  auto getColum() -> int { return m_column; };
  auto getRow() -> int { return m_row; };
  auto setRow(int x) -> void;
  auto setColum(int x) -> void;

 public:
  Matrix() = delete;
  explicit Matrix(int value);
  Matrix(int value_row, int value_column);
  Matrix(Matrix<T> &&other);
  Matrix(const Matrix &other);
  ~Matrix() { remove(); }

 public:
  auto eqMatrix(const Matrix<T> &other) const -> bool;
  auto sumMatrix(const Matrix<T> &other) -> void;
  auto subMatrix(const Matrix<T> &other) -> void;
  auto mulNumber(T num) -> void;
  auto mulMatrix(const Matrix<T> &other, bool sigmoid = false, bool multithread = false) -> void;
  auto determinant() -> double;
  auto transpose() -> Matrix<T>;
  auto calcComplements() -> Matrix<T>;
  auto inverseMatrix() -> Matrix<T>;

 public:
  auto operator=(const Matrix<T> &other) -> Matrix<T> &;
  auto operator=(Matrix<T> &&other) -> Matrix<T> &;
  auto operator+(const Matrix<T> &other) -> Matrix<T>;
  auto operator-(const Matrix<T> &other) -> Matrix<T>;
  auto operator+=(const Matrix<T> &other) -> Matrix<T>;
  auto operator-=(const Matrix<T> &other) -> Matrix<T>;
  auto operator*=(T &&value) -> Matrix<T>;
  auto operator*=(const T &value) -> Matrix<T>;
  auto operator()(int i, int j) -> T &;
  auto operator*(const Matrix<T> &other) -> Matrix<T>;
  auto operator*(T &&value) -> Matrix<T>;
  auto operator*(const T &value) -> Matrix<T>;
  auto operator==(const Matrix<T> &other) const -> bool;
  auto friend operator*(const T &value, const Matrix<T> &other) -> Matrix<T> {
    return Matrix<T>(other.m_row, other.m_column) * value;
  }
  auto friend operator*(T &&value, const Matrix &other) -> Matrix {
    T temp = value;
    return temp * other;
  }

 private:
  auto static copyMatrix(int row, int m_column, const Matrix &left,
                         const Matrix &right) -> void;
  auto create() -> void;
  auto remove() -> void;
  auto checkData(const Matrix<T> &other) -> void;
  auto checkForMult(const Matrix<T> &other) -> void;
  auto checkSquare() -> void;
  auto checkSetter(int value) -> void;
  auto checkConstructor(int value_row, int value_column) -> void;
  auto getMinor(int i, int j) -> Matrix<T>;

 private:
  int m_column = 0;
  int m_row = 0;
  T **m_matrix = nullptr;
};

template <typename T>
std::ostream& operator << (std::ostream &os, Matrix<T> &mtx) {
  std::string line = "";
  for(int i = 0; i < mtx.getRow(); ++i) {
    for(int j = 0; j < mtx.getColum(); ++j) {
      line += std::to_string(mtx(i, j));
      line += " ";
    }
    line += "\n";
  }
  return os << line;
}

template <typename T>
std::ostream& operator << (std::ostream &os, Matrix<T> &&mtx) {
  auto temp = mtx;
  return os << temp;;
}

template <typename T>
Matrix<T>::Matrix(int value) : Matrix(value, value) {}

template <typename T>
Matrix<T>::Matrix(int value_row, int value_column) {
  checkConstructor(value_column, value_row);

  m_row = value_row;
  m_column = value_column;
  create();
}

template <typename T>
Matrix<T>::Matrix(Matrix<T> &&other) {
  *this = other;
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T> &other) {
  *this = other;
}

template <typename T>
void Matrix<T>::setRow(int new_row) {
  checkSetter(new_row);
  if (m_row == new_row) return;
  Matrix<T> temp(new_row, m_column);
  int real_row = (new_row < m_row) ? new_row : m_row;
  copyMatrix(real_row, m_column, temp, *this);

  *this = temp;
}

template <typename T>
void Matrix<T>::checkSetter(int value) {
  if (value < 1) throw MyException("Bad value for row or colum.");
}

template <typename T>
void Matrix<T>::setColum(int new_column) {
  checkSetter(new_column);
  if (m_row == new_column) return;
  Matrix temp(m_row, new_column);
  int real_column = (new_column < m_column) ? new_column : m_column;
  copyMatrix(m_row, real_column, temp, *this);

  *this = temp;
}

template <typename T>
Matrix<T> Matrix<T>::inverseMatrix() {
  double determ = determinant();
  if (determ == 0) {
    throw MyException("Matrix's determinant is zero");
  }
  Matrix<T> temp = calcComplements();
  Matrix<T> result = temp.transpose();
  result.mulNumber(1. / determ);
  return result;
}

template <typename T>
Matrix<T> Matrix<T>::calcComplements() {
  checkSquare();
  Matrix<T> result(m_row, m_column);
  for (int i = 0; i < result.m_row; ++i) {
    for (int j = 0; j < result.m_column; ++j) {
      Matrix<T> temp = getMinor(i, j);
      double sign = (i + j) & 1 ? -1 : 1;
      result.m_matrix[i][j] = sign * temp.determinant();
    }
  }
  return result;
}

template <typename T>
double Matrix<T>::determinant() {
  checkSquare();
  double result = 0.0;
  if (m_row == 1) {
    result = m_matrix[0][0];
  } else if (m_row == 2) {
    result = m_matrix[0][0] * m_matrix[1][1] - m_matrix[0][1] * m_matrix[1][0];
  } else {
    int j = 0;
    for (int i = 0; i < m_column; ++i) {
      Matrix<T> temp = getMinor(i, j);
      double sign = (i + j) & 1 ? -1.0 : 1.0;
      result += sign * m_matrix[i][j] * temp.determinant();
    }
  }
  return result;
}

template <typename T>
bool Matrix<T>::eqMatrix(const Matrix<T> &other) const {
  for (int i = 0; i < m_row; ++i) {
    for (int j = 0; j < m_column; ++j) {
      if (fabs(m_matrix[i][j] - other.m_matrix[i][j]) > EPS) return false;
    }
  }
  return true;
}

template <typename T>
void Matrix<T>::sumMatrix(const Matrix<T> &other) {
  checkData(other);
  for (int i = 0; i < m_row; ++i) {
    for (int j = 0; j < m_column; ++j) {
      m_matrix[i][j] += other.m_matrix[i][j];
    }
  }
}

template <typename T>
void Matrix<T>::subMatrix(const Matrix<T> &other) {
  checkData(other);
  Matrix<T> result(other);
  result *= static_cast<T>(-1.);
  *this += result;
}

template <typename T>
void Matrix<T>::mulNumber(T num) {
  for (int i = 0; i < m_row; ++i) {
    for (int j = 0; j < m_column; ++j) {
      m_matrix[i][j] *= num;
    }
  }
}

template <typename T>
void Matrix<T>::mulMatrix(const Matrix<T> &other, bool sigmoid, bool multithread) {
  checkForMult(other);
  Matrix<T> result(m_row, other.m_column);
  for (int i = 0; i < result.m_row; ++i) {
    for (int j = 0; j < result.m_column; ++j) {
      result.m_matrix[i][j] = 0;
      for (int k = 0; k < m_column; ++k) {
        result.m_matrix[i][j] += m_matrix[i][k] * other.m_matrix[k][j];
      }
      if (sigmoid) {
        if ((typeid(T) == typeid(double)) || (typeid(T) == typeid(float))) {
          result.m_matrix[i][j] = 1./(1 + exp(-result.m_matrix[i][j]));
        } else {
          throw MyException("Incorrect typedef for find sigmoid");
        }
      }
    }
  }

  std::swap(*this, result);
}

template <typename T>
Matrix<T> Matrix<T>::transpose() {
  Matrix result(m_column, m_row);
  for (int i = 0; i < result.m_row; ++i) {
    for (int j = 0; j < result.m_column; ++j) {
      result.m_matrix[i][j] = m_matrix[j][i];
    }
  }
  return result;
}

template <typename T>
bool Matrix<T>::operator==(const Matrix<T> &other) const {
  return eqMatrix(other);
}

template <typename T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T> &other) {
  Matrix temp(other.m_row, other.m_column);
  copyMatrix(other.m_row, other.m_column, temp, other);

  std::swap(m_row, temp.m_row);
  std::swap(m_column, temp.m_column);
  std::swap(m_matrix, temp.m_matrix);
  return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator=(Matrix<T> &&other) {
  if (this != &other) {
    std::swap(m_column, other.m_column);
    std::swap(m_row, other.m_row);
    std::swap(m_matrix, other.m_matrix);
  }
  return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> &other) {
  Matrix temp(*this);
  temp.subMatrix(other);
  return temp;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &other) {
  Matrix temp(*this);
  temp.sumMatrix(other);
  return temp;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &other) {
  Matrix<T> temp(*this);
  temp.mulMatrix(other);
  return temp;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const T &value) {
  Matrix<T> temp(*this);
  temp.mulNumber(value);
  return temp;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(T &&value) {
  Matrix<T> temp(*this);
  auto num = value;
  return temp * num;
}

template <typename T>
Matrix<T> Matrix<T>::operator+=(const Matrix<T> &other) {
  sumMatrix(other);
  return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator-=(const Matrix<T> &other) {
  subMatrix(other);
  return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator*=(T &&value) {
  auto temp = value;
  *this *= temp;
  return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator*=(const T &value) {
  mulNumber(value);
  return *this;
}

template <typename T>
T &Matrix<T>::operator()(int i, int j) {
  if (i < 0 || j < 0 || i >= m_row || j >= m_column) {
    throw MyException("Nothing to get");
  }
  return m_matrix[i][j];
}

template <typename T>
void Matrix<T>::remove() {
  for (int i = 0; i < m_row; ++i) {
    delete[] m_matrix[i];
  }
  delete[] m_matrix;
}

template <typename T>
void Matrix<T>::create() {
  m_matrix = new T *[m_row];
  for (int i = 0; i < m_row; ++i) {
    m_matrix[i] = new T[m_column];
    std::memset(m_matrix[i], 0, sizeof(T) * m_column);
  }
}

template <typename T>
Matrix<T> Matrix<T>::getMinor(int i, int j) {
  Matrix<T> minor(m_row - 1, m_column - 1);
  int countI = 0;
  int countJ = 0;
  for (int l = 0; l < m_row; ++l) {
    if (l == i) {
      continue;
    }
    for (int k = 0; k < m_column; ++k) {
      if (k == j) continue;
      minor.m_matrix[countI][countJ] = m_matrix[l][k];
      countJ = (countJ + 1) % minor.m_column;
    }
    countI = (countI + 1) % minor.m_row;
  }
  return minor;
}

template <typename T>
void Matrix<T>::copyMatrix(int row, int colum, const Matrix<T> &left,
                           const Matrix<T> &right) {
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < colum; ++j) {
      left.m_matrix[i][j] = right.m_matrix[i][j];
    }
  }
}

template <typename T>
void Matrix<T>::checkData(const Matrix<T> &other) {
  if (m_row != other.m_row || m_column != other.m_column)
    throw MyException("Matrix have different rows or columns");
}

template <typename T>
void Matrix<T>::checkForMult(const Matrix<T> &other) {
  if (m_column != other.m_row) throw MyException("Not equals rows and column");
}

template <typename T>
void Matrix<T>::checkSquare() {
  if (m_column != m_row) throw MyException("Matrix isn't square");
}

template <typename T>
void Matrix<T>::checkConstructor(int value_row, int value_column) {
  if (value_column < 1 || value_row < 1)
    throw MyException("Bad value for row or colum.");
}

}  // namespace victoriv

#endif  // MY_MATRIX_H_
