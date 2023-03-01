#include <gtest/gtest.h>
#include <exception>

#include "neuron_matrix.h"
#include "perzeptron_mtx.h"
#include "parser.h"

// добавить в ресурсы
std::string k_one_matrix = "/Users/gonzo/program/school21/general/CPP7_MLP-0/src/Neuro/test_cases/one_al.txt";
std::string k_export = "/Users/gonzo/program/school21/general/CPP7_MLP-0/src/Neuro/test_cases/export.txt";

TEST(perzeptron_mtx, ex_1) {
  PerzeptronMtx temp;
  NeuronMatrix mtrx(1, 784);
   temp.learn(k_one_matrix, 2);
   temp.test(k_one_matrix, 0.2);
   temp.predict(mtrx);
  temp.exportDataBase(k_export);
  temp.importDataBase(k_export);
   system(("rm " + k_export).c_str());
}

TEST(parser, ex_1) {
  std::vector<NeuronMatrix> weight;
  std::vector<std::pair<int, NeuronMatrix>> temp = Parser::parseDataBase(k_one_matrix, 1.0);
  weight.push_back(temp[0].second);
  Parser::export_data(k_export, &weight);
  Parser::import_data(k_export, &weight);
  GTEST_ASSERT_EQ(temp[0].second == weight[0], 1);
  system(("rm " + k_export).c_str());
}

TEST(neuro_const_move, ex_1) {
  NeuronMatrix ex_1(2, 2);
  NeuronMatrix ex_2(std::move(ex_1));
  NeuronMatrix ex_5(6);
  ex_5 = std::move(ex_2);
}

TEST(neuro_const_copy, ex_2) {
  NeuronMatrix ex_1(2, 2);
  NeuronMatrix ex_2(ex_1);
  NeuronMatrix ex_4(4);
  ex_4 = ex_2;
}


TEST(neuro_const, ex_1) {
  NeuronMatrix ex(2);
  ex(0, 0) = 1.;
  ex(0, 1) = 2.;
  ex(1, 0) = 3.;
  ex(1, 1) = 4.;
  Matrix copy = ex;
  for (int i = 0; i < ex.getRow(); ++i) {
    for (int j = 0; j < ex.getColum(); ++j) {
      GTEST_ASSERT_EQ(copy(i, j), ex(i, j));
    }
  }
}

TEST(copy_constr, example_1) {
  Matrix<double> ex(2);
  ex(0, 0) = 1;
  ex(0, 1) = 2;
  ex(1, 0) = 3;
  ex(1, 1) = 4;
  Matrix copy = ex;
  for (int i = 0; i < ex.getRow(); ++i) {
    for (int j = 0; j < ex.getColum(); ++j) {
      GTEST_ASSERT_EQ(copy(i, j), ex(i, j));
    }
  }
}

TEST(determinant, normal_1_1) {
  Matrix<short> ex{1};
  ex(0, 0) = 100;
  GTEST_ASSERT_EQ(ex.determinant(), 100.);
}

TEST(determinant, normal_2_2) {
  Matrix<float> ex{2};
  ex(0, 0) = 1.;
  ex(0, 1) = 2.;
  ex(1, 0) = 3.;
  ex(1, 1) = 4.;
  GTEST_ASSERT_EQ(ex.determinant(), -2.0);
}

TEST(determinant, normal1_3_3) {
  Matrix<int> ex{3};
  ex(0, 0) = 1;
  ex(0, 1) = 2;
  ex(0, 2) = 3;
  ex(1, 0) = 4;
  ex(1, 1) = 5;
  ex(1, 2) = 6;
  ex(2, 0) = 7;
  ex(2, 1) = 8;
  ex(2, 2) = 9;
  GTEST_ASSERT_EQ(ex.determinant(), 0.0);
}

TEST(determinant, normal_double_3_3) {
  Matrix<float> ex{3};
  ex(0, 0) = 1.1;
  ex(0, 1) = 2.2;
  ex(0, 2) = 3;
  ex(1, 0) = 4.4;
  ex(1, 1) = 5;
  ex(1, 2) = 6.6;
  ex(2, 0) = 7;
  ex(2, 1) = 8.8;
  ex(2, 2) = 9;
  EXPECT_NEAR(ex.determinant(), 11.292, 0.7);
}

TEST(determinant, normal2_3_3) {
  Matrix<char> ex{3};
  ex(0, 0) = 1;
  ex(0, 1) = 2;
  ex(0, 2) = 3;
  ex(1, 0) = 4;
  ex(1, 1) = 5;
  ex(1, 2) = 6;
  ex(2, 0) = 7;
  ex(2, 1) = 8;
  ex(2, 2) = 9;
  GTEST_ASSERT_EQ(ex.determinant(), 0.0);
}

TEST(determinant, normal_4_4) {
  Matrix<short> ex{4};
  ex(0, 0) = 4;
  ex(0, 1) = 5;
  ex(0, 2) = -1;
  ex(0, 3) = 8;
  ex(1, 0) = 1;
  ex(1, 1) = -5;
  ex(1, 2) = 9;
  ex(1, 3) = 3;
  ex(2, 0) = -5;
  ex(2, 1) = 1;
  ex(2, 2) = 9;
  ex(2, 3) = 2;
  ex(3, 0) = 7;
  ex(3, 1) = 8;
  ex(3, 2) = 2;
  ex(3, 3) = 1;
  GTEST_ASSERT_EQ(ex.determinant(), 6930.0);
}

