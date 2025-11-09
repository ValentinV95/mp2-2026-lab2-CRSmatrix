// Пример использования класса TMatrix
#include <iostream>
#include "tmatrix.h"

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");

    cout << "Демонстрация работы с верхнетреугольными матрицами" << endl;
    cout << endl;

    int size;
    cout << "Введите размер матрицы: ";
    cin >> size;

    try
    {
        TMatrix<int> m1(size);
        TMatrix<int> m2(size);

        cout << endl << "Заполнение первой матрицы:" << endl;
        for (int i = 0; i < size; i++)
        {
            for (int j = i; j < size; j++)
            {
                cout << "m1[" << i << "][" << j << "] = ";
                cin >> m1[i][j];
            }
        }

        cout << endl << "Заполнение второй матрицы:" << endl;
        for (int i = 0; i < size; i++)
        {
            for (int j = i; j < size; j++)
            {
                cout << "m2[" << i << "][" << j << "] = ";
                cin >> m2[i][j];
            }
        }

        cout << endl << "Первая матрица:" << endl;
        cout << m1 << endl;

        cout << "Вторая матрица:" << endl;
        cout << m2 << endl;

        if (m1 == m2)
            cout << "Матрицы равны" << endl;
        else
            cout << "Матрицы не равны" << endl;

        cout << endl << "Сумма матриц (m1 + m2):" << endl;
        TMatrix<int> sum = m1 + m2;
        cout << sum << endl;

        cout << "Разность матриц (m1 - m2):" << endl;
        TMatrix<int> diff = m1 - m2;
        cout << diff << endl;

        cout << "Копирование матрицы m1:" << endl;
        TMatrix<int> m3 = m1;
        cout << m3 << endl;

        m3[0][0] = 999;
        cout << "После изменения копии:" << endl;
        cout << "m1[0][0] = " << m1[0][0] << endl;
        cout << "m3[0][0] = " << m3[0][0] << endl;
    }
    catch (const char* error)
    {
        cout << "Ошибка: " << error << endl;
        return 1;
    }
    catch (...)
    {
        cout << "Произошла неизвестная ошибка" << endl;
        return 1;
    }

    return 0;
}