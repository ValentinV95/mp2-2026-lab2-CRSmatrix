// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// sample_matrix.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (21.04.2015)
//

#include <iostream>
#include "utmatrix.h"

using namespace std;

void main()
{
    setlocale(LC_ALL, "Russian");

    cout << "Тестирование класса TMatrix" << endl;

    try
    {
        
        int size;
        cout << "Введите размер матрицы: ";
        cin >> size;

        TMatrix<int> m1(size), m2(size);

        
        cout << "Введите элементы первой матрицы:" << endl;
        cout << "(Для верхнетреугольной матрицы размера n вводится:" << endl;
        cout << " - для строки 0: n элементов" << endl;
        cout << " - для строки 1: n-1 элементов" << endl;
        
        cin >> m1;

        
        cout << "Введите элементы второй матрицы:" << endl;
        cin >> m2;

        
        cout << "\nПервая матрица:" << endl;
        cout << m1 << endl;

        cout << "Вторая матрица:" << endl;
        cout << m2 << endl;

        
        cout << "--- Операции с матрицами ---" << endl;

        
        if (m1 == m2)
            cout << "Матрицы равны" << endl;
        else
            cout << "Матрицы не равны" << endl;

        
        TMatrix<int> sum = m1 + m2;
        cout << "\nСумма матриц (m1 + m2):" << endl;
        cout << sum << endl;

        
        TMatrix<int> diff = m1 - m2;
        cout << "Разность матриц (m1 - m2):" << endl;
        cout << diff << endl;

        
        TMatrix<int> m3(m1);
        cout << "Копия первой матрицы (m3):" << endl;
        cout << m3 << endl;

        
        TMatrix<int> m4(size);
        m4 = m2;
        cout << "Присваивание (m4 = m2):" << endl;
        cout << m4 << endl;

        
        if (m4 == m2)
            cout << "Присваивание выполнено корректно" << endl;

        
        cout << "\nДоступ к элементам матрицы m1:" << endl;
        cout << "Элемент [0][0] = " << m1[0][0] << endl;
        if (size > 1)
            cout << "Элемент [0][1] = " << m1[0][1] << endl;

        
        cout << "\nИзменение элемента m1[0][0] на 999" << endl;
        m1[0][0] = 999;
        cout << "Новое значение m1[0][0] = " << m1[0][0] << endl;

        cout << "\nИзмененная первая матрица:" << endl;
        cout << m1 << endl;

        
        cout << "\n--- Тестирование класса TVector ---" << endl;
        TVector<int> v1(5, 0), v2(5, 0);

        cout << "Введите 5 элементов первого вектора: ";
        cin >> v1;

        cout << "Введите 5 элементов второго вектора: ";
        cin >> v2;

        cout << "\nПервый вектор: " << v1 << endl;
        cout << "Второй вектор: " << v2 << endl;

        TVector<int> v3 = v1 + v2;
        cout << "Сумма векторов: " << v3 << endl;

        TVector<int> v4 = v1 - v2;
        cout << "Разность векторов: " << v4 << endl;

        int scalarProd = v1 * v2;
        cout << "Скалярное произведение: " << scalarProd << endl;

        TVector<int> v5 = v1 * 2;
        cout << "Вектор v1, умноженный на 2: " << v5 << endl;

    }
    catch (const char* error)
    {
        cout << "Ошибка: " << error << endl;
    }
    catch (...)
    {
        cout << "Неизвестная ошибка" << endl;
    }

    cout << "\nНажмите Enter для завершения...";
    cin.ignore();
    cin.get();
}