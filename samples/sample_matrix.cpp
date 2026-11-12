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

int main()
{
  TCRSMatrix<int> A(vector<vector<int>>{
    {1, 0, 2},
    {0, 3, 0},
      {4, 0, 5}
  });

  int choice;

  do {
    cout << "\n===== CRS MATRIX DEMO =====\n";
    cout << "1. Show matrix\n";
    cout << "2. Show CRS view\n";
    cout << "3. Get element (at)\n";
    cout << "4. Transpose matrix\n";
    cout << "5. Multiply by vector (SpMV)\n";
    cout << "6. Multiply by another matrix (SpGEMM)\n";
    cout << "7. Compare with another matrix\n";
    cout << "0. Exit\n";
    cout << "Your choice: ";

    cin >> choice;

    switch (choice)
    {
      case 1:
        cout << "\nMatrix A:\n";
        cout << A;
        break;
      case 2:
        cout << "\nValues: ";
        for (const auto& x : A.getValues())
          cout << x << " ";
          cout << "\nColumn indices: ";
        for (const auto& x : A.getColIndices())
          cout << x << " ";
        cout << "\nRow pointers: ";
        for (const auto& x : A.getRowPtr())
          cout << x << " ";
        cout << "\nNNZ: " << A.getNNZ() << '\n';
        break;
      case 3:
      {
        size_t i, j;
        cout << "Enter row and column: ";
        cin >> i >> j;
        try {
          cout << "A[" << i << "][" << j << "] = " << A.at(i, j) << '\n';
        }
        catch (const out_of_range& e) {
          cout << "Error: " << e.what() << '\n';
        }
        break;
      }

      case 4:
      {
        TCRSMatrix<int> AT = A.transpose();
        cout << "\nTransposed matrix:\n" << AT;
        break;
      }
      case 5:
      {
        vector<int> v(A.getCols());
        cout << "Enter vector (" << A.getCols() << " elements): ";
        for (auto& x : v)
          cin >> x;
        vector<int> result = A * v;
        cout << "Result: ";
        for (const auto& x : result)
          cout << x << " ";
        cout << '\n';
        break;
      }
      case 6:
      {
        TCRSMatrix<int> B(vector<vector<int>>{
          {1, 2, 0},
          {0, 3, 0},
          {4, 0, 5}
        });
        cout << "\nMatrix B:\n" << B;
        TCRSMatrix<int> C = A * B;
        cout << "\nA * B:\n" << C;
          break;
      }

      case 7:
      {
        TCRSMatrix<int> B(vector<vector<int>>{
          {1, 0, 2},
          {0, 3, 0},
          {4, 0, 5}
        });
        cout << (A == B ? "Matrices are equal\n" : "Matrices are not equal\n");
        break;
      }
      case 0:
        cout << "Exit.\n";
        break;
      default:
        cout << "Invalid choice. Try again.\n";
      }
    } while (choice != 0);
    return 0;
  }