TEST(equals, true_1) {
  Matrix<int> first{1};
  first(0, 0) = 100;
  Matrix<int> second{1};
  second(0, 0) = 100;
  bool result = first.eqMatrix(second);
  GTEST_ASSERT_EQ(result, 1);
}

TEST(equals, true_2) {
  Matrix<float> first{2};
  first(0, 0) = 1.11;
  first(0, 1) = 2.22;
  first(1, 0) = 3.33;
  first(1, 1) = 4.44;
  Matrix<float> second{2};
  second(0, 0) = 1.11;
  second(0, 1) = 2.22;
  second(1, 0) = 3.33;
  second(1, 1) = 4.44;
  bool result = first.eqMatrix(second);
  GTEST_ASSERT_EQ(result, 1);
}

TEST(equals, true_3) {
  Matrix<char> first(3);
  first(0, 0) = 1;
  first(0, 1) = 2;
  first(0, 2) = 3;
  first(1, 0) = 4;
  first(1, 1) = 5;
  first(1, 2) = 6;
  first(2, 0) = 7;
  first(2, 1) = 8;
  first(2, 2) = 9;
  Matrix<char> second(3);
  second(0, 0) = 1;
  second(0, 1) = 2;
  second(0, 2) = 3;
  second(1, 0) = 4;
  second(1, 1) = 5;
  second(1, 2) = 6;
  second(2, 0) = 7;
  second(2, 1) = 8;
  second(2, 2) = 9;
  bool result = first.eqMatrix(second);
  GTEST_ASSERT_EQ(result, 1);
}

TEST(equals, true_4) {
  Matrix<double> first(3);
  first(0, 0) = 0.0000009;
  first(0, 1) = 0;
  first(0, 2) = 0;
  first(1, 0) = 1e-7;
  first(1, 1) = 0;
  first(1, 2) = 0;
  first(2, 0) = 0;
  first(2, 1) = 0;
  first(2, 2) = 0;
  Matrix<double> second(3);
  second(0, 0) = 0;
  second(0, 1) = 0;
  second(0, 2) = 0;
  second(1, 0) = -0.0;
  second(1, 1) = 0;
  second(1, 2) = 0;
  second(2, 0) = -1e-7;
  second(2, 1) = 0.000000000012315;
  second(2, 2) = 0;
  bool result = first.eqMatrix(second);
  GTEST_ASSERT_EQ(result, 1);
}

TEST(equals, false_1) {
  Matrix<float> first(1);
  first(0, 0) = 100.00001;
  Matrix<float> second(1);
  second(0, 0) = 100.;
  bool result = first.eqMatrix(second);
  GTEST_ASSERT_EQ(result, 0);
}

TEST(equals, false_2) {
  Matrix<double> first(2);
  first(0, 0) = 1.11;
  first(0, 1) = 1e-6;
  first(1, 0) = 3.33;
  first(1, 1) = 4.44;
  Matrix<double> second(2);
  second(0, 0) = 1.11;
  second(0, 1) = -1e-6;
  second(1, 0) = 3.33;
  second(1, 1) = 4.44;
  bool result = first.eqMatrix(second);
  GTEST_ASSERT_EQ(result, 0);
}

TEST(equals, false_3) {
  Matrix<float> first(2);
  first(0, 0) = 1.11;
  first(0, 1) = 1;
  first(1, 0) = 3.33;
  first(1, 1) = 4.44;
  Matrix<float> second(2);
  second(0, 0) = 1.11;
  second(0, 1) = -1;
  second(1, 0) = 3.33;
  second(1, 1) = 4.44;
  bool result = first.eqMatrix(second);
  GTEST_ASSERT_EQ(result, 0);
}

TEST(equals, false_4) {
  Matrix<float> first(2, 3);
  first(0, 0) = 1.11;
  first(0, 1) = 1;
  first(0, 2) = 1.5;
  first(1, 0) = 3.33;
  first(1, 1) = 4.44;
  first(1, 2) = 5;
  Matrix<float> second(2);
  second(0, 0) = 1.11;
  second(0, 1) = 1;
  second(1, 0) = 3.33;
  second(1, 1) = 4.44;
  bool result = first.eqMatrix(second);
  GTEST_ASSERT_EQ(result, 0);
}

TEST(transpose, example_1) {
  Matrix<int> first(2, 3);
  first(0, 0) = 1;
  first(0, 1) = 2;
  first(0, 2) = 3;
  first(1, 0) = 4;
  first(1, 1) = 5;
  first(1, 2) = 6;
  Matrix<int> second(3, 2);
  second(0, 0) = 1;
  second(0, 1) = 4;
  second(1, 0) = 2;
  second(1, 1) = 5;
  second(2, 0) = 3;
  second(2, 1) = 6;
  Matrix ex = first.transpose();
  for (int i = 0; i < second.getRow(); i++) {
    for (int j = 0; j < second.getColum(); j++) {
      GTEST_ASSERT_EQ(ex(i, j), second(i, j));
    }
  }
}

TEST(transpose, example_2) {
  Matrix<short> first(2);
  first(0, 0) = 1;
  first(0, 1) = 2;
  first(1, 0) = 3;
  first(1, 1) = 4;
  Matrix<short> second(2);
  second(0, 0) = 1;
  second(0, 1) = 3;
  second(1, 0) = 2;
  second(1, 1) = 4;
  Matrix<short> ex = first.transpose();
  for (int i = 0; i < second.getRow(); i++) {
    for (int j = 0; j < second.getColum(); j++) {
      GTEST_ASSERT_EQ(ex(i, j), second(i, j));
    }
  }
}

