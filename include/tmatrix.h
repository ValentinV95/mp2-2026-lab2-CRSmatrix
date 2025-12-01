// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Сысоев А.В.
//
//

#ifndef __TDynamicMatrix_H__
#define __TDynamicMatrix_H__

#include <iostream>

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Динамический вектор - 
// шаблонный вектор на динамической памяти
template<typename T>
class TDynamicVector
{
protected:
  size_t sz;
  T* pMem;
public:
  TDynamicVector(size_t size = 1) : sz(size)
  {
    if (sz == 0)
      throw invalid_argument("Vector size should be greater than zero");
    if (sz>MAX_VECTOR_SIZE)
      throw invalid_argument("Vector size overflow");
    pMem = new T[sz]();// {}; // У типа T д.б. констуктор по умолчанию
  }
  TDynamicVector(T* arr, size_t s) : sz(s)
  {
    assert(arr != nullptr && "TDynamicVector ctor requires non-nullptr arg");
    if (sz == 0)
        throw invalid_argument("Vector size should be greater than zero");
    if (sz > MAX_VECTOR_SIZE)
        throw invalid_argument("Vector size overflow");
    pMem = new T[sz];
    std::copy(arr, arr + sz, pMem);
  }
  TDynamicVector(const TDynamicVector& v) : sz(v.sz)
  {
      pMem = new T[sz];
      copy(v.pMem, v.pMem + sz, pMem);
  }
  TDynamicVector(TDynamicVector&& v) noexcept : sz(v.sz)
  {
      pMem = v.pMem;
      v.pMem = nullptr;
      v.sz = 0;
  }
  ~TDynamicVector()
  {
      delete[] pMem;
  }
  TDynamicVector& operator=(const TDynamicVector& v)
  {
      if (v != *this)
      {
          TDynamicVector t = TDynamicVector(v);
          swap(t, *this);
      }
      return *this;
  }
  TDynamicVector& operator=(TDynamicVector&& v) noexcept
  {
      TDynamicVector t = TDynamicVector(v);
      swap(t, *this);
      return *this;
  }

  size_t size() const noexcept { return sz; }

  // индексация
  T& operator[](size_t ind)
  {
      return pMem[ind];
  }
  const T& operator[](size_t ind) const
  {
      return pMem[ind];
  }
  // индексация с контролем
  T& at(size_t ind)
  {
      if (ind < 0 || ind >= sz) { throw out_of_range("index was out of range"); }
      return pMem[ind];
  }
  const T& at(size_t ind) const
  {
      if (ind < 0 || ind >= sz) { throw out_of_range("index was out of range"); }
      return pMem[ind];
  }

  // сравнение
  bool operator==(const TDynamicVector& v) const noexcept
  {
      if (sz != v.sz) { return false; }
      for (int i = 0; i < sz; i++)
      {
          if (v[i] != (*this)[i]) { return false; }
      }
      return true;
  }
  bool operator!=(const TDynamicVector& v) const noexcept
  {
      return !(v == *this);
  }

  // скалярные операции
  TDynamicVector operator+(T val)
  {
      TDynamicVector t = TDynamicVector(*this);
      for (int i = 0; i < sz; i++) t[i] += val;
      return t;
  }
  TDynamicVector operator-(T val)
  {
      TDynamicVector t = TDynamicVector(*this);
      for (int i = 0; i < sz; i++) t[i] -= val;
      return t;
  }
  TDynamicVector operator*(T val)
  {
      TDynamicVector t = TDynamicVector(*this);
      for (int i = 0; i < sz; i++) t[i] *= val;
      return t;
  }

  // векторные операции
  TDynamicVector operator+(const TDynamicVector& v)
  {
      if (sz != v.sz) { throw invalid_argument("vector sizes don't match"); }
      TDynamicVector t = TDynamicVector(*this);
      for (int i = 0; i < sz; i++) t[i] += v[i];
      return t;
  }
  TDynamicVector operator-(const TDynamicVector& v)
  {
      if (sz != v.sz) { throw invalid_argument("vector sizes don't match"); }
      TDynamicVector t = TDynamicVector(*this);
      for (int i = 0; i < sz; i++) t[i] -= v[i];
      return t;
  }
  T operator*(const TDynamicVector& v)
  {
      if (sz != v.sz) { throw invalid_argument("vector sizes don't match"); }
      T t = T();
      for (int i = 0; i < sz; i++) t += v[i]*(*this)[i];
      return t;
  }

