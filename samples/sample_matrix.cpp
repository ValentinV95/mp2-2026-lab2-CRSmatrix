// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Сысоев А.В.
//
// Тестирование матриц

#include <iostream>
#include "tmatrix.h"
//---------------------------------------------------------------------------

int main() {
    try {
        int typeChoice = GetChoice("Choose data type:\n1 - float\n2 - double\n3 - int\nYour choice: ", 1, 3);
        switch (typeChoice) {
        case 1: RunCalculator<float>(); break;
        case 2: RunCalculator<double>(); break;
        case 3: RunCalculator<int>(); break;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
//---------------------------------------------------------------------------
