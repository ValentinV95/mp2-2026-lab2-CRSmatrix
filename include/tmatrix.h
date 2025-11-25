// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Сысоев А.В.
//
//

#ifndef __TMatrix_H__
#define __TMatrix_H__

#include <cstddef>
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Динамический вектор - 
// шаблонный вектор на динамической памяти
template<typename T>
class TVector
{
private:
  static size_t check_vector_size(size_t sz); // метод для проверки размера
protected:
  size_t sz;
  T* pMem;
public:
  TVector(size_t size = 1); // Конструктор по умолчанию
  TVector(T* arr, size_t s); // Конструктор вектора из массива
  TVector(const TVector& v); // Конструктор копирования
  TVector(TVector&& v) noexcept; // Конструктор перемещения
  ~TVector(); // Деструктор
  TVector& operator=(const TVector& v); // Оператор =
  TVector& operator=(TVector&& v) noexcept; // Оператор перемещающего =

  size_t size() const noexcept; // Метод получения размера вектора

  T& operator[](size_t ind); // Индексация без контроля для неконстантных объектов
  const T& operator[](size_t ind) const; // Индексация без контроля для константных объектов

  T& at(size_t ind); // Индексация с контролем для неконстантных объектов
  const T& at(size_t ind) const; // Индексация с контролем для константных объектов

  bool operator==(const TVector& v) const noexcept; // Оператор сравнения
  bool operator!=(const TVector& v) const noexcept; // Оператор неравенства

  // скалярные операции
  TVector operator+(T val); // Оператор сложения скаляра и вектора
  TVector operator-(T val); // Оператор вычитания скаляра из вектора
  TVector operator*(T val); // Оператор произведения скаляра на вектор

  // векторные операции
  TVector operator+(const TVector& v); // Оператор сложения вектора и вектора
  TVector operator-(const TVector& v); // Оператор вычитания вектора из вектора
  T operator*(const TVector& v); // Оператор произведения вектора на вектор

  friend void swap(TVector& lhs, TVector& rhs) noexcept // Функция обмена содержимым двух векторов
  {
    std::swap(lhs.sz, rhs.sz);
    std::swap(lhs.pMem, rhs.pMem);
  }

  friend istream& operator>>(istream& istr, TVector& v) // Оператор ввода вектора
  {
    for (size_t i = 0; i < v.sz; i++)
    {
      istr >> v.pMem[i]; // требуется оператор >> для типа T
    }

    return istr;
  }

  friend ostream& operator<<(ostream& ostr, const TVector& v) // Оператор вывода вектора
  {
    ostr << "|";

    for (size_t i = 0; i < v.sz; i++)
    {
      ostr << setw(6) << v.pMem[i] << ' '; // требуется оператор << для типа T
    }

    ostr << "|";

    return ostr;
  }
};

// статический метод для проверки размера. Позволяет использовать его для еще неинициализарованных объектов
template<typename T>
size_t TVector<T>::check_vector_size(size_t sz)
{
  if (sz > MAX_VECTOR_SIZE || sz == 0) throw std::out_of_range("Vector size must be greater than zero and not over" + std::to_string(MAX_VECTOR_SIZE));

  return sz;
}

// Конструктор по умолчанию
template<typename T>
TVector<T>::TVector(size_t size) :
sz(check_vector_size(size)),
pMem(new T[sz]())
{
}

// Конструктор из массива
template<typename T>
TVector<T>::TVector(T* arr, size_t s) : sz(s)
{
  // Не знаю, почему тут assert, достаточно throw. Assert аварийно положит программу, если переданный указатель нулевой
  assert(arr != nullptr && "TVector ctor requires non-nullptr arg");

  pMem = new T[sz];
  std::copy(arr, arr + sz, pMem);
}

// Конструктор копирования
template<typename T>
TVector<T>::TVector(const TVector& v) : 
sz(v.sz), 
pMem(new T[v.sz])
{
  for (size_t i = 0; i < sz; ++i)
  {
    pMem[i] = v.pMem[i];
  }
}

// Конструктор перемещения
template<typename T>
TVector<T>::TVector(TVector&& v) noexcept :
sz(v.sz),
pMem(v.pMem)
{ 
  v.sz = 0;
  v.pMem = nullptr;
}

// Деструктор
template<typename T>
TVector<T>::~TVector()
{
  sz = 0;
  delete[] pMem;
  pMem = nullptr;
}

// Метод получения размера вектора
template<typename T>
size_t TVector<T>::size() const noexcept { return sz; } 

