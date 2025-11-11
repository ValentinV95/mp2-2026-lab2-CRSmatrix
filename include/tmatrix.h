// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Сысоев А.В.
//
//

#ifndef __TMATRIX_H__
#define __TMATRIX_H__

#include <iostream>
#include <iomanip>
#include <cassert>
#include <random>
#include <limits>
#include <type_traits>

#define MAX_VECTOR_SIZE 100000000
#define MAX_MATRIX_SIZE 10000

template<typename T>
class TVector {
protected:
    size_t sz;
    T* pMem;

private:
    void CheckSize(const TVector& lv, const TVector& rv) const {
        if (lv.sz != rv.sz) {
            throw std::invalid_argument("Vector sizes don't match");
        }
    }

public:
    TVector() : sz(0), pMem(nullptr) {}

    explicit TVector(size_t size) : sz(size), pMem(new T[sz]()) {
        if (sz == 0 || sz > MAX_VECTOR_SIZE) {
            throw std::out_of_range("Invalid vector size");
        }
    }

    TVector(const T* arr, size_t s) : sz(s), pMem(new T[sz]()) {
        if (!arr) {
            throw std::invalid_argument("Invalid pointer");
        }
        if (sz == 0 || sz > MAX_VECTOR_SIZE) {
            throw std::out_of_range("Invalid vector size");
        }
        std::copy(arr, arr + sz, pMem);
    }

    TVector(const TVector& v) : sz(v.sz), pMem(new T[sz]()) {
        std::copy(v.pMem, v.pMem + sz, pMem);
    }

    TVector(TVector&& v) noexcept : sz(v.sz), pMem(v.pMem) {
        v.sz = 0;
        v.pMem = nullptr;
    }

    ~TVector() { delete[] pMem; }

    TVector& operator=(const TVector& v) {
        if (this == &v) { return *this; }
        TVector tmp(v);
        swap(*this, tmp);
        return *this;
    }

    TVector& operator=(TVector&& v) noexcept {
        if (this == &v) { return *this; }
        TVector tmp(std::move(v));
        swap(*this, tmp);
        return *this;
    }

    size_t size() const noexcept { return sz; }

    T& operator[](size_t ind) noexcept { return pMem[ind]; }
    const T& operator[](size_t ind) const noexcept { return pMem[ind]; }

    T& at(size_t ind) {
        if (ind >= sz) { throw std::out_of_range("Vector index out of range"); }
        return pMem[ind];
    }

    const T& at(size_t ind) const {
        if (ind >= sz) { throw std::out_of_range("Vector index out of range"); }
        return pMem[ind];
    }