TEST(transpose, example_3) {
  Matrix<char> first(1);
  first(0, 0) = 1;
  Matrix<char> second(1);
  second(0, 0) = 1;
  Matrix<char> ex = first.transpose();
  for (int i = 0; i < second.getRow(); i++) {
    for (int j = 0; j < second.getColum(); j++) {
      GTEST_ASSERT_EQ(ex(i, j), second(i, j));
    }
  }
}

TEST(sum, example_1) {
  Matrix<int> first(3);
  first(0, 0) = 1;
  first(0, 1) = 2;
  first(0, 2) = 3;
  first(1, 0) = 4;
  first(1, 1) = 5;
  first(1, 2) = 6;
  first(2, 0) = 7;
  first(2, 1) = 8;
  first(2, 2) = 9;
  Matrix<int> second(3);
  second(0, 0) = -1;
  second(0, 1) = -2;
  second(0, 2) = -3;
  second(1, 0) = -4;
  second(1, 1) = -5;
  second(1, 2) = -6;
  second(2, 0) = -7;
  second(2, 1) = -8;
  second(2, 2) = -9;
  Matrix<int> result(3);
  result(0, 0) = 0;
  result(0, 1) = 0;
  result(0, 2) = 0;
  result(1, 0) = 0;
  result(1, 1) = 0;
  result(1, 2) = 0;
  result(2, 0) = 0;
  result(2, 1) = 0;
  result(2, 2) = 0;
  first.sumMatrix(second);
  for (int i = 0; i < second.getRow(); i++) {
    for (int j = 0; j < second.getColum(); j++) {
      GTEST_ASSERT_EQ(first(i, j), result(i, j));
    }
  }
}

TEST(sum, example_2) {
  Matrix<float> first(2);
  first(0, 0) = 1.0000001;
  first(0, 1) = 2;
  first(1, 0) = 3;
  first(1, 1) = 4;
  Matrix<float> second(2);
  second(0, 0) = -1;
  second(0, 1) = -2;
  second(1, 0) = -3;
  second(1, 1) = -4.0000001;
  Matrix<float> result(2);
  result(0, 0) = 0;
  result(0, 1) = 0;
  result(1, 0) = 0;
  result(1, 1) = 0;
  first.sumMatrix(second);
  for (int i = 0; i < second.getRow(); i++) {
    for (int j = 0; j < second.getColum(); j++) {
      EXPECT_NEAR(first(i, j), result(i, j), EPS);
    }
  }
}

TEST(sub, example_1) {
  Matrix<short> first(3);
  first(0, 0) = 1;
  first(0, 1) = 2;
  first(0, 2) = 3;
  first(1, 0) = 4;
  first(1, 1) = 5;
  first(1, 2) = 6;
  first(2, 0) = 7;
  first(2, 1) = 8;
  first(2, 2) = 9;
  Matrix<short> second(3);
  second(0, 0) = 1;
  second(0, 1) = 2;
  second(0, 2) = 3;
  second(1, 0) = 4;
  second(1, 1) = 5;
  second(1, 2) = 6;
  second(2, 0) = 7;
  second(2, 1) = 8;
  second(2, 2) = 9;
  Matrix<short> result(3);
  result(0, 0) = 0;
  result(0, 1) = 0;
  result(0, 2) = 0;
  result(1, 0) = 0;
  result(1, 1) = 0;
  result(1, 2) = 0;
  result(2, 0) = 0;
  result(2, 1) = 0;
  result(2, 2) = 0;
  first.subMatrix(second);
  for (int i = 0; i < second.getRow(); i++) {
    for (int j = 0; j < second.getColum(); j++) {
      GTEST_ASSERT_EQ(first(i, j), result(i, j));
    }
  }
}

TEST(sub, example_2) {
  Matrix<float> first(2);
  first(0, 0) = 1.0000001;
  first(0, 1) = 2.;
  first(1, 0) = 3.;
  first(1, 1) = 4.;
  Matrix<float> second(2);
  second(0, 0) = 1.;
  second(0, 1) = 2.;
  second(1, 0) = 3.;
  second(1, 1) = 4.0000001;
  Matrix<float> result(2);
  result(0, 0) = 0.;
  result(0, 1) = 0.;
  result(1, 0) = 0.;
  result(1, 1) = 0.;
  first.subMatrix(second);
  for (int i = 0; i < second.getRow(); i++) {
    for (int j = 0; j < second.getColum(); j++) {
      EXPECT_NEAR(first(i, j), result(i, j), EPS);
    }
  }
}

TEST(mulNumber, example_1) {
  Matrix<double> first(2);
  first(0, 0) = 1.0000001;
  first(0, 1) = 2.;
  first(1, 0) = 3.;
  first(1, 1) = 4.;
  Matrix<double> result(2);
  result(0, 0) = 1.0000001 * 5;
  result(0, 1) = 2. * 5.;
  result(1, 0) = 3. * 5.;
  result(1, 1) = 4. * 5.;
  first.mulNumber(5.0);
  for (int i = 0; i < first.getRow(); i++) {
    for (int j = 0; j < first.getColum(); j++) {
      EXPECT_NEAR(first(i, j), result(i, j), EPS);
    }
  }
}

TEST(mulNumber, example_2) {
  Matrix<float> first(2);
  first(0, 0) = 1.0000001;
  first(0, 1) = 2.;
  first(1, 0) = 3.;
  first(1, 1) = 4.;
  Matrix<float> result(2);
  result(0, 0) = 1.0000001 * 0.;
  result(0, 1) = 2 * 0.;
  result(1, 0) = 3 * 0.;
  result(1, 1) = 4 * 0.;
  first.mulNumber(0.000000001);
  for (int i = 0; i < first.getRow(); ++i) {
    for (int j = 0; j < first.getColum(); ++j) {
      EXPECT_NEAR(first(i, j), result(i, j), EPS);
    }
  }
}