  friend void swap(TDynamicVector& lhs, TDynamicVector& rhs) noexcept
  {
    std::swap(lhs.sz, rhs.sz);
    std::swap(lhs.pMem, rhs.pMem);
  }

  // ввод/вывод
  friend istream& operator>>(istream& istr, TDynamicVector& v)
  {
    for (size_t i = 0; i < v.sz; i++)
      istr >> v.pMem[i]; // требуется оператор>> для типа T
    return istr;
  }
  friend ostream& operator<<(ostream& ostr, const TDynamicVector& v)
  {
    for (size_t i = 0; i < v.sz; i++)
      ostr << v.pMem[i] << ' '; // требуется оператор<< для типа T
    return ostr;
  }
};


// Динамическая матрица - 
// шаблонная матрица на динамической памяти
template<typename T>
class TDynamicMatrix : private TDynamicVector<TDynamicVector<T>>
{
  using TDynamicVector<TDynamicVector<T>>::pMem;
  using TDynamicVector<TDynamicVector<T>>::sz;
public:
  TDynamicMatrix(size_t s = 1) : TDynamicVector<TDynamicVector<T>>(s)
  {
      sz = s;
      if (sz == 0)
          throw invalid_argument("Matrix size should be greater than zero");
      if (sz > MAX_MATRIX_SIZE)
          throw invalid_argument("Matrix size overflow");
      for (size_t i = 0; i < sz; i++)
          pMem[i] = TDynamicVector<T>(sz);
  }

  using TDynamicVector<TDynamicVector<T>>::operator[];

  using TDynamicVector<TDynamicVector<T>>::at;

  // сравнение
  bool operator==(const TDynamicMatrix& m) const noexcept
  {
      if (sz != m.sz) { return false; }
      for (int i = 0; i < sz; i++)
      {
          if (m[i] != (*this)[i]) { return false; }
      }
      return true;
  }

  bool operator!=(const TDynamicMatrix& m) const noexcept
  {
      return !(*this == m);
  }

  size_t size()
  {
      return sz;
  }

  // матрично-скалярные операции
  TDynamicMatrix<T> operator*(const T& val)
  {
      TDynamicMatrix t = TDynamicMatrix(*this);
      for (int i = 0; i < sz; i++) t[i] = t[i]* val;
      return t;
  }

  // матрично-векторные операции
  TDynamicVector<T> operator*(const TDynamicVector<T>& v)
  {
      if (sz != v.sz) { throw invalid_argument("matrix and vector sizes dont match"); }
      TDynamicVector<T> t(sz);
      for (int i = 0; i < sz; i++)
      {
          t[i] = T();
          for (int j = 0; j < sz; j++)
          {
              t[i] += v[j] * (*this[i][j]);
          }
      }
      return t;
  }

  // матрично-матричные операции
  TDynamicMatrix operator+(const TDynamicMatrix& m)
  {
      if (sz != m.sz) { throw invalid_argument("matrix sizes dont match"); }
      TDynamicMatrix<T> t(sz);
      for (int i = 0; i < sz; i++)
      {
          t[i] = (*this)[i] + m[i];
      }
      return t;
  }
  TDynamicMatrix operator-(const TDynamicMatrix& m)
  {
      if (sz != m.sz) { throw invalid_argument("matrix sizes dont match"); }
      TDynamicMatrix<T> t(sz);
      for (int i = 0; i < sz; i++)
      {
          t[i] = (*this)[i] - m[i];
      }
      return t;
  }
  TDynamicMatrix operator*(const TDynamicMatrix& m)
  {
      if (sz != m.sz) { throw invalid_argument("matrix sizes dont match"); }
      TDynamicMatrix<T> t(sz);
      for (int i = 0; i < sz; i++)
      {
          for (int j = 0; j < sz; j++)
          {
              t[i][j] = T();
              for (int k = 0; k < sz; k++)
              {
                  t[i][j] += (*this[i][k]) * m[k][j];
              }
          }
      }
      return t;
  }

  // ввод/вывод
  friend istream& operator>>(istream& istr, TDynamicMatrix& v)
  {
      for (int i = 0; i < v.sz; i++) istr >> v[i];
      return istr;
  }
  friend ostream& operator<<(ostream& ostr, const TDynamicMatrix& v)
  {
      for (int i = 0; i < v.sz; i++) ostr << v[i] << '\n';
      return ostr;
  }
};

#endif