    bool operator==(const TVector& v) const noexcept {
        if (sz != v.sz) {
            return false;
        }
        for (size_t i = 0; i < sz; ++i) {
            if (pMem[i] != v.pMem[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const TVector& v) const noexcept {
        return !(*this == v);
    }

    TVector operator+(const T& val) const {
        TVector res(sz);
        for (size_t i = 0; i < sz; ++i) {
            res.pMem[i] = pMem[i] + val;
        }
        return res;
    }

    TVector operator-(const T& val) const {
        TVector res(sz);
        for (size_t i = 0; i < sz; ++i) {
            res.pMem[i] = pMem[i] - val;
        }
        return res;
    }

    TVector operator*(const T& val) const {
        TVector res(sz);
        for (size_t i = 0; i < sz; ++i) {
            res.pMem[i] = pMem[i] * val;
        }
        return res;
    }

    TVector operator+(const TVector& v) const {
        CheckSize(*this, v);
        TVector res(sz);
        for (size_t i = 0; i < sz; ++i) {
            res.pMem[i] = pMem[i] + v.pMem[i];
        }
        return res;
    }

    TVector operator-(const TVector& v) const {
        CheckSize(*this, v);
        TVector res(sz);
        for (size_t i = 0; i < sz; ++i) {
            res.pMem[i] = pMem[i] - v.pMem[i];
        }
        return res;
    }

    T operator*(const TVector& v) const {
        CheckSize(*this, v);
        T res = T();
        for (size_t i = 0; i < sz; ++i) {
            res += pMem[i] * v.pMem[i];
        }
        return res;
    }

    friend void swap(TVector& lhs, TVector& rhs) noexcept {
        std::swap(lhs.sz, rhs.sz);
        std::swap(lhs.pMem, rhs.pMem);
    }

    friend std::istream& operator>>(std::istream& istr, TVector& v) {
        for (size_t i = 0; i < v.sz; ++i) {
            istr >> v.pMem[i];
        }
        return istr;
    }

    friend std::ostream& operator<<(std::ostream& ostr, const TVector& v) {
        for (size_t i = 0; i < v.sz; ++i) {
            ostr << std::setw(12) << std::fixed << std::setprecision(3) << v.pMem[i];
        }
        return ostr;
    }
};

template<typename T>
class TMatrix : private TVector<TVector<T>> {
private:
    size_t rows, cols;
    using Base = TVector<TVector<T>>;

    void CheckIndex(size_t row) const {
        if (row >= rows) {
            throw std::out_of_range("Row index out of range");
        }
    }
    static void CheckSize(const TMatrix& m1, const TMatrix& m2) {
        if (m1.rows != m2.rows || m1.cols != m2.cols) {
            throw std::invalid_argument("Matrix sizes don't match");
        }
    }

public:

    size_t GetRows() const noexcept { return rows; }
    size_t GetCols() const noexcept { return cols; }

    TMatrix() : rows(0), cols(0), Base() {}

    TMatrix(size_t r, size_t c) : rows(r), cols(c), Base(r) {
        if (rows == 0 || cols == 0 || rows > MAX_MATRIX_SIZE || cols > MAX_MATRIX_SIZE) {
            throw std::out_of_range("Invalid matrix size");
        }
        for (size_t i = 0; i < rows; ++i) {
            (*this)[i] = TVector<T>(cols);
        }
    }

    ~TMatrix() = default;

    TMatrix(const TMatrix& other) : rows(other.rows), cols(other.cols), Base(other) {}

    TMatrix& operator=(const TMatrix& m) {
        if (this == &m) {
            return *this;
        }
        rows = m.rows;
        cols = m.cols;
        Base::operator=(m);
        return *this;
    }


    TMatrix(TMatrix&& m) noexcept : rows(m.rows), cols(m.cols), Base(std::move(m)) {}

    TMatrix& operator=(TMatrix&& m) noexcept {
        if (this == &m) {
            return *this;
        }
        rows = m.rows;
        cols = m.cols;
        Base::operator=(std::move(m));
        m.rows = 0;
        m.cols = 0;
        return *this;
    }

    TVector<T>& operator[](size_t ind) noexcept { return Base::operator[](ind); }
    const TVector<T>& operator[](size_t ind) const noexcept { return Base::operator[](ind); }

    TVector<T>& at(size_t ind) {
        CheckIndex(ind);
        return (*this)[ind];
    }

    const TVector<T>& at(size_t ind) const {
        CheckIndex(ind);
        return (*this)[ind];
    }

    bool operator==(const TMatrix& m) const noexcept {
        if (rows != m.rows || cols != m.cols) {
            return false;
        }
        for (size_t i = 0; i < rows; ++i) {
            if ((*this)[i] != m[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const TMatrix& m) const noexcept {
        return !(*this == m);
    }

    TMatrix operator*(const T& val) const {
        TMatrix res(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            res[i] = (*this)[i] * val;
        }
        return res;
    }

    TVector<T> operator*(const TVector<T>& v) const {
        if (cols != v.size()) {
            throw std::invalid_argument("Matrix and vector dimensions don't match");
        }

        TVector<T> res(rows);
        for (size_t i = 0; i < rows; ++i) {
            T sum = T();
            for (size_t j = 0; j < cols; ++j) {
                sum += (*this)[i][j] * v[j];
            }
            res[i] = sum;
        }
        return res;
    }

    TMatrix operator+(const TMatrix& m) const {
        CheckSize(*this, m);
        TMatrix res(m.rows, m.cols);
        for (size_t i = 0; i < m.rows; ++i) {
            for (size_t j = 0; j < m.cols; ++j) {
                res[i][j] = (*this)[i][j] + m[i][j];
            }
        }
        return res;
    }

    TMatrix operator-(const TMatrix& m) const {
        CheckSize(*this, m);
        TMatrix res(m.rows, m.cols);
        for (size_t i = 0; i < m.rows; ++i) {
            for (size_t j = 0; j < m.cols; ++j) {
                res[i][j] = (*this)[i][j] - m[i][j];
            }
        }
        return res;
    }

    TMatrix operator*(const TMatrix& m) const {
        if (cols != m.rows) {
            throw std::invalid_argument("Matrix dimensions don't match for multiplication");
        }
        TMatrix res(rows, m.cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < m.cols; ++j) {
                T sum = T();
                for (size_t k = 0; k < cols; ++k) {
                    sum += (*this)[i][k] * m[k][j];
                }
                res[i][j] = sum;
            }
        }
        return res;
    }

    friend std::istream& operator>>(std::istream& istr, TMatrix& m) {
        for (size_t i = 0; i < m.rows; ++i) {
            if (!(istr >> m[i])) {
                throw std::invalid_argument("Invalid input");
            }
        }
        return istr;
    }

    friend std::ostream& operator<<(std::ostream& ostr, const TMatrix& m) {
        for (size_t i = 0; i < m.rows; ++i) {
            ostr << m[i] << std::endl;
        }
        return ostr;
    }

};

enum class MatrixType { OwnMatrices = 1, RandomMatrices = 2 };
enum class Operation { ADD = 1, SUB = 2, MUL = 3 };

template <typename T>
TMatrix<T> TOwnMatrix(size_t rows, size_t cols) {
    std::cout << "Enter " << rows << " x " << cols << " matrix:\n";
    TMatrix<T> m(rows, cols);
    std::cin >> m;
    return m;
}

template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, TMatrix<T>>::type
TMatrixGenerator(size_t rows, size_t cols) {
    TMatrix<T> m(rows, cols);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<T> dist(-1000.0, 1000.0);

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            m[i][j] = dist(gen);
        }
    }
    return m;
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value, TMatrix<T>>::type
TMatrixGenerator(size_t rows, size_t cols) {
    TMatrix<T> m(rows, cols);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<T> dist(-1000, 1000);

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            m[i][j] = dist(gen);
        }
    }
    return m;
}

inline int GetChoice(const std::string& str, int min_val, int max_val) {
    int value;
    while (true) {
        std::cout << str;
        if (!(std::cin >> value) || value < min_val || value > max_val) {
            std::cout << "Invalid input! Enter a number between " << min_val << " and " << max_val << "\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

inline MatrixType ChooseMatrixType() {
    std::cout << "\nChoose Matrix Type:\n";
    std::cout << "1. Enter matrices manually\n";
    std::cout << "2. Generate random matrices\n";

    return static_cast<MatrixType>(GetChoice("Enter your choice (1-2): ", 1, 2));
}

inline Operation ChooseOperation() {
    std::cout << "\nChoose Operation:\n";
    std::cout << "1. A + B\n";
    std::cout << "2. A - B\n";
    std::cout << "3. A * B\n";
    return static_cast<Operation>(GetChoice("Enter your choice (1-3): ", 1, 3));
}

inline void GetMatrixSize(size_t& rows, size_t& cols) {
    std::cout << "\nEnter matrix size:\n";
    rows = GetChoice("Rows: ", 1, MAX_MATRIX_SIZE);
    cols = GetChoice("Columns: ", 1, MAX_MATRIX_SIZE);
}

inline void GetSizesForMultiplication(size_t& rows1, size_t& cols1, size_t& rows2, size_t& cols2) {
    std::cout << "\nFor multiplication A * B:\n";
    std::cout << "Matrix A size:\n";
    rows1 = GetChoice("Rows for A: ", 1, MAX_MATRIX_SIZE);
    cols1 = GetChoice("Columns for A: ", 1, MAX_MATRIX_SIZE);

    std::cout << "Matrix B size (must have " << cols1 << " rows):\n";
    rows2 = cols1;
    cols2 = GetChoice("Columns for B: ", 1, MAX_MATRIX_SIZE);
}

template <typename T>
TMatrix<T> CreateMatrix(MatrixType type, size_t rows, size_t cols) {
    return type == MatrixType::OwnMatrices ? TOwnMatrix<T>(rows, cols) : TMatrixGenerator<T>(rows, cols);
}

template <typename T>
void PerformOperation(const TMatrix<T>& A, const TMatrix<T>& B, Operation operation) {
    switch (operation) {
    case Operation::ADD: std::cout << A + B; break;
    case Operation::SUB: std::cout << A - B; break;
    case Operation::MUL: std::cout << A * B; break;
    }
}

template <typename T>
void RunMatrixCalculator() {
    MatrixType matrixType = ChooseMatrixType();
    Operation operation = ChooseOperation();

    size_t rows1, cols1, rows2, cols2;

    if (operation == Operation::MUL) {
        GetSizesForMultiplication(rows1, cols1, rows2, cols2);
    }
    else {
        GetMatrixSize(rows1, cols1);
        rows2 = rows1;
        cols2 = cols1;
    }

    TMatrix<T> A = CreateMatrix<T>(matrixType, rows1, cols1);
    TMatrix<T> B = CreateMatrix<T>(matrixType, rows2, cols2);

    std::cout << "\nMatrix A:\n" << A;
    std::cout << "Matrix B:\n" << B;
    std::cout << "Result:\n"; PerformOperation(A, B, operation);
}

template <typename T>
void RunCalculator() {
    bool continueCalc = true;
    while (continueCalc) {
        RunMatrixCalculator<T>();
        continueCalc = (GetChoice("\nAnother calculation? (1-yes, 2-no): ", 1, 2) == 1);
        std::cout << "\n";
    }
}

#endif