TEST(mulNumber, example_3) {
  Matrix<float> first(2);
  first(0, 0) = 1.0000001;
  first(0, 1) = 2.;
  first(1, 0) = 3.;
  first(1, 1) = 4.;
  Matrix<float> result(2);
  result(0, 0) = 1.0000001;
  result(0, 1) = 2.;
  result(1, 0) = 3.;
  result(1, 1) = 4.;
  first.mulNumber(1.0);
  for (int i = 0; i < first.getRow(); i++) {
    for (int j = 0; j < first.getColum(); j++) {
      EXPECT_NEAR(first(i, j), result(i, j), EPS);
    }
  }
}

TEST(mulNumber, example_4) {
  Matrix<double> first(2);
  first(0, 0) = 1.0000001;
  first(0, 1) = 2.;
  first(1, 0) = 3.;
  first(1, 1) = 4.;
  Matrix<double> result(2);
  result(0, 0) = -1.0000001;
  result(0, 1) = -2.;
  result(1, 0) = -3.;
  result(1, 1) = -4.;
  first.mulNumber(-1.0);
  for (int i = 0; i < first.getRow(); i++) {
    for (int j = 0; j < first.getColum(); j++) {
      EXPECT_NEAR(first(i, j), result(i, j), EPS);
    }
  }
}

TEST(mulMatrix, example_1) {
  Matrix<int> first(3, 2);
  first(0, 0) = 1;
  first(0, 1) = 4;
  first(1, 0) = 2;
  first(1, 1) = 5;
  first(2, 0) = 3;
  first(2, 1) = 6;
  Matrix<int> second(2, 3);
  second(0, 0) = 1;
  second(0, 1) = -1;
  second(0, 2) = 1;
  second(1, 0) = 2;
  second(1, 1) = 3;
  second(1, 2) = 4;
  Matrix<int> result(3);
  result(0, 0) = 9;
  result(0, 1) = 11;
  result(0, 2) = 17;
  result(1, 0) = 12;
  result(1, 1) = 13;
  result(1, 2) = 22;
  result(2, 0) = 15;
  result(2, 1) = 15;
  result(2, 2) = 27;
  first.mulMatrix(second);
  for (int i = 0; i < result.getRow(); i++) {
    for (int j = 0; j < result.getColum(); j++) {
      EXPECT_NEAR(first(i, j), result(i, j), EPS);
    }
  }
}

TEST(mulMatrix, example_2) {
  Matrix<float> first(2);
  first(0, 0) = 1.5;
  first(0, 1) = 1.5;
  first(1, 0) = 1.5;
  first(1, 1) = 1.5;
  Matrix<float> second(2);
  second(0, 0) = 1;
  second(0, 1) = 1;
  second(1, 0) = 1;
  second(1, 1) = 1;
  Matrix<float> result(2);
  result(0, 0) = 3;
  result(0, 1) = 3;
  result(1, 0) = 3;
  result(1, 1) = 3;
  first.mulMatrix(second);
  for (int i = 0; i < first.getRow(); i++) {
    for (int j = 0; j < first.getColum(); j++) {
      EXPECT_NEAR(first(i, j), result(i, j), EPS);
    }
  }
}

TEST(calcComplements, example_1) {
  Matrix<short> first(3);
  first(0, 0) = 1;
  first(0, 1) = 2;
  first(0, 2) = 3;
  first(1, 0) = 0;
  first(1, 1) = 4;
  first(1, 2) = 2;
  first(2, 0) = 5;
  first(2, 1) = 2;
  first(2, 2) = 1;
  Matrix<short> second = first.calcComplements();
  Matrix<short> result(3);
  result(0, 0) = 0;
  result(0, 1) = 10;
  result(0, 2) = -20;
  result(1, 0) = 4;
  result(1, 1) = -14;
  result(1, 2) = 8;
  result(2, 0) = -8;
  result(2, 1) = -2;
  result(2, 2) = 4;
  for (int i = 0; i < first.getRow(); i++) {
    for (int j = 0; j < first.getColum(); j++) {
      EXPECT_NEAR(second(i, j), result(i, j), EPS);
    }
  }
}

TEST(calcComplements, example_2) {
  Matrix<char> first(3);
  first(0, 0) = 2;
  first(0, 1) = 5;
  first(0, 2) = 7;
  first(1, 0) = 6;
  first(1, 1) = 3;
  first(1, 2) = 4;
  first(2, 0) = 5;
  first(2, 1) = -2;
  first(2, 2) = -3;
  Matrix<char> second = first.calcComplements();
  Matrix<char> result(3);
  result(0, 0) = -1;
  result(0, 1) = 38;
  result(0, 2) = -27;
  result(1, 0) = 1;
  result(1, 1) = -41;
  result(1, 2) = 29;
  result(2, 0) = -1;
  result(2, 1) = 34;
  result(2, 2) = -24;
  for (int i = 0; i < first.getRow(); i++) {
    for (int j = 0; j < first.getColum(); j++) {
      EXPECT_NEAR(second(i, j), result(i, j), EPS);
    }
  }
}

