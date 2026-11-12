// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Волокитин В.Д.
//
//---------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <string>
#include "tmatrix.h"

using namespace std;

template<typename T>
void show(const TCRSMatrix<T>& m, const string& name)
{
    cout << "--- " << name << " (" << m.getRows() << "x" << m.getCols()
        << ", NNZ=" << m.getNNZ() << ") ---" << endl;
    cout << m << endl;
    cout << "values:    ";
    for (const auto& x : m.getValues()) cout << x << " ";
    cout << "\ncol_index: ";
    for (size_t x : m.getColIndices())  cout << x << " ";
    cout << "\nrow_ptr:   ";
    for (size_t x : m.getRowPtr())      cout << x << " ";
    cout << endl << endl;
}
template<typename T>
void show(const vector<T>& v, const string& name)
{
    cout << name << " = [ ";
    for (const auto& x : v) cout << x << " ";
    cout << "]" << endl;
}
template<typename T>
TCRSMatrix<T> readMatrix(const string& name)
{
    size_t rows = 0, cols = 0;
    cout << "Enter size of " << name << " (rows cols): ";
    cin >> rows >> cols;

    vector<vector<T>> dense(rows, vector<T>(cols, T{}));
    cout << "Enter " << rows * cols << " elements:" << endl;
    for (auto& row : dense)
        for (auto& x : row) cin >> x;

    return TCRSMatrix<T>(dense);
}
void printMenu()
{
    cout << "\n================ MENU ================" << endl;
    cout << " 1. Show internal CRS structure (A and B)" << endl;
    cout << " 2. Transpose A" << endl;
    cout << " 3. Transpose B" << endl;
    cout << " 4. Multiply A by vector v  (SpMV)" << endl;
    cout << " 5. Multiply B by vector v  (SpMV)" << endl;
    cout << " 6. Multiply A by B         (SpGEMM)" << endl;
    cout << " 7. Multiply B by A         (SpGEMM)" << endl;
    cout << " 8. Enter new matrix A" << endl;
    cout << " 9. Enter new matrix B" << endl;
    cout << "10. Enter new vector v" << endl;
    cout << " 0. Exit" << endl;
    cout << "Choice: ";
}
template<typename T>
void run()
{
    const size_t N = 5;
    vector<vector<T>> da(N, vector<T>(N, T{}));
    vector<vector<T>> db(N, vector<T>(N, T{}));
    vector<T> v(N, T{ 1 });
    for (size_t i = 0; i < N; i++)
    {
        db[i][i] = static_cast<T>((i + 1) * 10);
        for (size_t j = i; j < N; j++)
            da[i][j] = static_cast<T>(i * 10 + j + 1);
    }
    TCRSMatrix<T> a(da);
    TCRSMatrix<T> b(db);
    size_t choice = 1;
    do
    {
        printMenu();
        if (!(cin >> choice)) { cin.clear(); cin.ignore(10000, '\n'); continue; }
        try
        {
            switch (choice)
            {
            case 1:  show(a, "A"); show(b, "B");         break;
            case 2:  show(a.transpose(), "A^T");         break;
            case 3:  show(b.transpose(), "B^T");         break;
            case 4:  show(a * v, "A*v");                 break;
            case 5:  show(b * v, "B*v");                 break;
            case 6:  show(a * b, "A*B");                 break;
            case 7:  show(b * a, "B*A");                 break;
            case 8:  a = readMatrix<T>("A");             break;
            case 9:  b = readMatrix<T>("B");             break;
            case 10:
            {
                size_t n = 0;
                cout << "Enter size of vector v: ";
                cin >> n;
                v.assign(n, T{});
                cout << "Enter elements: ";
                for (auto& x : v) cin >> x;
                break;
            }
            case 0:  cout << "Exit." << endl;            break;
            default: cout << "Invalid choice. Please try again." << endl;
            }
        }
        catch (const exception& e)
        {
            cout << "Error: " << e.what() << endl;
        }
    } while (choice != 0);
}
int main()
{
    run<size_t>();
    return 0;
}
//---------------------------------------------------------------------------