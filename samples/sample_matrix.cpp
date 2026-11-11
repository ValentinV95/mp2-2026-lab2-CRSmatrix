// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Волокитин В.Д.
//
// Тестирование матриц
//---------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <limits>
#include "tmatrix.h"
//---------------------------------------------------------------------------

template<typename T>
void printCRS(const TCRSMatrix<T>& m, const char* name)
{
    cout << "CRS " << name << " (" << m.getRows() << "x" << m.getCols()
        << ", NNZ=" << m.getNNZ() << ") ---\n";
    cout << "values:    [";
    for (auto v : m.getValues())        cout << ' ' << v;
    cout << " ]\ncol_index: [";
    for (auto c : m.getColIndices())    cout << ' ' << c;
    cout << " ]\nrow_ptr:   [";
    for (auto p : m.getRowPtr())        cout << ' ' << p;
    cout << " ]\n\n";
}

int main()
{
    setlocale(LC_ALL, "Russian");

    TCRSMatrix<int> mat;
    int action;

    while (true) {
        cout << "\n1. ввести матрицу (плотный вид: rows cols, затем rows*cols чисел)\n"
            << "2. вывести матрицу\n"
            << "3. показать внутреннюю структуру CRS\n"
            << "4. транспонировать матрицу\n"
            << "5. умножить матрицу на вектор\n"
            << "6. умножить матрицу на другую матрицу\n"
            << "0. выход\n"
            << "действие: ";

        if (!(cin >> action)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ошибка: ожидалось целое число\n";
            continue;
        }

        if (action == 0)
            break;

        try {
            switch (action) {
            case 1: {
                cout << "введите rows cols, затем " << "rows*cols целых чисел (плотная матрица):\n";

                if (!(cin >> mat))
                    throw invalid_argument("ошибка чтения матрицы");

                cout << "матрица сохранена.\n";
                break;
            }

            case 2:
                cout << "A [" << mat.getRows() << " x " << mat.getCols()
                    << ", NNZ=" << mat.getNNZ() << "]:\n" << mat;
                break;

            case 3:
                printCRS(mat, "A");
                break;

            case 4:
                mat = mat.transpose();
                cout << "матрица после транспонирования:\n"
                    << "A^T [" << mat.getRows() << " x " << mat.getCols()
                    << ", NNZ=" << mat.getNNZ() << "]:\n" << mat;
                printCRS(mat, "A^T");
                break;

            case 5: {
                vector<int> vec(mat.getCols());

                cout << "введите " << vec.size() << " элементов вектора:\n";

                for (auto& value : vec) {
                    if (!(cin >> value))
                        throw invalid_argument("ошибка чтения вектора");
                }

                vector<int> prod = mat * vec;

                cout << "результат: ";
                for (int value : prod)
                    cout << value << ' ';
                cout << '\n';
                break;
            }

            case 6: {
                TCRSMatrix<int> mat2;

                cout << "введите вторую матрицу (rows cols, затем rows*cols чисел):\n";

                if (!(cin >> mat2))
                    throw invalid_argument("ошибка чтения матрицы");

                TCRSMatrix<int> mult = mat * mat2;
                cout << "результат умножения:\n"
                    << "A*B [" << mult.getRows() << " x " << mult.getCols()
                    << ", NNZ=" << mult.getNNZ() << "]:\n" << mult;
                printCRS(mult, "A*B");
                break;
            }

            default:
                cout << "нет такого пункта меню.\n";
            }
        }
        catch (const exception& error) {
            cout << "ошибка: " << error.what() << '\n';
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return 0;
}
//---------------------------------------------------------------------------