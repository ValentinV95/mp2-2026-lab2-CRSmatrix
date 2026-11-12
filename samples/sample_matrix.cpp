// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Волокитин В.Д.
//
// Тестирование матриц
//---------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include "tmatrix.h"
//---------------------------------------------------------------------------

// Вспомогательная функция для наглядного вывода внутренней структуры CRS
template<typename T>
void printCRSInternals(const TCRSMatrix<T>& m, const string& name)
{
  cout << "--- Внутренняя структура CRS матрицы " << name << " ---" << endl;
  cout << "Размер: " << m.getRows() << "x" << m.getCols() 
       << ", ненулевых элементов (NNZ): " << m.getNNZ() << endl;

  cout << "values:    [ ";
  for (const auto& val : m.getValues()) cout << val << " ";
  cout << "]" << endl;

  cout << "col_index: [ ";
  for (const auto& col : m.getColIndices()) cout << col << " ";
  cout << "]" << endl;

  cout << "row_ptr:   [ ";
  for (const auto& ptr : m.getRowPtr()) cout << ptr << " ";
  cout << "]" << endl << endl;
}

int main()
{

    setlocale(LC_ALL, "Russian");
    cout << "Тестирование класса работы с разреженными матрицами (CRS)" << endl << endl;


    TCRSMatrix<int> a;

    int r, c;
    cout << "Введите количество строк и столбцов матрицы A: ";
    cin >> r >> c;
    if (!cin || r <= 0 || c <= 0)
        throw invalid_argument("Некорректные размеры матрицы");

    TCRSMatrix<int> tmp(r, c);
    cout << "Введите элементы матрицы A:\n";
    cin >> tmp;
    if (!cin)
        throw invalid_argument("Некорректный ввод элементов");

    a = tmp;

    int choice = -1;

    while (choice != 0)
    {
        cout << "\n1. Показать матрицу A и её CRS-массивы\n"
            << "2. Транспонировать матрицу A\n"
            << "3. Умножить матрицу A на вектор\n"
            << "4. Умножить матрицу A на другую матрицу\n"
            << "0. Выход\n"
            << "Выберите: ";
        cin >> choice;

        if (!cin)
        {
            cout << "Ошибка ввода\n";
            break;
        }
        try {
            switch (choice)
            {
            case 0:
                break;

            case 1:
                cout << "Matrix A =\n" << a << endl;
                printCRSInternals(a, "A");
                break;

            case 2:
            {
                TCRSMatrix<int> t = a.transpose();
                cout << "Матрица A^T =\n" << t << endl;
                printCRSInternals(t, "A^T");

                break;
            }

            case 3:
            {
                vector<int> v(a.getCols());
                cout << "Введите вектор. Количество его элементов: " << v.size();
                for (int i = 0; i < v.size(); i++)
                    cin >> v[i];

                if (!cin)
                    throw invalid_argument("Некорректный ввод вектора");

                vector<int> res = a * v;
                cout << "Результат A * v = [ ";
                for (int i = 0; i < res.size(); i++) {
                    cout << res[i] << " ";
                }
                cout << "]" << endl;
                break;
            }

            case 4:
            {
                int r, c;
                cout << "Введите количество строк и столбцов матрицы B: ";
                cin >> r >> c;
                if (!cin || r <= 0 || c <= 0)
                    throw invalid_argument("Некорректные размеры матрицы");

                TCRSMatrix<int> b(r, c);
                cout << "Введите элементы матрицы B:\n";
                cin >> b;
                if (!cin)
                    throw invalid_argument("Некорректный ввод элементов");

                TCRSMatrix<int> res = a * b;
                cout << "Matrix A * B =\n" << res << endl;
                printCRSInternals(res, "A * B");
                break;
            }
            default:
                cout << "Нет такого пункта меню\n";
            }
        }
        catch (const exception& e)
        {
            cout << "Ошибка: " << e.what() << endl;
            break;
        }
    }
    return 0;
}