TEST(inverseMatrix, example_1) {
  Matrix<float> first(2);
  first(0, 0) = -1.5;
  first(0, 1) = -1.5;
  first(1, 0) = -0.5;
  first(1, 1) = -1.5;
  Matrix<float> result(2);
  result(0, 0) = -1;
  result(0, 1) = 1;
  result(1, 0) = 1. / 3;
  result(1, 1) = -1;
  Matrix<float> second = first.inverseMatrix();
  for (int i = 0; i < first.getRow(); i++) {
    for (int j = 0; j < first.getColum(); j++) {
      EXPECT_NEAR(second(i, j), result(i, j), EPS);
    }
  }
}

TEST(inverseMatrix, example_2) {
  Matrix<int> first(3);
  first(0, 0) = 2;
  first(0, 1) = 5;
  first(0, 2) = 7;
  first(1, 0) = 6;
  first(1, 1) = 3;
  first(1, 2) = 4;
  first(2, 0) = 5;
  first(2, 1) = -2;
  first(2, 2) = -3;
  Matrix<int> second = first.inverseMatrix();
  Matrix<int> result(3);
  result(0, 0) = 1;
  result(0, 1) = -1;
  result(0, 2) = 1;
  result(1, 0) = -38;
  result(1, 1) = 41;
  result(1, 2) = -34;
  result(2, 0) = 27;
  result(2, 1) = -29;
  result(2, 2) = 24;
  for (int i = 0; i < first.getRow(); i++) {
    for (int j = 0; j < first.getColum(); j++) {
      EXPECT_NEAR(second(i, j), result(i, j), EPS);
    }
  }
}

TEST(operator_eq, example_1) {
  Matrix<short> first(3);
  first(0, 0) = 2;
  first(0, 1) = 5;
  first(0, 2) = 7;
  first(1, 0) = 6;
  first(1, 1) = 3;
  first(1, 2) = 4;
  first(2, 0) = 5;
  first(2, 1) = -2;
  first(2, 2) = -3;
  Matrix<short> result(3);
  result(0, 0) = 1;
  result(0, 1) = -1;
  result(0, 2) = 1;
  result(1, 0) = -38;
  result(1, 1) = 41;
  result(1, 2) = -34;
  result(2, 0) = 27;
  result(2, 1) = -29;
  result(2, 2) = 24;
  int check = (first == result);
  GTEST_ASSERT_EQ(check, 0);
}

TEST(operator_eq, example_2) {
  Matrix<char> first(3);
  first(0, 0) = 2;
  first(0, 1) = 5;
  first(0, 2) = 7;
  first(1, 0) = 6;
  first(1, 1) = 3;
  first(1, 2) = 4;
  first(2, 0) = 5;
  first(2, 1) = -2;
  first(2, 2) = -3;
  Matrix<char> result(3);
  result(0, 0) = 2;
  result(0, 1) = 5;
  result(0, 2) = 7;
  result(1, 0) = 6;
  result(1, 1) = 3;
  result(1, 2) = 4;
  result(2, 0) = 5;
  result(2, 1) = -2;
  result(2, 2) = -3;
  int check = (first == result);
  GTEST_ASSERT_EQ(check, 1);
}

TEST(operator_sum, example_1) {
  Matrix<bool> first(3);
  first(0, 0) = 2;
  first(0, 1) = 5;
  first(0, 2) = 7;
  first(1, 0) = 6;
  first(1, 1) = 3;
  first(1, 2) = 4;
  first(2, 0) = 5;
  first(2, 1) = 2;
  first(2, 2) = 3;
  Matrix<bool> second(3);
  second(0, 0) = 2;
  second(0, 1) = 5;
  second(0, 2) = 7;
  second(1, 0) = 6;
  second(1, 1) = 3;
  second(1, 2) = 4;
  second(2, 0) = 5;
  second(2, 1) = 2;
  second(2, 2) = 3;
  Matrix<bool> result(3);
  result(0, 0) = 2 * 2;
  result(0, 1) = 5 * 2;
  result(0, 2) = 7 * 2;
  result(1, 0) = 6 * 2;
  result(1, 1) = 3 * 2;
  result(1, 2) = 4 * 2;
  result(2, 0) = 5 * 2;
  result(2, 1) = 2 * 2;
  result(2, 2) = 3 * 2;
  Matrix<bool> summa = (first + second);
  for (int i = 0; i < second.getRow(); i++) {
    for (int j = 0; j < second.getColum(); j++) {
      GTEST_ASSERT_EQ(summa(i, j), result(i, j));
    }
  }
}

TEST(operator_sum, example_2) {
  Matrix<int> first{3};
  first(0, 0) = 2;
  first(0, 1) = 5;
  first(0, 2) = 7;
  first(1, 0) = 6;
  first(1, 1) = 3;
  first(1, 2) = 4;
  first(2, 0) = 5;
  first(2, 1) = 2;
  first(2, 2) = 3;
  Matrix<int> second{3};
  second(0, 0) = -2;
  second(0, 1) = -5;
  second(0, 2) = -7;
  second(1, 0) = -6;
  second(1, 1) = -3;
  second(1, 2) = -4;
  second(2, 0) = -5;
  second(2, 1) = -2;
  second(2, 2) = -3;
  Matrix<int> result{3};
  result(0, 0) = 0;
  result(0, 1) = 0;
  result(0, 2) = 0;
  result(1, 0) = 0;
  result(1, 1) = 0;
  result(1, 2) = 0;
  result(2, 0) = 0;
  result(2, 1) = 0;
  result(2, 2) = 0;
  Matrix summa = first + second;
  for (int i = 0; i < second.getRow(); i++) {
    for (int j = 0; j < second.getColum(); j++) {
      GTEST_ASSERT_EQ(summa(i, j), result(i, j));
    }
  }
}

