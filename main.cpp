#include "libmatrix/matrix.hpp"
#include <iostream>
#include <fstream>
using namespace std;
int main(int argc, char *argv[])
{
  ofstream outStream;
  ifstream inStream;
  int p = 0;
  outStream.open("test.txt", ostream::out);
  int tabInt[9] = {-1, 2, -3, 4, -5, 6, 7, 8, 9};
  int tabInt1[] = {3,2,3,4,4,2,3,4,5,2,3,4};
  Matrix<int> matInt0 = Matrix<int>(3, 3, tabInt);
  Matrix<int> matInt1;
  cout << matInt0;
  outStream << matInt0;
  outStream.close();
  inStream.open("test.txt");
  inStream >> matInt1;
  cout << matInt1;
  cout << matInt0(1,1) <<endl;
  matInt0(1,1) = 10;
  cout << matInt0;
  cout << matInt1;
  cout << matInt0 + matInt1 << endl;
  cout << matInt0 << endl;
  cout << matInt0.T() << endl;
  Matrix<int> matInt2 = Matrix<int>({{1, 2, 3}});
  Matrix<int> matInt3 = Matrix<int>({{4}, {5}, {6}});
  Matrix<int> product = matInt2%matInt3;
  cout << matInt2 << endl;
  cout << matInt3 << endl;
  cout << product << endl;
  cout << (matInt0 += 1) << endl;
  cout << 1 + matInt0 << endl;
  cout << 10 + (p+= 5) << endl;
  cout << p << endl;
  return 0;
}

