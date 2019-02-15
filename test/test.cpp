#include "gtest/gtest.h"
#include "matrix.hpp" // TODO: Check if can be resolved better
#include <exception>
class MatrixIntTest : public ::testing::Test {
public:
  Matrix<int> tested;
  Matrix<int> zero;
protected:
  MatrixIntTest() { tested = Matrix<int>({{1, 2, 3}, {4,5,6}});
    zero = Matrix<int>({{0, 0, 0}, {0,0,0}});
      }
};

TEST(MatrixTest, TypeTest) {
  Matrix<int> testInt = Matrix<int>();
  Matrix<float> testFloat = Matrix<float>();
  Matrix<double> testDouble = Matrix<double>();
  Matrix<Matrix<int>> testMatrixInt = Matrix<Matrix<int>>();
}

TEST(MatrixText, ConstructorTest) {
  int tabInt[9] = {1,2,3,4,5,6};
  Matrix<int> first = Matrix<int>(3,2, tabInt);
  Matrix<int> second = Matrix<int>({{1, 2, 3}, {4, 5, 6}});
  Matrix<int> third = Matrix<int>(first);
  ASSERT_EQ(first, second);
  ASSERT_EQ(first, third);
}

TEST_F(MatrixIntTest, AddIntTestOne) {
  Matrix<int> matrix = Matrix<int>({{2, 3, 4}, {5, 6, 7}});  
  ASSERT_EQ(tested + 1, matrix);
  ASSERT_EQ(1 + tested, matrix);
  tested+=1;
  ASSERT_EQ(tested, matrix);
}

TEST_F(MatrixIntTest, AddIntTestTwo) {
 Matrix<int> twice = Matrix<int>({{2, 4, 6}, {8, 10, 12}});
 ASSERT_EQ(tested+tested, twice);
 tested+=tested;
 ASSERT_EQ(tested, twice);
}
TEST_F(MatrixIntTest, SubtractTest) {
  Matrix<int> matrix = Matrix<int>({{0, 1, 2}, {3, 4, 5}});
  Matrix<int> mat = Matrix<int>({{0, -1, -2}, {-3, -4, -5}});
  ASSERT_EQ(tested - 1, matrix);
  ASSERT_EQ(1 - tested, mat);
  tested-=1;
  ASSERT_EQ(tested, matrix);
  ASSERT_EQ(tested-tested, zero);
  tested-=tested;
  ASSERT_EQ(tested, zero);
}

TEST_F(MatrixIntTest, ElementWise) {
  Matrix<int> _double = Matrix<int>({{2, 4, 6}, {8, 10, 12}});
  Matrix<int> _minus = Matrix<int>({{-1, -2, -3}, {-4, -5, -6}});
  Matrix<int> _pow = Matrix<int>({{1, 2, 9}, {16,25,36}});
  Matrix<int> ones = Matrix<int>({{1, 1, 1}, {1,1,1}});
  ASSERT_EQ(tested * 2, _double);
  ASSERT_EQ(tested * -1, _minus);
  tested*=2;
  ASSERT_EQ(tested, _double);
  tested*=ones;
  ASSERT_EQ(tested, _double);
}

TEST_F(MatrixIntTest, DivideZeroTest) {
  try {
    tested/0;
    FAIL() << "Expected std::overflow_error :1";
  } catch (std::overflow_error const &err) {
    EXPECT_EQ(err.what(), std::string("Divide by zero"));
  }
  catch (...) {
    FAIL() << "Expected std::overflow_error :2";  
  }
  try {
    tested/=0;
    FAIL() << "Expected std::overflow_error :3";
  }
  catch (std::overflow_error const &err) {
    EXPECT_EQ(err.what(), std::string("Divide by zero"));
  }
  catch (...) {
    FAIL() << "Expected std::overflow_error :4";  
  }
}

TEST_F(MatrixIntTest, DivideTest) {
  Matrix<int> divide_two = Matrix<int>({{0, 1, 1}, {2, 2, 3}});
  ASSERT_EQ(tested / 2, divide_two);
  ASSERT_EQ(tested/=2, divide_two);
}

TEST_F(MatrixIntTest, CastTest) {
  Matrix<double> _double = (Matrix<double>) tested;
  Matrix<float> _float = (Matrix<float>) tested;
  ASSERT_EQ(_double, tested);
  ASSERT_EQ(_float, tested);
  Matrix<double> _double_s = Matrix<double>({{1.2, 2.4, 3.7}, {4.9, 5.8, 6.9}});
  ASSERT_EQ(tested, _double_s);
}

TEST_F(MatrixIntTest, TransposeTest) {
  Matrix<int> transposed = Matrix<int>({{1, 4}, {2, 5}, {3, 6}});
  ASSERT_EQ(tested.T(), transposed);
}

TEST_F(MatrixIntTest, CallTest) {
  ASSERT_EQ(tested(0,0), 1);
  try {
    auto test = tested(5,5);
    FAIL() << "Expected std::out_of_range :1";
  }
  catch (std::out_of_range const &err) {
    EXPECT_EQ(err.what(), std::string("Index out of range"));
  }
  catch (...) {
    FAIL() << "Expected std::out_of_range :2";
  }
}

TEST_F(MatrixIntTest, MultiplyTest) {
  Matrix<int> matrix = Matrix<int>({{1,2,3}});
  Matrix<int> matrixT = matrix.T();
  Matrix<int> resutl = Matrix<int>({{14}});
  Matrix<int> result2 = Matrix<int>({{1, 2, 3}, {2, 4, 6}, {3, 6, 9}});
  ASSERT_EQ(matrix%matrixT, resutl);
  ASSERT_EQ(matrixT%matrix, result2);
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
