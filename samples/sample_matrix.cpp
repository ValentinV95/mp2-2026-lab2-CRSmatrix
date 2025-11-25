// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Сысоев А.В.
//
// Тестирование матриц

#include <iostream>
#include "tmatrix.h"
//---------------------------------------------------------------------------

void main()
{
  size_t size = 5;
  TMatrix<int> m1(size), m2(size), mult_MS(size), sum_MM(size), sub_MM(size), mult_MM(size);
  TVector<int> v1(size), v2(size), sum_VS(size), sub_VS(size), mult_VS(size), sum_VV(size), sub_VV(size), mult_MV(size);
  int i, j, s = 5, mult_VV;

  setlocale(LC_ALL, "Russian");
  cout << "Тестирование класс работы с матрицами"
    << endl;
  for (i = 0; i < size; i++) {
    v1[i] = i;
    v2[i] = i * 4;
    for (j = i; j < size; j++ )
    {
      m1[i][j] =  i * 10 + j;
      m2[i][j] = (i * 10 + j) * 100;
    }
  }
  
  // Векторно-скалярные операции
  sum_VS = v1 + s; // Сумма вектора и скаляра
  sub_VS = v1 - s; // Разность вектора и скаляра
  mult_VS = v1 * s; // Произведение вектора и скаляра

  // Векторно-векторные операции
  sum_VV = v1 + v2; // Сумма вектора и вектора
  sub_VV = v1 - v2; // Разность вектора и вектора
  mult_VV = v1 * v2; // Скалярное произведение векторов
  
  // Матрично-скалярные операции
  mult_MS = m1 * s; // Произведение матрицы и скаляра

  // матрично-векторные операции
  mult_MV = m1 * v1; // Произведение матрицы и вектора

  // Матрично-матричные операции
  sum_MM = m1 + m2; // Сумма матрицы и матрицы
  sub_MM = m1 - m2; // Разность матрицы и матрицы
  mult_MM = m1 * m2; // Произведение матрицы и матрицы

  // Объявление операндов
  cout << "Matrix m1 = " << endl << m1 << endl;
  cout << "Matrix m2 = " << endl << m2 << endl;
  cout << "Vector v1 = " << endl << v1 << endl << endl;
  cout << "Vector v2 = " << endl << v2 << endl << endl;

  // Вывод результатов
  cout << "Vector sum_VS = v1 + s" << endl << sum_VS << endl << endl;
  cout << "Vector sub_VS = v1 - s" << endl << sub_VS << endl << endl;
  cout << "Vector mult_VS = v1 * s" << endl << mult_VS << endl << endl;

  cout << "Vector sum_VV = v1 + v2" << endl << sum_VV << endl << endl;
  cout << "Vector sub_VV = v1 - v2" << endl << sub_VV << endl << endl;
  cout << "Scalar mult_VV = v1 * v2" << endl << mult_VV << endl << endl;

  cout << "Matrix mult_MS = m1 * s" << endl << mult_MS << endl;

  cout << "Vector mult_MV = m1 * v1" << endl << mult_MV << endl << endl;

  cout << "Matrix sum_MM = m1 + m2" << endl << sum_MM << endl;
  cout << "Matrix sub_MM = m1 - m2" << endl << sub_MM << endl;
  cout << "Matrix mult_MM = m1 * m2" << endl << mult_MM << endl;
}
//---------------------------------------------------------------------------
