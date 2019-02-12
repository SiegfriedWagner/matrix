#ifndef __MATRIX
#define __MATRIX
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <exception>
//TODO: Replace for loop with tasty std::iterators

template <class Type> class Matrix {
protected:
  size_t rows;
  size_t columns;
  size_t size;
  Type *value;
  void resize(size_t columns, size_t rows);
  inline bool dimEqualTo(const Matrix<Type> &other) const;
public:
  Matrix();
  Matrix(size_t columns, size_t rows);
  Matrix(size_t columns, size_t rows, const Type *value);
  Matrix(size_t columns, size_t rows, const Type **value);
  Matrix(std::initializer_list<std::initializer_list<Type>> value);
  Matrix(const Matrix &to_copy);
  Matrix(Matrix &&to_move);
  ~Matrix();
  Matrix<Type> &operator=(const Matrix<Type> &other);
  Matrix<Type> operator+(const Matrix<Type> &other);
  Matrix<Type> operator-(const Matrix<Type> &other);
  Matrix<Type> operator*(const Matrix<Type> &other);
  Matrix<Type> operator%(const Matrix<Type> &other);
  Matrix<Type> &operator+=(const Matrix<Type> &other);
  Matrix<Type> &operator-=(const Matrix<Type> &other);
  Matrix<Type> &operator*=(const Matrix<Type> &other);
  Matrix<Type> &operator%=(const Matrix<Type> &other);
  bool operator==(const Matrix<Type> &other) const;
  bool operator!=(const Matrix<Type> &other);
  Matrix<Type> operator+(const Type &var);
  Matrix<Type> operator-(const Type &var);
  Matrix<Type> operator*(const Type &var);
  Matrix<Type> operator/(const Type &var);
  Matrix<Type> &operator+=(const Type &var);
  Matrix<Type> &operator-=(const Type &var);
  Matrix<Type> &operator*=(const Type &var);
  Matrix<Type> &operator/=(const Type &var);
  Type &operator()(const size_t row, const size_t column);
  Type operator()(const size_t row, const size_t columen) const;
  Matrix<Type> T();
  friend std::ostream &operator<<(std::ostream &o, const Matrix &matrix) {
    o << "[";
    for (int r = 0; r < matrix.rows; ++r) {
      if (!r)
	o << "[";
      else
	o << " [";
      for (int c = 0; c < matrix.columns-1; ++c) {
	o << matrix.value[matrix.columns*r + c] << ", ";
      }
      if (r!=matrix.rows-1)
	o << matrix.value[matrix.columns*r + matrix.columns-1] << "]," << std::endl;
      else
	o << matrix.value[matrix.columns*r + matrix.columns-1] << "]";
    }
    o << "]" << std::endl;
    return o;
  }
  friend std::ofstream &operator<<(std::ofstream &o, const Matrix &matrix) {
    o << matrix.columns << std::endl;
    o << matrix.rows << std::endl;
    for (int i=0; i < matrix.size; ++i) {
      o << *(matrix.value+i) << " ";
    }
    return o;
  }
  friend std::ifstream &operator>>(std::ifstream &o, Matrix &matrix) {
    std::string bufferString;
    int columns, rows;
    getline(o, bufferString);
    columns = stoi(bufferString);
    getline(o, bufferString);
    rows = stoi(bufferString);
    getline(o, bufferString);
    std::stringstream bufferStream(bufferString);
    if (columns*rows != matrix.size)
      matrix.resize(columns, rows);
    for (int i=0; i< matrix.size; ++i) {
      bufferStream >> bufferString;
      matrix.value[i] = stod(bufferString); // TODO: Think about better option to implement that
    }
    return o;
  }
  
  friend Matrix<Type> operator+(Type var, Matrix<Type> &matrix) {
    return matrix+var;
  }

  friend Matrix<Type> operator-(Type var, Matrix<Type> &matrix) {
    //TODO: Consider more optimal solution
    return matrix*(-1)+var;
  }
  friend Matrix<Type> operator*(Type var, Matrix<Type> &matrix) {
    return matrix*var;
  }
  friend Matrix<Type> operator+(Matrix &other, Matrix<Type> &that) {
   if (!that.dimEqualTo(other)) 
    throw std::invalid_argument("Dimensions of both matrixes are different");
   Matrix ret = Matrix(that);
   for (int i=0; i < that.size; ++i) {
     ret.value[i]+= other.value[i];
   }
   return ret;
  }
};

