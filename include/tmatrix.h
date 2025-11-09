// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// utmatrix.h - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (21.04.2015)
//
//

#ifndef __TMATRIX_H__
#define __TMATRIX_H__

#include <iostream>

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

template <class ValType>
class TVector
{
protected:
    ValType* pVector;
    int Size;
    int StartIndex;
public:
    TVector(int s = 10, int si = 0);
    TVector(const TVector& v);
    ~TVector();
    int GetSize() { return Size; }
    int GetStartIndex() { return StartIndex; }
    ValType& operator[](int pos);
    bool operator==(const TVector& v) const;
    bool operator!=(const TVector& v) const;
    TVector& operator=(const TVector& v);

    TVector  operator+(const ValType& val);
    TVector  operator-(const ValType& val);
    TVector  operator*(const ValType& val);

    TVector  operator+(const TVector& v);
    TVector  operator-(const TVector& v);
    ValType  operator*(const TVector& v);

    friend istream& operator>>(istream& in, TVector& v)
    {
        for (int i = 0; i < v.Size; i++)
            in >> v.pVector[i];
        return in;
    }
    friend ostream& operator<<(ostream& out, const TVector& v)
    {
        for (int i = 0; i < v.Size; i++)
            out << v.pVector[i] << ' ';
        return out;
    }
};

template <class ValType>
TVector<ValType>::TVector(int s, int si)
{
    if (s < 1 || s > MAX_VECTOR_SIZE)
        throw "Invalid vector size";
    if (si < 0)
        throw "Invalid start index";

    Size = s;
    StartIndex = si;
    pVector = new ValType[Size]();
}

template <class ValType>
TVector<ValType>::TVector(const TVector<ValType>& v)
{
    Size = v.Size;
    StartIndex = v.StartIndex;
    pVector = new ValType[Size];

    for (int i = 0; i < Size; i++)
        pVector[i] = v.pVector[i];
}

template <class ValType>
TVector<ValType>::~TVector()
{
    delete[] pVector;
}

template <class ValType>
ValType& TVector<ValType>::operator[](int pos)
{
    if (pos - StartIndex < 0 || pos - StartIndex >= Size)
        throw "Index out of range";

    return pVector[pos - StartIndex];
}

template <class ValType>
bool TVector<ValType>::operator==(const TVector& v) const
{
    if (Size != v.Size)
        return false;
    if (StartIndex != v.StartIndex)
        return false;

    for (int i = 0; i < Size; i++)
    {
        if (pVector[i] != v.pVector[i])
            return false;
    }

    return true;
}

template <class ValType>
bool TVector<ValType>::operator!=(const TVector& v) const
{
    return !(*this == v);
}

template <class ValType>
TVector<ValType>& TVector<ValType>::operator=(const TVector& v)
{
    if (this == &v)
        return *this;

    if (Size != v.Size)
    {
        delete[] pVector;
        Size = v.Size;
        pVector = new ValType[Size];
    }

    StartIndex = v.StartIndex;

    for (int i = 0; i < Size; i++)
        pVector[i] = v.pVector[i];

    return *this;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator+(const ValType& val)
{
    TVector<ValType> result(*this);

    for (int i = 0; i < Size; i++)
        result.pVector[i] = result.pVector[i] + val;

    return result;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator-(const ValType& val)
{
    TVector<ValType> result(*this);

    for (int i = 0; i < Size; i++)
        result.pVector[i] = result.pVector[i] - val;

    return result;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator*(const ValType& val)
{
    TVector<ValType> result(*this);

    for (int i = 0; i < Size; i++)
        result.pVector[i] = result.pVector[i] * val;

    return result;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator+(const TVector<ValType>& v)
{
    if (Size != v.Size)
        throw "Vectors have different sizes";

    TVector<ValType> result(*this);

    for (int i = 0; i < Size; i++)
        result.pVector[i] = pVector[i] + v.pVector[i];

    return result;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator-(const TVector<ValType>& v)
{
    if (Size != v.Size)
        throw "Vectors have different sizes";

    TVector<ValType> result(*this);

    for (int i = 0; i < Size; i++)
        result.pVector[i] = pVector[i] - v.pVector[i];

    return result;
}

template <class ValType>
ValType TVector<ValType>::operator*(const TVector<ValType>& v)
{
    if (Size != v.Size)
        throw "Vectors have different sizes";

    ValType result = 0;

    for (int i = 0; i < Size; i++)
        result = result + pVector[i] * v.pVector[i];

    return result;
}


template <class ValType>
class TMatrix : public TVector<TVector<ValType> >
{
public:
    TMatrix(int s = 10);
    TMatrix(const TMatrix& mt);
    TMatrix(const TVector<TVector<ValType> >& mt);
    bool operator==(const TMatrix& mt) const;
    bool operator!=(const TMatrix& mt) const;
    TMatrix& operator= (const TMatrix& mt);
    TMatrix  operator+ (const TMatrix& mt);
    TMatrix  operator- (const TMatrix& mt);

    friend istream& operator>>(istream& in, TMatrix& mt)
    {
        for (int i = 0; i < mt.Size; i++)
            in >> mt.pVector[i];
        return in;
    }
    friend ostream& operator<<(ostream& out, const TMatrix& mt)
    {
        for (int i = 0; i < mt.Size; i++)
            out << mt.pVector[i] << endl;
        return out;
    }
};

template <class ValType>
TMatrix<ValType>::TMatrix(int s) : TVector<TVector<ValType> >(s)
{
    if (s < 1 || s > MAX_MATRIX_SIZE)
        throw "Invalid matrix size";

    for (int i = 0; i < s; i++)
        this->pVector[i] = TVector<ValType>(s - i, i);
}

template <class ValType>
TMatrix<ValType>::TMatrix(const TMatrix<ValType>& mt) :
    TVector<TVector<ValType> >(mt.Size)
{
    for (int i = 0; i < this->Size; i++)
        this->pVector[i] = mt.pVector[i];
}

template <class ValType>
TMatrix<ValType>::TMatrix(const TVector<TVector<ValType> >& mt) :
    TVector<TVector<ValType> >(mt)
{
}

template <class ValType>
bool TMatrix<ValType>::operator==(const TMatrix<ValType>& mt) const
{
    return TVector<TVector<ValType> >::operator==(mt);
}

template <class ValType>
bool TMatrix<ValType>::operator!=(const TMatrix<ValType>& mt) const
{
    return !(*this == mt);
}

template <class ValType>
TMatrix<ValType>& TMatrix<ValType>::operator=(const TMatrix<ValType>& mt)
{
    TVector<TVector<ValType> >::operator=(mt);
    return *this;
}

template <class ValType>
TMatrix<ValType> TMatrix<ValType>::operator+(const TMatrix<ValType>& mt)
{
    if (this->Size != mt.Size)
        throw "Vectors have different sizes";

    TMatrix<ValType> result(*this);

    for (int i = 0; i < this->Size; i++)
        result.pVector[i] = result.pVector[i] + mt.pVector[i];

    return result;
}

template <class ValType>
TMatrix<ValType> TMatrix<ValType>::operator-(const TMatrix<ValType>& mt)
{
    if (this->Size != mt.Size)
        throw "Vectors have different sizes";

    TMatrix<ValType> result(*this);

    for (int i = 0; i < this->Size; i++)
        result.pVector[i] = result.pVector[i] - mt.pVector[i];

    return result;
}

#endif