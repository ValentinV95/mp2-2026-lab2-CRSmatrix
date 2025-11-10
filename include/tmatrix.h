// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Сысоев А.В.
//
//

#ifndef __TDynamicMatrix_H__
#define __TDynamicMatrix_H__

#include <iostream>
#include <stdexcept>
#include <cassert>
#include <algorithm>

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Динамический вектор -
// шаблонный вектор на динамической памяти
template <typename T>
class TDynamicVector {
 protected:
  size_t sz;
  T* pMem;

 public:
  TDynamicVector(size_t size = 1) : sz(size) {
    if (sz == 0 || sz > MAX_VECTOR_SIZE)
      throw out_of_range("Vector size should be greater than zero");

    pMem = new T[sz]();  // {}; // У типа T д.б. констуктор по умолчанию
  }
  TDynamicVector(T* arr, size_t s) : sz(s) {
    assert((arr != nullptr || s > MAX_VECTOR_SIZE) &&
           "TDynamicVector ctor requires non-nullptr arg");

    pMem = new T[sz];
    std::copy(arr, arr + sz, pMem);
  }
  TDynamicVector(const TDynamicVector& v) : sz(v.sz) { 
    if (v.size() == 0)
      throw out_of_range("Vector should be not empty");

    pMem = new T[sz];
    std::copy(v.pMem, v.pMem + sz, pMem);
  }
  TDynamicVector(TDynamicVector&& v) noexcept : sz(v.sz), pMem(v.pMem) {
    v.pMem = nullptr;
  }
  ~TDynamicVector() { delete[] pMem; }
  TDynamicVector& operator=(const TDynamicVector& v) {
    if (this == &v) return *this;

    if (v.size() == 0)
      throw out_of_range("Vector should be not empty");
    
    delete[] pMem;
    sz = v.sz;
    pMem = new T[sz];
    std::copy(v.pMem, v.pMem + sz, pMem);
    return *this;
  }
  TDynamicVector& operator=(TDynamicVector&& v) noexcept {
    if (this == &v) return *this;

    delete[] pMem;
    sz = v.sz;
    pMem = v.pMem;
    v.pMem = nullptr;
    return *this;
  }

  size_t size() const noexcept { return sz; }
  
  // индексация
  T& operator[](size_t ind) { return pMem[ind]; }
  const T& operator[](size_t ind) const { return pMem[ind]; }
  // индексация с контролем
  T& at(size_t ind) {
    if (ind >= sz) throw out_of_range("Index out of range");
    return pMem[ind];
  }
  const T& at(size_t ind) const {
    if (ind >= sz) throw out_of_range("Index out of range");
    return pMem[ind];
  }

  // сравнение
  bool operator==(const TDynamicVector& v) const noexcept { 
    if (sz != v.size()) return false;
    for (size_t i = 0; i < sz; ++i)
      if (pMem[i] != v.pMem[i]) return false;
    return true;
  }
  bool operator!=(const TDynamicVector& v) const noexcept {
    return !(*this == v);
  }

  // скалярные операции
  TDynamicVector operator+(T val) {
    TDynamicVector temp = TDynamicVector(*this);
    for (size_t i = 0; i < sz; i++) temp.pMem[i] += val;
    return temp;
  }
  TDynamicVector operator-(double val) {
    TDynamicVector temp = TDynamicVector(*this);
    for (size_t i = 0; i < sz; i++) temp.pMem[i] -= val;
    return temp;
  }
  TDynamicVector operator*(double val) {
    TDynamicVector temp = TDynamicVector(*this);
    for (size_t i = 0; i < sz; i++) temp.pMem[i] *= val;
    return temp;
  }

  // векторные операции
  TDynamicVector operator+(const TDynamicVector& v) {
    if (sz != v.sz) throw runtime_error("Lengths are not equal");

    TDynamicVector temp = TDynamicVector(*this);
    for (size_t i = 0; i < sz; i++) temp.pMem[i] += v.pMem[i];
    return temp;
  }
  TDynamicVector operator-(const TDynamicVector& v) {
    if (sz != v.sz) throw runtime_error("Lengths are not equal");

    TDynamicVector temp = TDynamicVector(*this);
    for (size_t i = 0; i < sz; i++) temp.pMem[i] -= v.pMem[i];
    return temp;
  }
  T operator*(const TDynamicVector& v) {
    if (sz != v.sz) throw runtime_error("Lengths are not equal");
   
    T sum = T();
    for (size_t i = 0; i < sz; i++)
      sum += pMem[i] * v.pMem[i];
    return sum;
  }