TEST(operator_sub, example_1) {
  Matrix<short> first{3};
  first(0, 0) = 2;
  first(0, 1) = 5;
  first(0, 2) = 7;
  first(1, 0) = 6;
  first(1, 1) = 3;
  first(1, 2) = 4;
  first(2, 0) = 5;
  first(2, 1) = 2;
  first(2, 2) = 3;
  Matrix<short> second{3};
  second(0, 0) = 2;
  second(0, 1) = 5;
  second(0, 2) = 7;
  second(1, 0) = 6;
  second(1, 1) = 3;
  second(1, 2) = 4;
  second(2, 0) = 5;
  second(2, 1) = 2;
  second(2, 2) = 3;
  Matrix<short> result{3};
  result(0, 0) = 0;
  result(0, 1) = 0;
  result(0, 2) = 0;
  result(1, 0) = 0;
  result(1, 1) = 0;
  result(1, 2) = 0;
  result(2, 0) = 0;
  result(2, 1) = 0;
  result(2, 2) = 0;
  Matrix<short> summa = first - second;
  for (int i = 0; i < second.getRow(); ++i) {
    for (int j = 0; j < second.getColum(); ++j) {
      GTEST_ASSERT_EQ(summa(i, j), result(i, j));
    }
  }
}

TEST(operator_brackets, example_1) {
  Matrix<short> first(1);
  first(0, 0) = 15;
  Matrix<short> result(1);
  result(0, 0) = 15;
  GTEST_ASSERT_EQ(first(0, 0), result(0, 0));
}

TEST(operator_brackets, example_2) {
  Matrix<int> first(2);
  first(0, 0) = 1;
  first(0, 1) = 2;
  first(1, 0) = 3;
  first(1, 1) = 4;
  Matrix<int> result(2);
  result(0, 0) = 1;
  result(0, 1) = 2;
  result(1, 0) = 3;
  result(1, 1) = 4;
  for (int i = 0; i < result.getRow(); i++) {
    for (int j = 0; j < result.getColum(); j++) {
      GTEST_ASSERT_EQ(first(i, j), result(i, j));
    }
  }
}

TEST(operator_mult_number, example_1) {
  Matrix<float> first(2);
  first(0, 0) = 1.;
  first(0, 1) = 2.;
  first(1, 0) = 3.;
  first(1, 1) = 4.;
  Matrix<float> result(2);
  result(0, 0) = 1.;
  result(0, 1) = 2.;
  result(1, 0) = 3.;
  result(1, 1) = 4.;
  float n = 1.0;
  first = first * n;
  for (int i = 0; i < result.getRow(); ++i) {
    for (int j = 0; j < result.getColum(); ++j) {
      GTEST_ASSERT_EQ(first(i, j), result(i, j));
    }
  }
}

TEST(operator_mult_number, example_2) {
  Matrix<int> first(2);
  first(0, 0) = 1;
  first(0, 1) = 2;
  first(1, 0) = 3;
  first(1, 1) = 4;
  Matrix<int> result(2);
  result(0, 0) = 2;
  result(0, 1) = 4;
  result(1, 0) = 6;
  result(1, 1) = 8;
  int n = 2;
  first = first * n;
  for (int i = 0; i < result.getRow(); i++) {
    for (int j = 0; j < result.getColum(); j++) {
      GTEST_ASSERT_EQ(first(i, j), result(i, j));
    }
  }
}

TEST(operator_mult_number, example_3) {
  Matrix<int> first(2);
  first(0, 0) = 1;
  first(0, 1) = 2;
  first(1, 0) = 3;
  first(1, 1) = 4;
  Matrix<int> result(2);
  result(0, 0) = 0;
  result(0, 1) = 0;
  result(1, 0) = 0;
  result(1, 1) = 0;
  int n = 0;
  first = first * n;
  for (int i = 0; i < result.getRow(); i++) {
    for (int j = 0; j < result.getColum(); j++) {
      GTEST_ASSERT_EQ(first(i, j), result(i, j));
    }
  }
}

TEST(operator_mult_number, example_4) {
  Matrix<short> first(2);
  first(0, 0) = 1;
  first(0, 1) = 2;
  first(1, 0) = 3;
  first(1, 1) = 4;
  Matrix<short> result(2);
  result(0, 0) = 0;
  result(0, 1) = 0;
  result(1, 0) = 0;
  result(1, 1) = 0;
  short n = 0;
  first *= n;
  for (int i = 0; i < result.getRow(); i++) {
    for (int j = 0; j < result.getColum(); j++) {
      GTEST_ASSERT_EQ(first(i, j), result(i, j));
    }
  }
}

TEST(operator_mult_number, friend_1) {
  Matrix<double> first(2);
  first(0, 0) = 1;
  first(0, 1) = 2;
  first(1, 0) = 3;
  first(1, 1) = 4;
  Matrix<double> result(2);
  result(0, 0) = 0;
  result(0, 1) = 0;
  result(1, 0) = 0;
  result(1, 1) = 0;
  double n = 0.0;
  first = n * first;
  for (int i = 0; i < result.getRow(); i++) {
    for (int j = 0; j < result.getColum(); j++) {
      GTEST_ASSERT_EQ(first(i, j), result(i, j));
    }
  }
}

TEST(operator_mult_number, friend_2) {
  Matrix<float> first(2);
  first(0, 0) = 1;
  first(0, 1) = 2;
  first(1, 0) = 3;
  first(1, 1) = 4;
  Matrix<float> result(2);
  result(0, 0) = 0;
  result(0, 1) = 0;
  result(1, 0) = 0;
  result(1, 1) = 0;
  first = static_cast<float>(0.0) * first;
  for (int i = 0; i < result.getRow(); i++) {
    for (int j = 0; j < result.getColum(); j++) {
      GTEST_ASSERT_EQ(first(i, j), result(i, j));
    }
  }
}