// Оператор присваивания
template<typename T>
TVector<T>& TVector<T>::operator=(const TVector& v)
{
  if (this == &v) return *this;

  TVector<T> temp(v);
  swap(*this, temp);
  return *this;
}

// Оператор перемещения
template<typename T>
TVector<T>& TVector<T>::operator=(TVector&& v) noexcept
{
  if (this == &v) return *this;

  swap(*this, v);
  return *this;
}

// Индексация без контроля для неконстантных объектов
template<typename T>
T& TVector<T>::operator[](size_t ind)
{
  return pMem[ind];
}

// Индексация без контроля для константных объектов
template<typename T>
const T& TVector<T>::operator[](size_t ind) const
{
  return pMem[ind];
}

// Индексация с контролем для неконстантных объектов
template<typename T>
T& TVector<T>::at(size_t ind)
{
  if (ind >= sz) throw std::out_of_range("Index out of range");

  return pMem[ind];
}

// Для индексации с контролем у константных объектов
template<typename T>
const T& TVector<T>::at(size_t ind) const
{
  if (ind >= sz) throw std::out_of_range("Index out of range");

  return pMem[ind];
}

// Оператор сравнения
template<typename T>
bool TVector<T>::operator==(const TVector& v) const noexcept
{
  if (sz != v.sz) return false;

  for (size_t i = 0; i < v.sz; ++i)
  {
    if (pMem[i] != v.pMem[i]) return false;
  }

  return true;
}

// Оператор неравенства
template<typename T>
bool TVector<T>::operator!=(const TVector& v) const noexcept
{
  return !(*this == v);
}

// Оператор сложения скаляра и вектора
template<typename T>
TVector<T> TVector<T>::operator+(T val)
{
  TVector<T> result(sz); 

  for (size_t i = 0; i < sz; ++i)
  {
    result.pMem[i] = pMem[i] + val;
  }

  return result;
}

// Оператор вычитания скаляра из вектора
template<typename T>
TVector<T> TVector<T>::operator-(T val)
{
  TVector<T> result(sz); 

  for (size_t i = 0; i < sz; ++i)
  {
    result.pMem[i] = pMem[i] - val;
  }

  return result;
}

// Оператор произведения скаляра на вектор
template<typename T>
TVector<T> TVector<T>::operator*(T val)
{
  TVector<T> result(sz); 

  for (size_t i = 0; i < sz; ++i)
  {
    result.pMem[i] = pMem[i] * val;
  }

  return result;
}

// Оператор сложения вектора и вектора
template<typename T>
TVector<T> TVector<T>::operator+(const TVector& v)
{
  if (sz != v.sz) throw std::invalid_argument("Vectors sizes must be equal for addition");

  TVector<T> result(sz); 

  for (size_t i = 0; i < sz; ++i)
  {
    result.pMem[i] = pMem[i] + v.pMem[i];
  }

  return result;
}

// Оператор вычитания вектора из вектора
template<typename T>
TVector<T> TVector<T>::operator-(const TVector& v)
{
  if (sz != v.sz) throw std::invalid_argument("Vectors sizes must be equal for substraction");

  TVector<T> result(sz); 

  for (size_t i = 0; i < sz; ++i)
  {
    result.pMem[i] = pMem[i] - v.pMem[i];
  }

  return result;
}

// Оператор произведения вектора на вектор
template<typename T>
T TVector<T>::operator*(const TVector& v)
{
  if (sz != v.sz) throw std::invalid_argument("Vectors sizes must be equal for scalar multiplication");

  T result = T();

  for (size_t i = 0; i < sz; ++i)
  {
    result += pMem[i] * v.pMem[i];
  }

  return result;
}

// Динамическая матрица - 
// шаблонная матрица на динамической памяти
template<typename T>
class TMatrix : private TVector<TVector<T>>
{
  using TVector<TVector<T>>::pMem;
  using TVector<TVector<T>>::sz;
private:
  static size_t check_matrix_size(size_t s); // Метод для проверки размера
public:
  TMatrix(size_t s = 1); // Конструктор по умолчанию
  TMatrix(const TMatrix& m);// Конструктор копирования

  using TVector<TVector<T>>::operator[]; // Заимствование оператора индексации без контроля
  using TVector<TVector<T>>::size; // Заимствование оператора возвращения размера
  using TVector<TVector<T>>::at; // Заимствование оператора индексации с контролем

  bool operator==(const TMatrix& m) const noexcept; // Оператор сравнения
  bool operator!=(const TMatrix& m) const noexcept; // Оператор неравенства. Его не было в изначальном шаблоне, решил добавить, почему нет..

