// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Сысоев А.В.
//
//

#ifndef __TDynamicMatrix_H__
#define __TDynamicMatrix_H__

#include <iostream>
using namespace std;

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
    if (sz <= 0)throw out_of_range("Vector size should be greater than zero");
    if (sz > MAX_VECTOR_SIZE)throw "too large vector";
    pMem = new T[sz]();// {}; // У типа T д.б. констуктор по умолчанию
  }
  TDynamicVector(T* arr, size_t s) : sz(s)
  {
    assert(arr != nullptr && "TDynamicVector ctor requires non-nullptr arg");
    if (sz <= 0)throw out_of_range("Vector size should be greater than zero");
    if (sz > MAX_VECTOR_SIZE)throw "too large vector";
    pMem = new T[sz];
    std::copy(arr, arr + sz, pMem);
  }
  TDynamicVector(const TDynamicVector& v): sz(v.sz)
  {
      pMem = new T[sz];
      std::copy(v.pMem, v.pMem + sz, pMem);
  }
  TDynamicVector(TDynamicVector&& v) noexcept
  {
      pMem = v.pMem;
      sz = v.sz;
      v.sz = 0;
      v.pMem = nullptr;
  }
  ~TDynamicVector()
  {
      sz = 0;
      delete[] pMem;
  }
  TDynamicVector& operator=(const TDynamicVector& v)
  {
      if (&v != this) {
          if (sz == v.sz)std::copy(v.pMem, v.pMem + sz, pMem);
          delete[] pMem;
          sz = v.sz;
          pMem = new T[sz];
          copy(v.pMem, v.pMem + sz, pMem);
      }
      return *this;
  }
  TDynamicVector& operator=(TDynamicVector&& v) noexcept
  {
      delete[] pMem;
      pMem = v.pMem;
      sz = v.sz;
      v.sz = 0;
      v.pMem = nullptr;
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
      if (ind<0 || ind>=sz)throw "[] index out of range";
      return pMem[ind];
  }
  const T& at(size_t ind) const
  {
      if (ind<0 || ind>=sz)throw "[] index out of range";
      return pMem[ind];
  }

  // сравнение
  bool operator==(const TDynamicVector& v) const noexcept
  {
      if (sz != v.sz)return false;
      for (int i = 0; i < sz; i++)if (pMem[i] != v[i])return false;
      return true;
  }
  bool operator!=(const TDynamicVector& v) const noexcept
  {
      return !(*this == v);
  }


  // скалярные операции
  TDynamicVector operator+(T val)
  {
      TDynamicVector<T> t(sz);
      for (int i = 0; i<sz; i++)t[i] = pMem[i]+val;
      return t;
  }
  TDynamicVector operator-(T val)
  {
      TDynamicVector<T> t(sz);
      for (int i = 0; i<sz; i++)t[i] = pMem[i] - val;
      return t;
  }
  TDynamicVector operator*(T val)
  {
      TDynamicVector<T> t(sz);
      for (int i = 0; i<sz; i++)t[i] = pMem[i] * val;
      return t;
  }

  // векторные операции
  TDynamicVector operator+(const TDynamicVector& v)
  {
      if (sz != v.sz)throw "+ lengths do not match";
      TDynamicVector<T> t(sz);
      for (int i = 0; i<sz; i++)t[i] = pMem[i] + v[i];
      return t;
  }
  TDynamicVector operator-(const TDynamicVector& v)
  {
      if (sz != v.sz)throw "- lengths do not match";
      TDynamicVector<T> t(sz);
      for (int i = 0; i<sz; i++)t[i] = pMem[i] - v[i];
      return t;
  }
  T operator*(const TDynamicVector& v) noexcept(T())
  {
      if (sz != v.sz)throw "* lengths do not match";
      T t = 0;
      for (int i = 0; i < sz; i++)t += pMem[i] * v[i];
      return t;
  }

  friend void swap(TDynamicVector& lhs, TDynamicVector& rhs) noexcept
  {
    swap(lhs.sz, rhs.sz);
    swap(lhs.pMem, rhs.pMem);
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
      if (s<0 || s>MAX_MATRIX_SIZE)throw "matrix size out of range";
    for (size_t i = 0; i < sz; i++)
      pMem[i] = TDynamicVector<T>(sz);
  }

  using TDynamicVector<TDynamicVector<T>>::operator[];
  size_t size() const noexcept { return sz; }

  // сравнение
  bool operator==(const TDynamicMatrix& m) const noexcept
  {
      if (sz != m.sz)return false;
      for (int i = 0; i < sz; i++)if (pMem[i] != m[i])return false;
      return true;
  }

  // матрично-скалярные операции
  TDynamicVector<T> operator*(const T& val)
  {
      TDynamicVector<T> t(sz);
      for (int i = 0; i<sz; i++)t[i] = pMem[i] * val;
      return t;
  }

  // матрично-векторные операции
  TDynamicVector<T> operator*(const TDynamicVector<T>& v)
  {
      if (sz != v.sz)throw "m*v lengths do not match";
      TDynamicMatrix<T> t(sz);
      for (int i = 0; i < sz; i++) {
          for (int j = 0; j < sz; j++) {
              for (int k = 0; k < sz; k++) {
                  t[i][k] += (*this)[i][j] * m[k];
              }
          }
      }
  }

  // матрично-матричные операции
  TDynamicMatrix operator+(const TDynamicMatrix& m)
  {
      if (sz != m.sz)throw "m+m lengths do not match";
      TDynamicMatrix<T> t(sz);
      for (int i = 0; i < sz; i++)t[i] = (*this)[i] + m[i];
      return t;
  }
  TDynamicMatrix operator-(const TDynamicMatrix& m)
  {
      if (sz != m.sz)throw "m-m lengths do not match";
      TDynamicMatrix<T> t(sz);
      for (int i = 0; i < sz; i++)t[i] = (*this)[i] - m[i];
      return t;
  }
  TDynamicMatrix operator*(const TDynamicMatrix& m)
  {
      if (sz != m.sz)throw "m*m lengths do not match";
      TDynamicMatrix<T> t(sz);
      for (int i = 0; i < sz; i++){
          for (int j = 0; j < sz; j++) {
              for (int k = 0; k < sz; k++) {
                  for (int l = 0; l < sz; l++) {
                      t[i][j] += (*this)[i][k] * m[l][j];
                  }
              }
          }
      }
      return t;
  }

  // ввод/вывод
  friend istream& operator>>(istream& istr, TDynamicMatrix& v)
  {
      for (int i = 0; i < v.sz; i++)istr >> v[i];
      return istr;
  }
  friend ostream& operator<<(ostream& ostr, const TDynamicMatrix& v)
  {
      for (int i = 0; i < v.sz; i++)ostr<<v[i]<<'\n';
      return ostr;
  }
};

#endif