TEST(operator_mult_number_rvalue, example_4) {
  Matrix<double> first(2);
  first(0, 0) = 1;
  first(0, 1) = 2;
  first(1, 0) = 3;
  first(1, 1) = 4;
  Matrix<double> result(2);
  result(0, 0) = 0;
  result(0, 1) = 0;
  result(1, 0) = 0;
  result(1, 1) = 0;
  first = first * 0.0;
  for (int i = 0; i < result.getRow(); i++) {
    for (int j = 0; j < result.getColum(); j++) {
      GTEST_ASSERT_EQ(first(i, j), result(i, j));
    }
  }
}

TEST(operator_mult_matrix, example_1) {
  Matrix<int> first(2);
  first(0, 0) = 1;
  first(0, 1) = 2;
  first(1, 0) = 3;
  first(1, 1) = 4;
  Matrix<int> second(2);
  second(0, 0) = 1;
  second(0, 1) = 1;
  second(1, 0) = 1;
  second(1, 1) = 1;
  Matrix<int> result(2);
  result(0, 0) = 3;
  result(0, 1) = 3;
  result(1, 0) = 7;
  result(1, 1) = 7;
  first = first * second;
  for (int i = 0; i < result.getRow(); i++) {
    for (int j = 0; j < result.getColum(); j++) {
      GTEST_ASSERT_EQ(first(i, j), result(i, j));
    }
  }
}

TEST(operator_sum_eq, example_1) {
  Matrix<short> first(2);
  first(0, 0) = 1;
  first(0, 1) = 2;
  first(1, 0) = 3;
  first(1, 1) = 4;
  Matrix<short> second(2);
  second(0, 0) = 1;
  second(0, 1) = 1;
  second(1, 0) = 1;
  second(1, 1) = 1;
  Matrix<short> result(2);
  result(0, 0) = 2;
  result(0, 1) = 3;
  result(1, 0) = 4;
  result(1, 1) = 5;
  first += second;
  for (int i = 0; i < result.getRow(); i++) {
    for (int j = 0; j < result.getColum(); j++) {
      GTEST_ASSERT_EQ(first(i, j), result(i, j));
    }
  }
}

TEST(operator_sub_eq, example_1) {
  Matrix<char> first(2);
  first(0, 0) = 1;
  first(0, 1) = 2;
  first(1, 0) = 3;
  first(1, 1) = 4;
  Matrix<char> second(2);
  second(0, 0) = 1;
  second(0, 1) = 2;
  second(1, 0) = 3;
  second(1, 1) = 4;
  Matrix<char> result(2);
  result(0, 0) = 0;
  result(0, 1) = 0;
  result(1, 0) = 0;
  result(1, 1) = 0;
  first -= second;
  for (int i = 0; i < result.getRow(); i++) {
    for (int j = 0; j < result.getColum(); j++) {
      GTEST_ASSERT_EQ(first(i, j), result(i, j));
    }
  }
}

TEST(operator_eq_move, example_1) {
  Matrix<int8_t> first(2);
  Matrix<int8_t> second(2);
  second(0, 0) = 1;
  second(0, 1) = 2;
  second(1, 0) = 3;
  second(1, 1) = 4;
  Matrix<int8_t> result(2);
  result(0, 0) = 1;
  result(0, 1) = 2;
  result(1, 0) = 3;
  result(1, 1) = 4;
  first = std::move(second);
  for (int i = 0; i < result.getRow(); i++) {
    for (int j = 0; j < result.getColum(); j++) {
      GTEST_ASSERT_EQ(first(i, j), result(i, j));
    }
  }
}

TEST(constructor_move, example_1) {
  Matrix<long> second(2);
  second(0, 0) = 1;
  second(0, 1) = 2;
  second(1, 0) = 3;
  second(1, 1) = 4;
  Matrix<long> result(2);
  result(0, 0) = 1;
  result(0, 1) = 2;
  result(1, 0) = 3;
  result(1, 1) = 4;
  Matrix<long> first(std::move(second));
  for (int i = 0; i < result.getRow(); i++) {
    for (int j = 0; j < result.getColum(); j++) {
      GTEST_ASSERT_EQ(first(i, j), result(i, j));
    }
  }
}

TEST(setRow_scale, example_1) {
  Matrix<int16_t> second(2);
  second(0, 0) = 1;
  second(0, 1) = 2;
  second(1, 0) = 3;
  second(1, 1) = 4;
  second.setRow(3);
  Matrix<int16_t> result(3, 2);
  result(0, 0) = 1;
  result(0, 1) = 2;
  result(1, 0) = 3;
  result(1, 1) = 4;
  result(2, 0) = 0;
  result(2, 1) = 0;
  for (int i = 0; i < result.getRow(); i++) {
    for (int j = 0; j < result.getColum(); j++) {
      GTEST_ASSERT_EQ(second(i, j), result(i, j));
    }
  }
}

TEST(setRow_scale, example_2) {
  Matrix<int64_t> second(3, 2);
  second(0, 0) = 1;
  second(0, 1) = 2;
  second(1, 0) = 3;
  second(1, 1) = 4;
  second(2, 0) = 5;
  second(2, 1) = 8;
  second.setRow(2);
  Matrix<int64_t> result(2, 2);
  result(0, 0) = 1;
  result(0, 1) = 2;
  result(1, 0) = 3;
  result(1, 1) = 4;
  for (int i = 0; i < result.getRow(); i++) {
    for (int j = 0; j < result.getColum(); j++) {
      GTEST_ASSERT_EQ(second(i, j), result(i, j));
    }
  }
}