  // матрично-скалярные операции
  TMatrix<T> operator*(const T& val); // Произведение матрицы на скаляр

  // матрично-векторные операции
  TVector<T> operator*(const TVector<T>& v); // Произведение матрицы на вектор

  // матрично-матричные операции
  TMatrix operator+(const TMatrix& m); // Сложение матрицы и матрицы
  TMatrix operator-(const TMatrix& m); // Вычитание матрицы из матрицы
  TMatrix operator*(const TMatrix& m); // Произведение матрицы на матрицу

  friend istream& operator>>(istream& istr, TMatrix& m) // Оператор ввода
  {
    for (size_t i = 0; i < m.sz; ++i)
    {
      istr >> m.pMem[i]; // Используем перегруженный оператор ввода вектора
    }

    return istr;
  }

  friend ostream& operator<<(ostream& ostr, const TMatrix& m) // Оператор вывода
  {
    for (size_t i = 0; i < m.sz; ++i)
    {
      ostr << m.pMem[i] << endl; // Используем перегруженный оператор вывода вектора
    }

    return ostr;
  }
};

// статический метод для проверки размера. Позволяет использовать его для еще неинициализарованных объектов
template<typename T>
size_t TMatrix<T>::check_matrix_size(size_t s)
{
  if (s == 0 || s > MAX_MATRIX_SIZE) throw std::out_of_range("Matrix size must be greater than zero and not over" + std::to_string(MAX_MATRIX_SIZE));

  return s;
}

// Конструктор по умолчанию
template<typename T>
TMatrix<T>::TMatrix(size_t s) :
TVector<TVector<T>>(check_matrix_size(s))
{
  for (size_t i = 0; i < sz; i++)
  {
    pMem[i] = TVector<T>(sz);
  }
}

// Конструктор копирования
template<typename T>
TMatrix<T>::TMatrix(const TMatrix& m) :
TVector<TVector<T>>(m)
{
}

// Оператор сравнения
template<typename T>
bool TMatrix<T>::operator==(const TMatrix& m) const noexcept
{
  return TVector<TVector<T>>::operator==(m);
}

// Оператор неравенства
template<typename T>
bool TMatrix<T>::operator!=(const TMatrix& m) const noexcept
{
  return !(*this == m);
}

// Произведение матрицы на скаляр
template<typename T>
TMatrix<T> TMatrix<T>::operator*(const T& val)
{
  TMatrix<T> result(sz);

  for (size_t i = 0; i < sz; ++i)
  {
    result.pMem[i] = pMem[i] * val;
  }

  return result;
}

// Произведение матрицы на вектор
template<typename T>
TVector<T> TMatrix<T>::operator*(const TVector<T>& v)
{
  if (sz != v.size()) throw std::invalid_argument("Matrix and Vector sizes must be equal for multiplication"); // Проверяем соразмерность

  TVector<T> result(sz);

  for (size_t i = 0; i < sz; ++i)
  {
    result[i] = pMem[i] * v;
  }

  return result;
}

// Сложение матрицы и матрицы
template<typename T>
TMatrix<T> TMatrix<T>::operator+(const TMatrix& m)
{
  if (sz != m.sz) throw std::invalid_argument("Matrixes sizes must be equal for addition"); // Проверяем соразмерность

  TMatrix<T> result(sz);

  for (size_t i = 0; i < sz; ++i)
  {
    result.pMem[i] = pMem[i] + m.pMem[i];
  }

  return result;
}

// Вычитание матрицы из матрицы
template<typename T>
TMatrix<T> TMatrix<T>::operator-(const TMatrix& m)
{
  if (sz != m.sz) throw std::invalid_argument("Matrixes sizes must be equal for substraction");

  TMatrix<T> result(sz); 

  for (size_t i = 0; i < sz; ++i)
  {
    result.pMem[i] = pMem[i] - m.pMem[i];
  }

  return result;
}

// Произведение матрицы на матрицу
template<typename T>
TMatrix<T> TMatrix<T>::operator*(const TMatrix& m)
{
  if (sz != m.sz) throw std::invalid_argument("Matrixes sizes must be equal for multiplication");
  TMatrix<T> result(sz); 

  for (size_t i = 0; i < sz; ++i)
  {
    for (size_t j = 0; j < sz; ++j)
    {
      for (size_t k = 0; k < sz; ++k)
      {
        result[i][j] += (*this)[i][k] * m[k][j];
      }
    }
  }

  return result;
}

#endif
