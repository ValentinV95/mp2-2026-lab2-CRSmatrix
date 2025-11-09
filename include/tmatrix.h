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
      throw out_of_range("Vector size should be greater than zero");
    if (sz > MAX_VECTOR_SIZE)
        throw out_of_range("Vector size should be less");
    pMem = new T[sz]();// {}; // У типа T д.б. констуктор по умолчанию
  }
  TDynamicVector(T* arr, size_t s) : sz(s)
  {
    assert(arr != nullptr && "TDynamicVector ctor requires non-nullptr arg");
    if (sz == 0)
        throw out_of_range("Vector size should be greater than zero");
    if (sz > MAX_VECTOR_SIZE)
        throw out_of_range("Vector size should be less");
    pMem = new T[sz];
    std::copy(arr, arr + sz, pMem);
  }
  TDynamicVector(const TDynamicVector& v) : sz(v.sz), pMem(new T[sz]())
  {
      std::copy(v.pMem, v.pMem + sz, pMem);
  }
  TDynamicVector(TDynamicVector&& v) noexcept : sz(v.sz)
  {
      std::swap(pMem, v.pMem);
      v.pMem = nullptr;
      v.sz = 0;
  }
  ~TDynamicVector()
  {
      delete[] pMem;
  }
  TDynamicVector& operator=(const TDynamicVector& v)
  {
      if (v != *this) {         
          auto q = TDynamicVector(v);
          std::swap(q, *this);
          sz = v.sz;
      }
      return *this;
  }
  TDynamicVector& operator=(TDynamicVector&& v) noexcept
  {
      if (v != *this) { 
          delete[] pMem;         
          pMem = v.pMem;  
          sz = v.sz;
          v.pMem = nullptr;
          v.sz = 0;
      }
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
      if ((ind < 0) || (ind >= sz)) {
          throw out_of_range("Index out of range");
      }
      else {
          return pMem[ind];
      }
  }
  const T& at(size_t ind) const
  {
      if ((ind < 0) || (ind >= sz)) {
          throw out_of_range("Index out of range");
      }
      else {
          return pMem[ind];
      }
  }

  // сравнение
  bool operator==(const TDynamicVector& v) const noexcept
  {
      if (sz != v.sz) { return 0; }
      for (int i = 0; i < sz; i++) {
          if (v[i] != pMem[i]) {
              return 0;
          }
      }
      return 1;
  }
  bool operator!=(const TDynamicVector& v) const noexcept
  {
      if (sz != v.sz) { return 1; }
      for (int i = 0; i < sz; i++) {
          if (v[i] != pMem[i]) {
              return 1;
          }
      }
      return 0;
  }

  // скалярные операции
  TDynamicVector operator+(const T& val)
  {
      auto v = TDynamicVector(*this);
      for (int i = 0; i < sz; i++) {
          v.pMem[i] += val;
      }
      return v;
  }
  TDynamicVector operator-(const T& val)
  {
      auto v = TDynamicVector(*this);
      for (int i = 0; i < sz; i++) {
          v.pMem[i] -= val;
      }
      return v;
  }
  TDynamicVector operator*(const T& val)
  {
      auto v = TDynamicVector(*this);
      for (int i = 0; i < sz; i++) {
          v.pMem[i] *= val;
      }
      return v;
  }

  // векторные операции
  TDynamicVector operator+(const TDynamicVector& v)
  {
      if (sz != v.sz) {
          throw runtime_error("lengths are not equal");
      }
      else {
          auto q = TDynamicVector(*this);
          for (int i = 0; i < sz; i++) {
              q.pMem[i] += v.pMem[i];
          }
          return q;
      }
  }
  TDynamicVector operator-(const TDynamicVector& v)
  {
      if (sz != v.sz) {
          throw runtime_error("lengths are not equal");
      }
      else {
          auto q = TDynamicVector(*this);
          for (int i = 0; i < sz; i++) {
              q.pMem[i] -= v.pMem[i];
          }
          return q;
      }
  }
  T operator*(const TDynamicVector& v)
  {
      if (sz != v.sz) {
          throw runtime_error("lengths are not equal");
      }
      T q = T();
      for (int i = 0; i < sz; i++) {
          q += pMem[i] * v.pMem[i];
      }
      return q;
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
  size_t s_n;
  size_t s_m;
public:
  TDynamicMatrix(size_t q = 1, size_t r = 1) : s_n(q), s_m(r), TDynamicVector<TDynamicVector<T>>(q)
  {
     if ((s_n == 0) || (s_m == 0))
         throw out_of_range("Matrix size should be greater than zero");
     if ((s_n > MAX_MATRIX_SIZE) || ( s_m > MAX_MATRIX_SIZE))
          throw out_of_range("Matrix size should be less");
    for (size_t i = 0; i < s_n; i++)
      pMem[i] = TDynamicVector<T>(s_m);
  }

  using TDynamicVector<TDynamicVector<T>>::operator[];
  size_t sn() const noexcept { return s_n; }
  size_t sm() const noexcept { return s_m; }
  // сравнение
  bool operator==(const TDynamicMatrix& m) const noexcept
  {
      if ((s_n != m.sn()) || (s_m != m.sm())) {
          return 0;
      }
      for (int i = 0; i < s_n; i++) {
          if (pMem[i] != m.pMem[i]) {
              return 0;
          }
      }
      return 1;
  }
  bool operator!=(const TDynamicMatrix& m) const noexcept
  {
      if ((s_n != m.sn()) || (s_m != m.sm())) {
          return 1;
      }
      for (int i = 0; i < s_n; i++) {
          if (pMem[i] != m.pMem[i]) {
              return 1;
          }
      }
      return 0;
  }

  // матрично-скалярные операции
  TDynamicMatrix<T> operator*(const T& val)
  {
      auto q = TDynamicMatrix<T>(*this);
      for (int i = 0; i < s_n; i++) {
          q.pMem[i] = q.pMem[i]*val;
      }
      return q;
  }

  // матрично-векторные операции
  TDynamicVector<T> operator*(const TDynamicVector<T>& v)
  {
      if (s_m != v.size()) {
          throw runtime_error("lengths are not equal");
      }
      auto q = TDynamicVector<T>(s_n);
      for (int i = 0; i < s_n; i++) {
          q[i] = T();
          for (int j = 0; j < s_m; j++) {
              q[i] += v[j] * pMem[i][j];
          }
      }
      return q;
  }

  // матрично-матричные операции
  TDynamicMatrix operator+(const TDynamicMatrix& m)
  {
      if ((s_n != m.sn()) || (s_m != m.sm())) {
          throw runtime_error("lengths are not equal");
      }
      auto q = TDynamicMatrix<T>(s_n, s_m);
      for (int i = 0; i < s_n; i++) {
          for (int j = 0; j < s_m; j++) {
              q.pMem[i][j] = m[i][j] + pMem[i][j];
          }
      }
      return q;
  }

  TDynamicMatrix operator-(const TDynamicMatrix& m)
  {
      if ((s_n != m.sn()) || (s_m != m.sm())) {
          throw runtime_error("lengths are not equal");
      }
      auto q = TDynamicMatrix<T>(s_n, s_m);
      for (int i = 0; i < s_n; i++) {
          for (int j = 0; j < s_m; j++) {
              q.pMem[i][j] = - m[i][j] + pMem[i][j];
          }
      }
      return q;
  }
  TDynamicMatrix operator*(const TDynamicMatrix& m)
  {
      if (s_m != m.sn()) {
          throw runtime_error("lengths are not equal");
      }
      auto q = TDynamicMatrix<T>(s_n,m.sm());
      for (int i = 0; i < s_n; i++) {
          for (int j = 0; j < m.sm(); j++) {
              q[i][j] = T();
              for (int k = 0; k < s_m; k++) {
                  q[i][j] += pMem[i][k] * m[k][j];
              }
          }
      }
      return q;
  }

  // ввод/вывод
  friend istream& operator>>(istream& istr, TDynamicMatrix& m)
  {
      for (size_t i = 0; i < m.sn(); i++)
          istr >> m[i]; // требуется оператор>> для типа T
      return istr;
  }
  friend ostream& operator<<(ostream& ostr, const TDynamicMatrix& m)
  {
      for (size_t i = 0; i < m.sn(); i++)
          ostr << m[i] << endl; // требуется оператор<< для типа T
      return ostr;
  }
};

#endif
