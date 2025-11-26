#ifndef __TDynamicMatrix_H__
#define __TDynamicMatrix_H__

#include <iostream>
#include <cassert>
#include <algorithm>
#include <stdexcept>

using namespace std;

const int MAX_VECTOR_LEN = 100000000;
const int MAX_MATRIX_LEN = 10000;

template<typename T>
class TDynamicVector
{
protected:
    size_t size;
    T* pData;

public:
    TDynamicVector(size_t _size = 1) : size(_size)
    {
        if (size == 0)
            throw out_of_range("Vector size must be greater than 0");
        if (size > MAX_VECTOR_LEN)
            throw out_of_range("Vector size is too large");
        
        pData = new T[size]();
    }

    TDynamicVector(T* arr, size_t _size) : size(_size)
    {
        assert(arr != nullptr && "Source array cannot be null");
        if (size == 0)
            throw out_of_range("Vector size must be greater than 0");
        if (size > MAX_VECTOR_LEN)
            throw out_of_range("Vector size is too large");

        pData = new T[size];
        std::copy(arr, arr + size, pData);
    }

    TDynamicVector(const TDynamicVector& v) : size(v.size)
    {
        pData = new T[size];
        std::copy(v.pData, v.pData + size, pData);
    }

    TDynamicVector(TDynamicVector&& v) noexcept : size(0), pData(nullptr)
    {
        std::swap(size, v.size);
        std::swap(pData, v.pData);
    }

    ~TDynamicVector()
    {
        delete[] pData;
        pData = nullptr;
    }

    TDynamicVector& operator=(const TDynamicVector& v)
    {
        if (this == &v) return *this;

        if (size != v.size) 
        {
            T* newData = new T[v.size];
            delete[] pData;
            pData = newData;
            size = v.size;
        }
        std::copy(v.pData, v.pData + size, pData);
        
        return *this;
    }

    TDynamicVector& operator=(TDynamicVector&& v) noexcept
    {
        if (this == &v) return *this;

        delete[] pData;
        pData = nullptr;
        size = 0;

        std::swap(size, v.size);
        std::swap(pData, v.pData);

        return *this;
    }

    size_t length() const noexcept { return size; }

    T& operator[](size_t ind)
    {
        return pData[ind];
    }

    const T& operator[](size_t ind) const
    {
        return pData[ind];
    }

    T& at(size_t ind)
    {
        if (ind >= size) throw out_of_range("Index is out of range");
        return pData[ind];
    }

    const T& at(size_t ind) const
    {
        if (ind >= size) throw out_of_range("Index is out of range");
        return pData[ind];
    }

    bool operator==(const TDynamicVector& v) const noexcept
    {
        if (size != v.size) return false;
        for (size_t i = 0; i < size; ++i) 
        {
            if (pData[i] != v.pData[i]) return false;
        }
        return true;
    }

    bool operator!=(const TDynamicVector& v) const noexcept
    {
        return !(*this == v);
    }

    TDynamicVector operator+(T val)
    {
        TDynamicVector res(*this);
        for (size_t i = 0; i < size; ++i) res.pData[i] += val;
        return res;
    }

    TDynamicVector operator-(T val)
    {
        TDynamicVector res(*this);
        for (size_t i = 0; i < size; ++i) res.pData[i] -= val;
        return res;
    }

    TDynamicVector operator*(T val)
    {
        TDynamicVector res(*this);
        for (size_t i = 0; i < size; ++i) res.pData[i] *= val;
        return res;
    }

    TDynamicVector operator+(const TDynamicVector& v)
    {
        if (size != v.size) throw length_error("Vector lengths mismatch");
        
        TDynamicVector res(size);
        for (size_t i = 0; i < size; ++i) 
            res.pData[i] = pData[i] + v.pData[i];
        
        return res;
    }

    TDynamicVector operator-(const TDynamicVector& v)
    {
        if (size != v.size) throw length_error("Vector lengths mismatch");

        TDynamicVector res(size);
        for (size_t i = 0; i < size; ++i)
            res.pData[i] = pData[i] - v.pData[i];

        return res;
    }

