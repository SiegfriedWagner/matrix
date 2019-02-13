#include "gtest/gtest.h"
#include "matrix.hpp" // TODO: Check if can be resolved better
class MatrixIntTest : public ::testing::Test {
public:
  Matrix<int> tested;
protected:
  MatrixIntTest() { tested = Matrix<int>({{1, 2, 3}, {4,5,6}}); }
};

TEST(MatrixTest, ConstructorTest) {
  Matrix<int> testInt = Matrix<int>();
  Matrix<float> testFloat = Matrix<float>();
  Matrix<double> testDouble = Matrix<double>();
  Matrix<Matrix<int>> testMatrixInt = Matrix<Matrix<int>>();
}

TEST_F(MatrixIntTest, AddIntTest) {
  Matrix<int> matrix = Matrix<int>({{2, 3, 4}, {5, 6, 7}});
  ASSERT_EQ(tested + 1, matrix);
  tested+=1;
  ASSERT_EQ(tested, matrix);
}
int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
