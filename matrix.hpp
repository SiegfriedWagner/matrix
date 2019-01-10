#include <stdlib.h>
#include <cstring>
#include <iostream>
template <class T> class Matrix {
private:
  size_t rows;
  size_t columns;
  size_t size;
  T *value;
public:
  Matrix();
  Matrix(size_t columns, size_t rows);
  Matrix(size_t columns, size_t rows, const T **value);
  Matrix(size_t widht, size_t rows, const T *value);
  Matrix(const Matrix &to_copy);
  Matrix operator+(const Matrix &other);
  friend std::ostream &operator<<(std::ostream &o, const Matrix &two) {
    o << "[";
    for (int r = 0; r < two.rows; ++r) {
      if (!r)
	o << "[";
      else
	o << " [";
      for (int c = 0; c < two.columns-1; ++c) {
	o << two.value[two.rows*r + c] << ", ";
      }
      if (r!=two.rows-1)
	o << two.value[two.rows*r + two.columns-1] << "]," << std::endl;
      else
	o << two.value[two.rows*r + two.columns-1] << "]";
    }
    o << "]" << std::endl;
    return o;
  }
};

template <typename T> Matrix<T>::Matrix(): size(0), rows(0), columns(0), value(nullptr) {}

template<typename T> Matrix<T>::Matrix(size_t columns, size_t rows) {
  this->size=columns * rows;
  this->rows = rows;
  this->columns = columns;
  this->value = (T*) malloc(size*sizeof(T));
}

template<typename T> Matrix<T>::Matrix(size_t columns, size_t rows, const T*value) {
  this->size=columns * rows;
  this->rows = rows;
  this->columns = columns;
  this->value = (T *) malloc(size*sizeof(T));
  memcpy(this->value, value, columns*rows*sizeof(T));
}

template <typename T> Matrix<T>::Matrix(const Matrix &to_copy) {
  this->size = to_copy.size;
  this->rows = to_copy.rows;
  this->columns = to_copy.columns;
  this->value = (T *) malloc(size*sizeof(T));
  memcpy(this->value, to_copy.value, size*sizeof(T));
}

template <typename T> Matrix<T> Matrix<T>::operator+(Matrix<T> &other) {
  
}
