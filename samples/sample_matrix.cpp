#include <iostream>
#include "tmatrix.h"

int main()
{
    setlocale(LC_ALL, "");

    try {
        int n;
        std::cout << "Enter matrix size: ";
        std::cin >> n;

        TDynamicMatrix<int> A(n), B(n);

        std::cout << "Generating random matrix A..." << std::endl;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                A[i][j] = i * 10 + j;

        std::cout << "Matrix A:" << std::endl << A << std::endl;

        std::cout << "Generating matrix B (Identity)..." << std::endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                B[i][j] = (i == j) ? 1 : 0;
            }
        }
        std::cout << "Matrix B:" << std::endl << B << std::endl;

        TDynamicMatrix<int> C = A * B;
        std::cout << "Result of A * B:" << std::endl << C << std::endl;

        if (C == A) {
            std::cout << "Multiplication checked: Correct (A * E = A)." << std::endl;
        } else {
            std::cout << "Multiplication checked: Incorrect." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
