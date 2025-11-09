// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Сысоев А.В.
//
// Тестирование матриц

#include <iostream>
#include <stdexcept>
#include "tmatrix.h"

using namespace std;

template <typename T>
void run_matrix_test(size_t size) {
  cout << "\n--- Тестирование матриц размера " << size << "x" << size
       << " (Тип: " << typeid(T).name() << ") ---\n";

  try {
    TDynamicMatrix<T> M1(size);
    TDynamicMatrix<T> M2(size);
    TDynamicVector<T> V1(size);

    cout << "Введите элементы для Матрицы M1 (" << size << "x" << size
         << "):" << endl;
    cin >> M1;

    cout << "Введите элементы для Матрицы M2 (" << size << "x" << size
         << "):" << endl;
    cin >> M2;

    cout << "Введите элементы для Вектора V1 (размер " << size << "):" << endl;
    cin >> V1;

    cout << "\n--- Исходные данные ---\n";
    cout << "M1:\n" << M1 << endl;
    cout << "M2:\n" << M2 << endl;
    cout << "V1: " << V1 << endl;

    cout << "\n--- Результаты операций ---\n";

    TDynamicMatrix<T> M_sum = M1 + M2;
    cout << "1. Сложение (M1 + M2):\n" << M_sum << endl;

    TDynamicMatrix<T> M_diff = M1 - M2;
    cout << "2. Вычитание (M1 - M2):\n" << M_diff << endl;

    TDynamicMatrix<T> M_prod = M1 * M2;
    cout << "3. Умножение (M1 * M2):\n" << M_prod << endl;

    TDynamicVector<T> M_V_prod = M1 * V1;
    cout << "4. Умножение на вектор (M1 * V1):\n" << M_V_prod << endl;

    T scalar = 2;
    TDynamicMatrix<T> M_scalar_prod = M1 * scalar;
    cout << "5. Умножение на скаляр (M1 * 2):\n" << M_scalar_prod << endl;

    if (M1 == M1) {
      cout << "6. Сравнение: M1 == M1\n";
    }

  } catch (const out_of_range& e) {
    cerr << "Ошибка диапазона: " << e.what() << endl;
  } catch (const runtime_error& e) {
    cerr << "Ошибка выполнения: " << e.what() << endl;
  } catch (const exception& e) {
    cerr << "Неизвестная ошибка: " << e.what() << endl;
  }
}

int main() {
  setlocale(LC_ALL, "Russian");

  size_t size;
  cout << "Введите размер N для квадратных матриц (NxN): ";
  if (!(cin >> size) || size == 0) {
    return 1;
  }

  run_matrix_test<int>(size);

  return 0;
}
