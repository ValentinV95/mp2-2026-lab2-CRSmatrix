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
    if (sz <= 0)
        throw out_of_range("Vector size should be straight");
    if (sz > MAX_VECTOR_SIZE)
        throw "Vector should be of a reasonable size";
    pMem = new T[sz]();// {}; // У типа T д.б. констуктор по умолчанию
  }
  TDynamicVector(T* arr, size_t s) : sz(s)
  {
    assert(arr != nullptr && "TDynamicVector vector requires non-nullptr argument");
    if (sz <= 0)
        throw out_of_range("Vector size should be straight");
    if (sz > MAX_VECTOR_SIZE)
        throw "Vector should be of a reasonable size";
    pMem = new T[sz];
    std::copy(arr, arr + sz, pMem);
  }
  TDynamicVector(const TDynamicVector& v):sz(v.sz)
  {
      pMem = new T[sz];
      std::copy(v.pMem, v.pMem+sz, pMem);
  }
  TDynamicVector(TDynamicVector&& v) noexcept
  {
      pMem=v.pMem;
      sz=v.sz;
      v.sz=0;
      v.pMem=nullptr;
  }
  ~TDynamicVector()
  {
      sz=0;
      delete[] pMem;
  }
  TDynamicVector& operator=(const TDynamicVector& v) // Копирующее присваивание
  {
      if(&v!=this)
      {
          if (sz == v.sz)
          {
              std::copy(v.pMem, v.pMem + sz, pMem);
          }
          else 
          {
              delete[] pMem;
              sz = v.sz;
              pMem = new T[sz];
              copy(v.pMem, v.pMem + sz, pMem);
          }
      }
      return *this;
  }
  TDynamicVector& operator=(TDynamicVector&& v) noexcept //Перемещающее присваивание (вроде как эффективнее?)
  {
      delete[] pMem;
      pMem=v.pMem;
      v.pMem=nullptr;
      sz=v.sz;
      v.sz=0;
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
      if (ind<0 || ind>=sz)
          throw "[] index out of range";
      return pMem[ind];
  }
  const T& at(size_t ind) const
  {
      if (ind < 0 || ind >= sz)
          throw "[] index out of range";
      return pMem[ind];
  }

  // сравнение
  bool operator==(const TDynamicVector& v) const noexcept
  {
      if(sz != v.sz)
      {
          return false;
      }
      for(int i=0; i<sz; i++)
      {
          if(pMem[i]!=v[i])
          {
              return false;
          }
      }
      return true;
  }
  bool operator!=(const TDynamicVector& v) const noexcept
  {
      return !(*this==v);
  }

  // скалярные операции
  TDynamicVector operator+(T val)
  {
      TDynamicVector<T> t(sz);
      for (int i = 0; i < sz; i++)
      {
          t[i] = pMem[i] + val;
      }
      return t;
  }
  TDynamicVector operator-(T val)
  {
      TDynamicVector<T> t(sz);
      for (int i = 0; i < sz; i++)
      {
          t[i] = pMem[i] - val;
      }
      return t;
  }
  TDynamicVector operator*(T val)
  {
      TDynamicVector<T> t(sz);
      for (int i = 0; i < sz; i++)
      {
          t[i] = pMem[i] * val;
      }
      return t;
  }

  // векторные операции
  TDynamicVector operator+(const TDynamicVector& v)
  {
      if (sz != v.sz)
          throw "Sum lengths don't match";
      TDynamicVector<T> t(sz);
      for (int i = 0; i < sz; i++)
      {
          t[i] = pMem[i] + v[i];
      }
      return t;
  }
  TDynamicVector operator-(const TDynamicVector& v)
  {
      if (sz != v.sz)
          throw "Subtract lengths don't match";
      TDynamicVector<T> t(sz);
      for (int i = 0; i < sz; i++)
      {
          t[i] = pMem[i] - v[i];
      }
      return t;
  }
  T operator*(const TDynamicVector& v) noexcept(T())
  {
      if (sz != v.sz)
          throw "Multik lengths don't match";
      T t = 0;
      for (int i = 0; i < sz; i++)
      {
          t += pMem[i] * v[i];
      }
      return t;
  }

  friend void swap(TDynamicVector& lhs, TDynamicVector& rhs) noexcept
  {
    std::swap(lhs.sz, rhs.sz);
    std::swap(lhs.pMem, rhs.pMem);
  }

  // ввод/вывод
  friend std::istream& operator>>(std::istream& istr, TDynamicVector& v)
  {
    for (size_t i = 0; i < v.sz; i++)
      istr >> v.pMem[i]; // требуется оператор>> для типа T
    return istr;
  }
  friend std::ostream& operator<<(std::ostream& ostr, const TDynamicVector& v)
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
      if (s<0 || s>MAX_MATRIX_SIZE)
          throw "matrix size out of range";
      for (size_t i = 0; i < sz; i++)
      {
          pMem[i] = TDynamicVector<T>(sz);
      }
  }

  using TDynamicVector<TDynamicVector<T>>::operator[];
  using TDynamicVector<TDynamicVector<T>>::at;
  
  size_t size() const noexcept { return sz; }

  // сравнение
  bool operator==(const TDynamicMatrix& m) const noexcept
  {
      if (sz != m.sz)
      {
          return false;
      }
      for (int i = 0; i < sz; i++)
      {
          if (pMem[i] != m[i])
          {
              return false;
          }
      }
      return true;
  }

  // матрично-скалярные операции
  TDynamicVector<T> operator*(const T& val)
  {
      TDynamicVector<T> t(sz);
      for (int i = 0; i < sz; i++)
      {
          t[i] = pMem[i] * val;
      }
      return t;
  }

  // матрично-векторные операции
  TDynamicVector<T> operator*(const TDynamicVector<T>& v)
  {
      if (sz != v.sz)throw "MatVec_Mult lengths don't match";
      TDynamicMatrix<T> t(sz);
      for (int i = 0; i < sz; i++) 
      {
          for (int j = 0; j < sz; j++) 
          {
              for (int r = 0; r < sz; k++) 
              {
                  t[i][r] += (*this)[i][j] * m[r];
              }
          }
      }
  }

  // матрично-матричные операции
  TDynamicMatrix operator+(const TDynamicMatrix& m)
  {
      if (sz != m.sz)
          throw "mATmAT_sUM lengths don't match";
      TDynamicMatrix<T> t(sz);
      for (int i = 0; i < sz; i++)
      {
          t[i] = (*this)[i] + m[i];
      }
      return t;
  }
  TDynamicMatrix operator-(const TDynamicMatrix& m)
  {
      if (sz != m.sz)
          throw "mATmAT_sUBT lengths don't match";
      TDynamicMatrix<T> t(sz);
      for (int i = 0; i < sz; i++)
      {
          t[i] = (*this)[i] - m[i];
      }
      return t;
  }
  TDynamicMatrix operator*(const TDynamicMatrix& m)
  {
      if (sz != m.sz)throw "mATmAT_mULT lengths don't match";
      TDynamicMatrix<T> t(sz);
      for (int i = 0; i < sz; i++) 
      {
          for (int j = 0; j < sz; j++) 
          {
              for (int k = 0; k < sz; k++) 
              {
                  for (int l = 0; l < sz; l++) 
                  {
                      t[i][j] += (*this)[i][k] * m[l][j];
                  }
              }
          }
      }
      return t;
  }

  // ввод/вывод
  friend std::istream& operator>>(std::istream& istr, TDynamicMatrix& v)
  {
      for (int i = 0; i < v.sz; i++)
      {
          istr >> v[i];
      }
      return istr;
  }
  friend std::ostream& operator<<(std::ostream& ostr, const TDynamicMatrix& v)
  {
      for (int i = 0; i < v.sz; i++)
      {
          ostr << v[i] << '\n';
      }
      return ostr;
  }
};

#endif