  friend void swap(TDynamicVector& lhs, TDynamicVector& rhs) noexcept {
    std::swap(lhs.sz, rhs.sz);
    std::swap(lhs.pMem, rhs.pMem);
  }

  // ввод/вывод
  friend std::istream& operator>>(std::istream& istr, TDynamicVector& v) {
    for (size_t i = 0; i < v.sz; i++)
      istr >> v.pMem[i];  // требуется оператор>> для типа T
    return istr;
  }
  friend std::ostream& operator<<(std::ostream& ostr, const TDynamicVector& v) {
    for (size_t i = 0; i < v.sz; i++)
      ostr << v.pMem[i] << ' ';  // требуется оператор<< для типа T
    return ostr;
  }
};

// Динамическая матрица -
// шаблонная матрица на динамической памяти
template <typename T>
class TDynamicMatrix : private TDynamicVector<TDynamicVector<T>> {
  using TDynamicVector<TDynamicVector<T>>::pMem;
  using TDynamicVector<TDynamicVector<T>>::sz;

 public:
  TDynamicMatrix(size_t s = 1) : TDynamicVector<TDynamicVector<T>>(s) {
    if (MAX_MATRIX_SIZE < s) 
      throw out_of_range("Matrix size should be less than MAX_MATRIX_SIZE");
    for (size_t i = 0; i < sz; i++) pMem[i] = TDynamicVector<T>(sz);
  }

  using TDynamicVector<TDynamicVector<T>>::operator[];

  // сравнение
  bool operator==(const TDynamicMatrix& m) const noexcept {
    if (sz != m.sz) return false;

    for (size_t i = 0; i < sz; ++i)
      if (pMem[i] != m.pMem[i]) return false;
    return true;
  }

  // матрично-скалярные операции
  TDynamicMatrix operator*(const T& val) {
    TDynamicMatrix temp(*this);

    for (size_t i = 0; i < sz; ++i)
      for (size_t j = 0; j < pMem[i].size(); ++j)
      temp.pMem[i][j] *= val;
    return temp;
  }

  // матрично-векторные операции
  TDynamicVector<T> operator*(const TDynamicVector<T>& v) {
    if (sz != v.size())
      throw runtime_error("Matrix and vector sizes are not equal");

    TDynamicVector<T> result(sz);
    for (size_t i = 0; i < sz; ++i) result[i] = pMem[i] * v;
    return result;
  }

  // матрично-матричные операции
  TDynamicMatrix operator+(const TDynamicMatrix& m) {
    if (sz != m.sz) throw runtime_error("Matrix sizes are not equal");

    TDynamicMatrix temp(*this);
    for (size_t i = 0; i < sz; ++i) temp.pMem[i] = temp.pMem[i] + m.pMem[i];
    return temp;
  }
  TDynamicMatrix operator-(const TDynamicMatrix& m) {
    if (sz != m.sz) throw runtime_error("Matrix sizes are not equal");

    TDynamicMatrix temp(*this);
    for (size_t i = 0; i < sz; ++i) temp.pMem[i] = temp.pMem[i] - m.pMem[i];
    return temp;
  }
  TDynamicMatrix operator*(const TDynamicMatrix& m) {
    if (sz != m.sz) throw runtime_error("Matrix sizes are not equal");

    TDynamicMatrix result(sz);
    TDynamicMatrix m_transposed(sz);
    for (size_t i = 0; i < sz; ++i)
      for (size_t j = 0; j < sz; ++j) m_transposed.pMem[i][j] = m.pMem[j][i];

    for (size_t i = 0; i < sz; ++i)
      for (size_t j = 0; j < sz; ++j)
        result.pMem[i][j] = pMem[i] * m_transposed.pMem[j];

    return result;
  }

  // ввод/вывод
  friend std::istream& operator>>(std::istream& istr, TDynamicMatrix& m) {
    for (size_t i = 0; i < m.sz; i++) istr >> m.pMem[i];

    return istr;
  }
  friend std::ostream& operator<<(std::ostream& ostr, const TDynamicMatrix& m) {
    for (size_t i = 0; i < m.sz; i++)
      ostr << m.pMem[i] << (i < m.sz - 1 ? '\n' : '\n');
    return ostr;
  }
};

#endif