TEST(setColum_scale, example_1) {
  Matrix<int64_t> second(2, 3);
  second(0, 0) = 1;
  second(0, 1) = 2;
  second(0, 2) = 3;
  second(1, 0) = 3;
  second(1, 1) = 4;
  second(1, 2) = 8;
  second.setColum(2);
  Matrix<int64_t> result(2, 2);
  result(0, 0) = 1;
  result(0, 1) = 2;
  result(1, 0) = 3;
  result(1, 1) = 4;
  for (int i = 0; i < result.getRow(); i++) {
    for (int j = 0; j < result.getColum(); j++) {
      GTEST_ASSERT_EQ(second(i, j), result(i, j));
    }
  }
}

TEST(setColum_scale, example_2) {
  Matrix<char> second(2, 2);
  second(0, 0) = 1;
  second(0, 1) = 2;
  second(1, 0) = 3;
  second(1, 1) = 4;
  second.setColum(3);
  Matrix<char> result(2, 3);
  result(0, 0) = 1;
  result(0, 1) = 2;
  result(0, 2) = 0;
  result(1, 0) = 3;
  result(1, 1) = 4;
  result(1, 2) = 0;
  for (int i = 0; i < result.getRow(); i++) {
    for (int j = 0; j < result.getColum(); j++) {
      GTEST_ASSERT_EQ(second(i, j), result(i, j));
    }
  }
}

TEST(catch_exaption_square_constr, example_1) {
  EXPECT_THROW(Matrix<int>(-2), MyException);
}

TEST(catch_exaption_square_constr, example_2) {
  EXPECT_THROW(Matrix<int>(0), MyException);
}

TEST(catch_exaption_double_constr, example_1) {
  EXPECT_THROW(Matrix<int>(2, -2), MyException);
}

TEST(catch_exaption_double_constr, example_2) {
  EXPECT_THROW(Matrix<int>(-2, 2), MyException);
}

TEST(catch_exaption_double_constr, example_3) {
  EXPECT_THROW(Matrix<double>(0, 0), MyException);
}

TEST(catch_exaption_sumMatrix, example_4) {
  Matrix<short> second(2, 2);
  second(0, 0) = 1;
  second(0, 1) = 2;
  second(1, 0) = 3;
  second(1, 1) = 4;
  Matrix<short> result(2, 3);
  result(0, 0) = 1;
  result(0, 1) = 2;
  result(0, 2) = 0;
  result(1, 0) = 3;
  result(1, 1) = 4;
  result(1, 2) = 0;
  EXPECT_THROW(second.sumMatrix(result), MyException);
}

TEST(catch_exaption_subMatrix, example_5) {
  Matrix<char> second(2, 2);
  second(0, 0) = 1;
  second(0, 1) = 2;
  second(1, 0) = 3;
  second(1, 1) = 4;
  Matrix<char> result(2, 3);
  result(0, 0) = 1;
  result(0, 1) = 2;
  result(0, 2) = 0;
  result(1, 0) = 3;
  result(1, 1) = 4;
  result(1, 2) = 0;
  EXPECT_THROW(second.subMatrix(result), MyException);
}

TEST(catch_exaption_mulMatrix, example_6) {
  Matrix<int8_t> second(2, 2);
  second(0, 0) = 1;
  second(0, 1) = 2;
  second(1, 0) = 3;
  second(1, 1) = 4;
  Matrix<int8_t> result(2, 3);
  result(0, 0) = 1;
  result(0, 1) = 2;
  result(0, 2) = 0;
  result(1, 0) = 3;
  result(1, 1) = 4;
  result(1, 2) = 0;
  EXPECT_THROW(result.mulMatrix(second), MyException);
}

TEST(catch_exaption_determinant, example_7) {
  Matrix<int> result(2, 3);
  result(0, 0) = 1;
  result(0, 1) = 2;
  result(0, 2) = 0;
  result(1, 0) = 3;
  result(1, 1) = 4;
  result(1, 2) = 0;
  EXPECT_THROW(result.determinant(), MyException);
}

TEST(catch_exaption_calcComplements, example_8) {
  Matrix<int> result(2, 3);
  result(0, 0) = 1;
  result(0, 1) = 2;
  result(0, 2) = 0;
  result(1, 0) = 3;
  result(1, 1) = 4;
  result(1, 2) = 0;
  EXPECT_THROW(result.calcComplements(), MyException);
}

TEST(catch_exaption_inverse, example_9) {
  Matrix<char> result(3, 3);
  result(0, 0) = 1;
  result(0, 1) = 2;
  result(0, 2) = 3;
  result(1, 0) = 4;
  result(1, 1) = 5;
  result(1, 2) = 6;
  result(2, 0) = 7;
  result(2, 1) = 8;
  result(2, 2) = 9;
  EXPECT_THROW(result.inverseMatrix(), MyException);
}

TEST(catch_exaption_bracket_operator, example_10) {
  Matrix<short> result(3, 3);
  EXPECT_THROW(result(3, 4) = 2, MyException);
}

TEST(catch_exaption_bracket_operator, example_11) {
  Matrix<int> result(3, 3);
  EXPECT_THROW(result(-3, 4) = 2, MyException);
}

int main(int argc, char **argv) {
testing::InitGoogleTest(&argc, argv);
return RUN_ALL_TESTS();
}