    T operator*(const TDynamicVector& v)
    {
        if (size != v.size) throw length_error("Vector lengths mismatch");

        T dotProduct = T();
        for (size_t i = 0; i < size; ++i)
            dotProduct += pData[i] * v.pData[i];
        
        return dotProduct;
    }

    friend void swap(TDynamicVector& lhs, TDynamicVector& rhs) noexcept
    {
        std::swap(lhs.size, rhs.size);
        std::swap(lhs.pData, rhs.pData);
    }

    friend istream& operator>>(istream& istr, TDynamicVector& v)
    {
        for (size_t i = 0; i < v.size; ++i) istr >> v.pData[i];
        return istr;
    }

    friend ostream& operator<<(ostream& ostr, const TDynamicVector& v)
    {
        for (size_t i = 0; i < v.size; ++i) ostr << v.pData[i] << ' ';
        return ostr;
    }
};

template<typename T>
class TDynamicMatrix : private TDynamicVector<TDynamicVector<T>>
{
    using Base = TDynamicVector<TDynamicVector<T>>;
    using Base::pData;
    using Base::size;

public:
    TDynamicMatrix(size_t s = 1) : Base(s)
    {
        if (s == 0)
            throw out_of_range("Matrix size must be greater than 0");
        if (s > MAX_MATRIX_LEN)
            throw out_of_range("Matrix size exceeds maximum limit");
        
        for (size_t i = 0; i < size; ++i)
            pData[i] = TDynamicVector<T>(size);
    }

    TDynamicVector<T>& operator[](size_t ind)
    {
        if (ind >= size) throw out_of_range("Matrix index out of range");
        return pData[ind];
    }

    const TDynamicVector<T>& operator[](size_t ind) const
    {
        if (ind >= size) throw out_of_range("Matrix index out of range");
        return pData[ind];
    }

    size_t get_size() const { return size; } 

    bool operator==(const TDynamicMatrix& m) const noexcept
    {
        return Base::operator==(m);
    }

    bool operator!=(const TDynamicMatrix& m) const noexcept
    {
        return !(*this == m);
    }

    TDynamicMatrix<T> operator*(const T& val)
    {
        TDynamicMatrix<T> res(size);
        for (size_t i = 0; i < size; ++i)
        {
            res[i] = pData[i] * val;
        }
        return res;
    }

    TDynamicVector<T> operator*(const TDynamicVector<T>& v)
    {
        if (v.length() != size)
            throw length_error("Vector and Matrix dimensions incompatible");

        TDynamicVector<T> res(size);
        for (size_t i = 0; i < size; ++i)
        {
            res[i] = pData[i] * v;
        }
        return res;
    }

    TDynamicMatrix operator+(const TDynamicMatrix& m)
    {
        if (m.get_size() != size)
            throw length_error("Matrix dimensions mismatch");

        TDynamicMatrix<T> res(size);
        for (size_t i = 0; i < size; ++i)
        {
            res[i] = pData[i] + m[i];
        }
        return res;
    }

    TDynamicMatrix operator-(const TDynamicMatrix& m)
    {
        if (m.get_size() != size)
            throw length_error("Matrix dimensions mismatch");

        TDynamicMatrix<T> res(size);
        for (size_t i = 0; i < size; ++i)
        {
            res[i] = pData[i] - m[i];
        }
        return res;
    }

    TDynamicMatrix operator*(const TDynamicMatrix& m)
    {
        if (m.get_size() != size)
            throw length_error("Matrix dimensions mismatch for multiplication");

        TDynamicMatrix<T> res(size);
        for (size_t i = 0; i < size; ++i) 
        {
            for (size_t j = 0; j < size; ++j) 
            {
                T sum = T();
                for (size_t k = 0; k < size; ++k) 
                {
                    sum += pData[i][k] * m[k][j];
                }
                res[i][j] = sum;
            }
        }
        return res;
    }

    friend istream& operator>>(istream& istr, TDynamicMatrix& m)
    {
        for (size_t i = 0; i < m.size; ++i) istr >> m.pData[i];
        return istr;
    }

    friend ostream& operator<<(ostream& ostr, const TDynamicMatrix& m)
    {
        for (size_t i = 0; i < m.size; ++i) ostr << m.pData[i] << endl;
        return ostr;
    }
};

#endif
