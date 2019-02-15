#ifndef __MATRIX
#define __MATRIX
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <exception>
#include <type_traits>

template <class Type> class Matrix {
protected:
  size_t rows;
  size_t columns;
  size_t size;
  Type *value;
  void resize(size_t columns, size_t rows);
  template<typename X> bool dimEqualTo(const Matrix<X> &other) const;
  inline Type *_begin() { return value; }
  inline Type *_end() { return value+size; }
public:
  inline const Type * begin() const { return value; }
  inline const Type * end() const { return value+size; }
  inline bool dimEqualTo(const Matrix<Type> &other) const
  { return columns == other.columns && rows == other.rows; }
  inline size_t GetRows() const { return rows; }
  inline size_t GetColumns() const { return columns; }
  Matrix();
  Matrix(size_t columns, size_t rows);
  Matrix(size_t columns, size_t rows, const Type *value);
  Matrix(size_t columns, size_t rows, Type **to_move); //Not working
  Matrix(std::initializer_list<std::initializer_list<Type>> value);
  Matrix(const Matrix &to_copy);
  Matrix(Matrix &&to_move);
  ~Matrix();
  template <typename X> operator Matrix<X>() const;
  Matrix<Type> &operator=(const Matrix<Type> &other);
  Matrix<Type> operator+(const Matrix<Type> &other);
  // EXPERIMENTAL: OPERATION WORKS WITH DIFERENT TYPES OF MATRIX
  // template <typename X>  Matrix<Type> operator+(Matrix<X> &other);
  Matrix<Type> operator-(const Matrix<Type> &other);
  Matrix<Type> operator*(const Matrix<Type> &other);
  Matrix<Type> operator%(const Matrix<Type> &other);
  Matrix<Type> &operator+=(const Matrix<Type> &other);
  Matrix<Type> &operator-=(const Matrix<Type> &other);
  Matrix<Type> &operator*=(const Matrix<Type> &other);
  Matrix<Type> &operator/=(const Matrix<Type> &other);
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

  friend Matrix operator+(Type var, Matrix<Type> matrix) {
    return matrix+var;
  };
  
  friend Matrix<Type> operator-(Type var, Matrix<Type> &matrix) {
    //TODO: Consider more optimal solution
    return matrix*(-1)+var;
  }

  friend Matrix<Type> operator*(Type var, Matrix<Type> &matrix) {
    return matrix*var;
  }
   
  friend std::ostream &operator<<(std::ostream &o, const Matrix<Type> &matrix) {
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

  friend std::ofstream &operator<<(std::ofstream &o, Matrix<Type> &matrix) {
    o << matrix.columns << std::endl;
    o << matrix.rows << std::endl;
    for (auto it=matrix.begin(); it < matrix.end(); ++it) {
      o << *(it) << " ";
    }
    return o;
  }

  friend  std::ifstream &operator>>(std::ifstream &o, Matrix<Type> &matrix) {
    std::string bufferString;
    int columns, rows;
    getline(o, bufferString);
    columns = stoi(bufferString);
    getline(o, bufferString);
    rows = stoi(bufferString);
    getline(o, bufferString);
    std::stringstream bufferStream(bufferString);
    if (columns != matrix.columns || rows != matrix.rows)
      matrix.resize(columns, rows);
    for (auto it= matrix.begin(); it < matrix.end(); ++it) {
      bufferStream >> bufferString;
      *(it) = stod(bufferString); // TODO: Think about better option to implement that
    }
    return o;
  }
};

template<typename Type>  template <typename X> inline bool Matrix<Type>::dimEqualTo(const Matrix<X> &other) const {
    return this->GetColumns() == other.GetColumns() && this->GetRows() == other.GetRows();
  }

template <typename Type> void Matrix<Type>::resize(size_t columns, size_t rows) {
  this->columns = columns;
  this->rows = rows;
  this->size = columns*rows;
  delete [] value;
  value = new Type[this->size];
}

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
Matrix<Type>::Matrix(size_t columns, size_t rows, Type **to_move) { //TODO: Fix me: munmap_chunk(): invalid pointer

  this->rows = rows;
  this->columns = columns;
  this->size = columns*rows;
  this->value = *to_move;
  *to_move = (Type *) nullptr;
}

template <typename Type>
Matrix<Type>::Matrix(std::initializer_list<std::initializer_list<Type>> value) {
  this->rows = value.size();
  this->columns = value.begin()->size();
  this->size = this->columns * this->rows;
  this->value = new Type[size];
  auto iter = this->_begin();
  for (auto row = value.begin(); row < value.end(); ++row)
    for (auto col = (*row).begin(); col < (*row).end(); ++col)
      *(iter++) = *col;
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

template <typename Type> template <typename X> Matrix<Type>::operator Matrix<X>() const {
  if (!std::is_convertible<Type, X>::value)
    throw std::runtime_error("Conversion failed"); // TODO: Better explanation, format{
  X tab[this->size];
  X *tab_p = tab;
  if (tab == nullptr)
    throw std::bad_alloc();
  for (auto it = this->begin(); it < this->end(); it++) {
    *(tab_p++) = (Type) (*it);
  }
  Matrix<X> ret = Matrix<X>(this->columns, this->rows, tab);
  return ret;
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
    auto o_iter = other.begin();
    for (auto it = this->_begin(); it < this->end();)
      *(it++) = *(o_iter++);
  }
  return *this;
}
template <typename Type> Matrix<Type> Matrix<Type>::operator+(const Matrix &other) {
  if (!this->dimEqualTo(other)) 
    throw std::invalid_argument("Dimensions of both matrixes are different");
  Matrix ret = Matrix(*this);
  auto o_iter = other.begin();
  for (auto it=ret._begin(); it < ret.end();) {
    *(it++)+=*(o_iter++);
  }
  return ret;
}
// template <typename Type> template <typename X>
// Matrix<Type> Matrix<Type>::operator+(Matrix<X> &other) {
//   if (!this->dimEqualTo(other)) 
//     throw std::invalid_argument("Dimensions of both matrixes are different");
//   Matrix ret = Matrix(*this);
//   auto o_it = other.begin();
//   for (auto it=ret._begin(); it < ret.end();)
//     *(it++)+=*(o_it++);
//   return ret;
// }

template <typename Type> Matrix<Type> Matrix<Type>::operator-(const Matrix &other) {
  if (!this->dimEqualTo(other)) 
    throw std::invalid_argument("Dimensions of both matrixes are different");
  Matrix ret = Matrix(*this);
  auto o_it = other.begin();
  for (auto it=ret._begin(); it < ret.end();)
    *(it++)-= *(o_it++);
  return ret;
}

template <typename Type> Matrix<Type> Matrix<Type>::operator*(const Matrix &other) {
  if (!this->dimEqualTo(other))
    throw  std::invalid_argument("Dimensions of both matrixes are different");
  Matrix ret = Matrix<Type>(*this);
  auto o_it = other.begin();
  for (auto it = ret._begin(); it < ret.end();)
    *(it++) = *(o_it++);
  return ret;
}

template <typename Type> Matrix<Type> Matrix<Type>::operator%(const Matrix &other) {
  if (this->columns != other.rows || this->rows != other.columns)
    throw  std::invalid_argument("Rows and columns does not equal"); // TOOD: Make msg better
  Matrix ret = Matrix<Type>(this->rows, other.columns);
  for (size_t row=0; row < ret.rows; ++row) {
    for (size_t col=0; col < ret.columns; ++col) {
      Type val = Type();
      for (size_t i =0; i < this->columns; ++i) {
	val+=(*this)(i, row)*other(col, i);
      }
      ret(row, col)=val;
    }
  }
  return ret;
}

template <typename Type> bool Matrix<Type>::operator==(const Matrix<Type> &other) const {
  if (this->dimEqualTo(other)) {
    auto o_it = other.begin();
    for (auto it = this->begin(); it < this->end();)
      if (*(it++) != *(o_it++))
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
  auto t_it = this->begin();
  for (auto it = ret._begin(); it < ret.end();)
    *(it++)=*(t_it++)+var;
  return ret;
}

template <typename Type> Matrix<Type> Matrix<Type>::operator-(const Type &var) {
  Matrix ret = Matrix(this->columns, this->rows);
  auto t_it = this->begin();
  for (auto it = ret._begin(); it < ret.end();)
    (*it++) = *(t_it++)-var;
  return ret;
}

template <typename Type> Matrix<Type> Matrix<Type>::operator*(const Type &var) {
  Matrix ret = Matrix(this->columns, this->rows);
  auto t_it = this->begin();
  for (auto it = ret._begin(); it < ret.end();)
    (*it++) = *(t_it++)*var;
  return ret;
}

template <typename Type> Matrix<Type> Matrix<Type>::operator/(const Type &var) {
  if (var == 0)
    throw std::overflow_error("Divide by zero");
  Matrix ret = Matrix(this->columns, this->rows);
  auto t_it = this->begin();
  for (auto it = ret._begin(); it < ret.end();)
    (*it++) = *(t_it++)/var;
  return ret;
}

template <typename Type> Matrix<Type> &Matrix<Type>::operator+=(const Matrix<Type> &other) {
  if (!this->dimEqualTo(other)) 
    throw std::invalid_argument("Dimensions of both matrixes are different");
  auto o_it = other.begin();
  for (auto it = _begin(); it < end();)
    *(it++)+=*(o_it++);
  return *this;
}

template <typename Type> Matrix<Type> &Matrix<Type>::operator-=(const Matrix<Type> &other) {
  if (!this->dimEqualTo(other)) 
    throw std::invalid_argument("Dimensions of both matrixes are different");
  auto o_it = other.begin();
  for (auto it = _begin(); it < end();)
    *(it++)-=*(o_it++);
  return *this;
}

template <typename Type> Matrix<Type> &Matrix<Type>::operator*=(const Matrix<Type> &other) {
  if (!this->dimEqualTo(other)) 
    throw std::invalid_argument("Dimensions of both matrixes are different");
  auto o_it = other.begin();
  for (auto it = _begin(); it < end();)
    *(it++)*=*(o_it++);
  return *this;
}

template <typename Type> Matrix<Type> &Matrix<Type>::operator/=(const Matrix<Type> &other) {
  if (!this->dimEqualTo(other)) 
    throw std::invalid_argument("Dimensions of both matrixes are different");
  auto o_it = other.begin();
  for (auto it = _begin(); it < end();)
    if (*(o_it) == 0)
      throw std::overflow_error("Divide by zero");
    else
      *(it++)/=*(o_it++);
  return *this;
}

template <typename Type> Matrix<Type> &Matrix<Type>::operator+=(const Type &var) {
  for (auto it = this->_begin(); it < this->end(); it++)
    *it+=var;
  return *this;
}

template <typename Type> Matrix<Type> &Matrix<Type>::operator-=(const Type &var) {
  for (auto it = this->_begin(); it < this->end(); it++)
    *it-=var;
  return *this;
}

template <typename Type> Matrix<Type> &Matrix<Type>::operator*=(const Type &var) {
  for (auto it = this->_begin(); it < this->end(); it++)
    *it*=var;
  return *this;
}

template <typename Type> Matrix<Type> &Matrix<Type>::operator/=(const Type &var) {
  if (var == 0)
    throw std::overflow_error("Divide by zero");
  for (auto it = this->_begin(); it < this->end(); it++)
    (*it)/=var;
  return *this;
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