template <typename Type> Matrix<Type>::Matrix(): size(0), rows(0), columns(0), value(nullptr) {}

template<typename Type> Matrix<Type>::Matrix(size_t columns, size_t rows) {
  this->size=columns * rows;
  this->rows = rows;
  this->columns = columns;
  this->value = new Type[size];
}

template<typename Type> Matrix<Type>::Matrix(size_t columns, size_t rows, const Type*value) {
  this->size=columns * rows;
  this->rows = rows;
  this->columns = columns;
  this->value = new Type[size];
  memcpy(this->value, value, columns*rows*sizeof(Type));
}

template <typename Type>
Matrix<Type>::Matrix(std::initializer_list<std::initializer_list<Type>> value) {
  this->rows = value.size();
  this->columns = value.begin()->size();
  this->size = this->columns * this->rows;
  this->value = new Type[size];
  int iter = 0;
  for (auto row = value.begin(); row < value.end(); ++row) {
    for (auto col = (*row).begin(); col < (*row).end(); ++col) {
      this->value[iter] = *col;
      ++iter;
    }
  }
}

template <typename Type> Matrix<Type>::Matrix(const Matrix &to_copy) {
  this->size = to_copy.size;
  this->rows = to_copy.rows;
  this->columns = to_copy.columns;
  this->value = new Type[size];
  memcpy(this->value, to_copy.value, size*sizeof(Type));
}

template <typename Type> Matrix<Type>::Matrix(Matrix &&to_move) {
    this->value = to_move.value;
    this->columns = to_move.columns;
    this->rows = to_move.rows;
    this->size = to_move.size;
    to_move.value = nullptr;
    to_move.columns = 0;
    to_move.rows = 0;
    to_move.size = 0;
}

template <typename Type> Matrix<Type>::~Matrix() {
  delete[] this->value;
}

template <typename Type> void Matrix<Type>::resize(size_t columns, size_t rows) {
  this->columns = columns;
  this->rows = rows;
  this->size = columns*rows;
  delete [] value;
  value = new Type[this->size];
}

template <typename Type> inline bool Matrix<Type>::dimEqualTo(const Matrix &other) const {
  return this->columns == other.columns && this->rows == other.rows;
}

template <typename Type> Type &Matrix<Type>::operator()(size_t column, size_t row) {
  if (column > this->columns-1 || row > this->rows-1)
    throw std::out_of_range("Index out of range"); //TODO: Better explanation, format
  return this->value[row*this->columns+column];
}

template <typename Type> Type Matrix<Type>::operator()(size_t column, size_t row) const {
  if (column > this->columns-1 || row > this->rows-1)
    throw std::out_of_range("Index out of range"); //TODO: Better explanation, format
  return this->value[row*this->columns+column];
}

template <typename Type> Matrix<Type> &Matrix<Type>::operator=(const Matrix &other) {
  if (this != &other) {
    if (this->size != other.size) {
      delete[] this->value;
      this->value = new Type[other.size];
      this->size = other.size;
    }
    this->columns = other.columns;
    this->rows = other.rows;
    for (size_t i = 0; i < this->size; ++i)
      this->value[i] = other.value[i];
  }
  return *this;
}
template <typename Type> Matrix<Type> Matrix<Type>::operator+(const Matrix &other) {
  if (!this->dimEqualTo(other)) 
    throw std::invalid_argument("Dimensions of both matrixes are different");
  Matrix ret = Matrix(*this);
  for (int i=0; i < this->size; ++i) {
    ret.value[i]+= other.value[i];
  }
  return ret;
}

