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
  TDynamicMatrix<int> a(5), b(5);
  int i, j;
TDynamicMatrix<int> c(a);
  setlocale(LC_ALL, "Russian");
  TDynamicVector<int> m(MAX_VECTOR_SIZE - 1);
  TDynamicVector<int> m2(MAX_VECTOR_SIZE - 1);
  
  m = m2;
  cout <<( c == a) << endl;
}
//---------------------------------------------------------------------------
