#include "matrix.hpp"
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{
  int tabInt[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  Matrix<int> matInt = Matrix<int>(3,3,tabInt);
  cout << matInt;
  return 0;
}