template <typename Type> Matrix<Type> Matrix<Type>::operator-(const Matrix &other) {
  if (!this->dimEqualTo(other)) 
    throw std::invalid_argument("Dimensions of both matrixes are different");
  Matrix ret = Matrix(*this);
  for (int i=0; i < this->size; ++i)
    ret.value[i]-= other.value[i];
  return ret;
}

template <typename Type> Matrix<Type> Matrix<Type>::operator*(const Matrix &other) {
  if (!this->dimEqualTo(other))
    throw  std::invalid_argument("Dimensions of both matrixes are different");
  Matrix ret = Matrix<Type>(*this);
  for (int i=0; i < this->size; ++i)
    ret.value[i]*=other.value[i];
  return ret;
}

template <typename Type> Matrix<Type> Matrix<Type>::operator%(const Matrix &other) {
  if (this->columns != other.rows || this->rows != other.columns)
    throw  std::invalid_argument("Rows and columns does not equal"); // TOOD: Make msg better
  Matrix ret = Matrix<Type>(other.rows, this->columns);
  for (size_t row=0; row < ret.rows; ++row) {
    for (size_t col=0; col < ret.columns; ++col) {
      Type val = Type();
      for (size_t i =0; i < this->rows; ++i) {
	val+=(*this)(row, i)*other(i, col);
      }
      ret(row, col)=val;
    }
  }
  return ret;
}

template <typename Type> bool Matrix<Type>::operator==(const Matrix<Type> &other) const {
  if (this->dimEqualTo(other)) {
    for (size_t i = 0; i < this->size; ++i)
      if (this->value[i] != other.value[i])
	return false;
    return true;
  }
  else
    return false;
}

template <typename Type> bool Matrix<Type>::operator!=(const Matrix<Type> &other) {
  return !(this==other);
}

template <typename Type> Matrix<Type> Matrix<Type>::operator+(const Type &var) {
  Matrix ret = Matrix(this->columns, this->rows);
  for (int i =0; i < ret.size; ++i)
    ret.value[i]=this->value[i]+var;
  return ret;
}

template <typename Type> Matrix<Type> Matrix<Type>::operator-(const Type &var) {
  Matrix ret = Matrix(this->columns, this->rows);
  for (int i =0; i < ret.size; ++i)
    ret.value[i]=this->value[i]-var;
  return ret;
}

template <typename Type> Matrix<Type> Matrix<Type>::operator*(const Type &var) {
  Matrix ret = Matrix(this->columns, this->rows);
  for (int i =0; i < ret.size; ++i)
    ret.value[i]=this->value[i]*var;
  return ret;
}

template <typename Type> Matrix<Type> Matrix<Type>::operator/(const Type &var) {
  // TODO: Divide by 0 exception here
  Matrix ret = Matrix(this->columns, this->rows);
  for (int i =0; i < ret.size; ++i) 
    ret.value[i]=this->value[i]/var;
  return ret;
}

template <typename Type> Matrix<Type> &Matrix<Type>::operator+=(const Type &var) {
  for (int i =0; i < size; ++i)
    value[i]+=var;
  return *this;
}

template <typename Type> Matrix<Type> &Matrix<Type>::operator-=(const Type &var) {
  for (int i =0; i < size; ++i)
    value[i]-=var;
  return *this;
}

template <typename Type> Matrix<Type> &Matrix<Type>::operator*=(const Type &var) {
  for (int i =0; i < size; ++i)
    value[i]*=var;
  return *this;
}

template <typename Type> Matrix<Type> &Matrix<Type>::operator/=(const Type &var) {
  //TODO: Zero divide exception
  for (int i =0; i < size; ++i)
    value[i]+=var;
  return *this;
}

template <typename Type> Matrix<Type> Matrix<Type>::T() { //QUESTION: OPTIONAL INPLACE?
  // Matrix *ret = new Matrix<Type>(this->rows, this->columns);
  Matrix ret = Matrix<Type>(this->rows, this->columns);
  for (int i=0; i < this->rows; ++i) {
    for (int j=0; j < this->columns; ++j) {
      ret(i, j) = (*this)(j, i);
    }
  }
  return ret;
}
#endif
