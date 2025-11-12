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
        for (size_t i = 0; i < sz; i++)
        {
            pMem[i] = v[i];
        }
    }
    TDynamicVector(TDynamicVector&& v) noexcept : sz(v.sz)
    {
        sz = v.sz;
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
         if (this == &v) {
            return *this;
        }
        
        if (pMem != nullptr) {
            delete[] pMem;
        }
        
        sz = v.sz;
        pMem = new T[sz];
        for (size_t i = 0; i < sz; i++) {
            pMem[i] = v.pMem[i];
        }
        
        return *this;
       
    }
    TDynamicVector& operator=(TDynamicVector&& v) noexcept
    {
        if (this == &v) {
            return *this;
        }

        if (pMem != nullptr) {
            delete[] pMem;
        }

        sz = v.sz;
        pMem = v.pMem;
        v.pMem = nullptr;
        v.sz = 0;

        return *this;
        
    }

    size_t size() const noexcept { return sz; }

    // индексация
    T& operator[]( int ind)
    {
        if (ind < 0 || ind >= sz || ind <0) {
            throw out_of_range("Index out of range");
        }
        return pMem[ind];
    }
    const T& operator[](int ind) const
    {
        if (ind < 0 || ind >= sz || ind < 0) {
            throw out_of_range("Index out of range");
        }
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
        if (sz != v.sz) { return false; }
        for (int i = 0; i < sz; i++) {
            if (v[i] != pMem[i]) {
                return false;
            }
        }
        return true;
    }
    bool operator!=(const TDynamicVector& v) const noexcept
    {
        
        return ((((*this)==v)+1)%2);
    }

    // скалярные операции
    TDynamicVector operator+(const T& val)
    {
        TDynamicVector v(*this);
        for (int i = 0; i < sz; i++) {
            v.pMem[i] += val;
        }
        return v;
    }
    TDynamicVector operator-(const T& val)
    {
        TDynamicVector v(*this);
        for (int i = 0; i < sz; i++) {
            v.pMem[i] -= val;
        }
        return v;
    }
    TDynamicVector operator*(const T& val)
    {
        TDynamicVector v(*this);
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
            TDynamicVector q(*this);
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
            TDynamicVector q(*this);
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
private:
  using TDynamicVector<TDynamicVector<T>>::pMem;
  using TDynamicVector<TDynamicVector<T>>::sz;
public:
  TDynamicMatrix(size_t s = 1) : TDynamicVector<TDynamicVector<T>>(s)
  {
      if (sz>= MAX_MATRIX_SIZE)
      {
          throw out_of_range("Too large matrix");
      }
    for (size_t i = 0; i < sz; i++)
      pMem[i] = TDynamicVector<T>(sz);
  }

  using TDynamicVector<TDynamicVector<T>>::operator[];

  // сравнение
  bool operator==(const TDynamicMatrix& m) const noexcept
  {
      if (sz != m.size())
          return false;
      for (size_t i = 0; i < sz; i++)
      {
          if (pMem[i] != m[i])
              return false;
      }
      return true;
  }

  // матрично-скалярные операции
  TDynamicMatrix<T> operator*(const T& val)
  {
      TDynamicMatrix<T> tem(this);
      for (size_t i = 0; i < sz; i++)
      {
          tem[i] = tem[i] * val;
      }
      return tem;
  }

  // матрично-векторные операции
  TDynamicVector<T> operator*(const TDynamicVector<T>& v)
  {
      TDynamicVector<T> Tem(v.size());
      if (v.size() != sz)
      {
          throw length_error("the dimensions of the vector and matrix do not match");
      }
      for (size_t i = 0; i < sz; i++)
      {
          T s();
          for (size_t j = 0; j < sz; j++)
          {
              s = s + this[i][j] * v[j];
          }
          Tem[i] = s;
      }
      return Tem;
  }

  // матрично-матричные операции
  TDynamicMatrix operator+(const TDynamicMatrix& m)
  {
      TDynamicMatrix<T> Tem(m.size());
      if (m.size() != sz)
      {
          throw length_error("the dimensions of the matrixs do not match");
      }
      for (size_t i = 0; i < sz; i++)
      {
          
              Tem[i] = ((*this)[i] + m[i]);
          
      }
      return Tem;
  }
  TDynamicMatrix operator-(const TDynamicMatrix& m)
  {
      TDynamicMatrix<T> Tem(m.size());
      if (m.size() != sz)
      {
          throw length_error("the dimensions of the matrixs do not match");
      }
      for (size_t i = 0; i < sz; i++)
      {
          for (size_t j = 0; j < sz; j++)
          {
              Tem[i][j] = (*this)[i][j] - m[i][j];
          }
      }
      return Tem;
  }
  TDynamicMatrix operator*(const TDynamicMatrix& m)
  {
      TDynamicMatrix<T> Tem(v.size());
      if (v.size() != sz)
      {
          throw length_error("the dimensions of the matrixs do not match");
      }
      for (size_t i = 0; i < sz; ++i) {  
          TDynamicVector<T> res(sz);       

          for (size_t j = 0; j < sz; ++j) {
              T sum();
              for (size_t k = 0; k < sz; ++k) {
                  sum += (*this)[i][k] * m[k][j];
              }
              res[j] = sum;
          }
          Tem[i] = res;
      }
      return Tem;
  }
  TDynamicVector<T>&  operator[](size_t ind)
  {
      return pMem[ind];
  }
  const TDynamicVector<T>& operator[](size_t ind) const
  {
      return pMem[ind];
  }
  // ввод/вывод
  friend istream& operator>>(istream& istr, TDynamicMatrix& v)
  {
      for (size_t i = 0; i < v.sz; i++)
          for (size_t j = 0; j < v.sz; j++) {
                istr >> v.pMem[i][j];
          }
           // требуется оператор>> для типа T
      return istr;
  }
  friend ostream& operator<<(ostream& ostr, const TDynamicMatrix& v)
  {
      for (size_t i = 0; i < v.sz; i++)
          for (size_t j = 0; j < v.sz; j++) {
            ostr << v.pMem[i][j] << ' '; // требуется оператор<< для типа T
          }
      return ostr;
  }
  int get_size() { return sz; };
};

#endif
