
#ifndef __TMATRIX_H__
#define __TMATRIX_H__

#include <iostream>

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;


template <class T>
class TVector
{
protected:
    T* pVector;
    int Size;       
    int StartIndex; 
public:
    TVector(int s = 10, int si = 0);
    TVector(const TVector& v);                
    ~TVector();
    int GetSize() { return Size; } 
    int GetStartIndex() { return StartIndex; } 
    T& operator[](int pos);             
    bool operator==(const TVector& v) const;  
    bool operator!=(const TVector& v) const;  
    TVector& operator=(const TVector& v);     

    
    TVector  operator+(const T& val);   
    TVector  operator-(const T& val);   
    TVector  operator*(const T& val);   

    
    TVector  operator+(const TVector& v);     
    TVector  operator-(const TVector& v);     
    T  operator*(const TVector& v);     

    
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

template <class T>
TVector<T>::TVector(int s, int si)
{
    if (s < 0 || s > MAX_VECTOR_SIZE)
        throw "Invalid vector size";
    if (si < 0)
        throw "Invalid start index";

    Size = s;
    StartIndex = si;
    pVector = new T[Size];
    for (int i = 0; i < Size; i++)
        pVector[i] = T();
}

template <class T> 
TVector<T>::TVector(const TVector<T>& v)
{
    Size = v.Size;
    StartIndex = v.StartIndex;
    pVector = new T[Size];
    for (int i = 0; i < Size; i++)
        pVector[i] = v.pVector[i];
}

template <class T>
TVector<T>::~TVector()
{
    delete[] pVector;
}

template <class T> 
T& TVector<T>::operator[](int pos)
{
    if (pos < StartIndex || pos >= StartIndex + Size)
        throw "Index out of range";
    return pVector[pos - StartIndex];
}

template <class T> 
bool TVector<T>::operator==(const TVector& v) const
{
    if (Size != v.Size || StartIndex != v.StartIndex)
        return false;
    for (int i = 0; i < Size; i++)
        if (pVector[i] != v.pVector[i])
            return false;
    return true;
}

template <class T> 
bool TVector<T>::operator!=(const TVector& v) const
{
    return !(*this == v);
}

template <class T> 
TVector<T>& TVector<T>::operator=(const TVector& v)
{
    if (this == &v)
        return *this;

    if (Size != v.Size)
    {
        delete[] pVector;
        Size = v.Size;
        pVector = new T[Size];
    }

    StartIndex = v.StartIndex;
    for (int i = 0; i < Size; i++)
        pVector[i] = v.pVector[i];

    return *this;
}

template <class T> 
TVector<T> TVector<T>::operator+(const T& val)
{
    TVector<T> result(Size, StartIndex);
    for (int i = 0; i < Size; i++)
        result.pVector[i] = pVector[i] + val;
    return result;
}

template <class T> 
TVector<T> TVector<T>::operator-(const T& val)
{
    TVector<T> result(Size, StartIndex);
    for (int i = 0; i < Size; i++)
        result.pVector[i] = pVector[i] - val;
    return result;
}

template <class T> 
TVector<T> TVector<T>::operator*(const T& val)
{
    TVector<T> result(Size, StartIndex);
    for (int i = 0; i < Size; i++)
        result.pVector[i] = pVector[i] * val;
    return result;
}

template <class T> 
TVector<T> TVector<T>::operator+(const TVector<T>& v)
{
    if (Size != v.Size)
        throw "Different vector sizes";

    TVector<T> result(Size, StartIndex);
    for (int i = 0; i < Size; i++)
        result.pVector[i] = pVector[i] + v.pVector[i];

    return result;
}

template <class T> 
TVector<T> TVector<T>::operator-(const TVector<T>& v)
{
    if (Size != v.Size)
        throw "Different vector sizes";

    TVector<T> result(Size, StartIndex);
    for (int i = 0; i < Size; i++)
        result.pVector[i] = pVector[i] - v.pVector[i];

    return result;
}

template <class T> 
T TVector<T>::operator*(const TVector<T>& v)
{
    if (Size != v.Size)
        throw "Different vector sizes";

    T result = T();
    for (int i = 0; i < Size; i++)
        result += pVector[i] * v.pVector[i];

    return result;
}



template <class T>
class TMatrix : public TVector<TVector<T> >
{
public:
    TMatrix(int s = 10);
    TMatrix(const TMatrix& mt);                    
    TMatrix(const TVector<TVector<T> >& mt); 
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

template <class T>
TMatrix<T>::TMatrix(int s) : TVector<TVector<T> >(s)
{
    if (s < 0 || s > MAX_MATRIX_SIZE)
        throw "Invalid matrix size";

    for (int i = 0; i < s; i++)
        this->pVector[i] = TVector<T>(s - i, i);
}

template <class T> 
TMatrix<T>::TMatrix(const TMatrix<T>& mt) :
    TVector<TVector<T> >(mt) {}

template <class T> 
TMatrix<T>::TMatrix(const TVector<TVector<T> >& mt) :
    TVector<TVector<T> >(mt) {}

template <class T> 
bool TMatrix<T>::operator==(const TMatrix<T>& mt) const
{
    return TVector<TVector<T> >::operator==(mt);
}

template <class T> 
bool TMatrix<T>::operator!=(const TMatrix<T>& mt) const
{
    return !(*this == mt);
}

template <class T> 
TMatrix<T>& TMatrix<T>::operator=(const TMatrix<T>& mt)
{
    TVector<TVector<T> >::operator=(mt);
    return *this;
}

template <class T> 
TMatrix<T> TMatrix<T>::operator+(const TMatrix<T>& mt)
{
    return TMatrix<T>(TVector<TVector<T> >::operator+(mt));
}

template <class T> 
TMatrix<T> TMatrix<T>::operator-(const TMatrix<T>& mt)
{
    return TMatrix<T>(TVector<TVector<T> >::operator-(mt));
}

// TVector Î3 Ë2 Ï4 Ñ6
// TMatrix Î2 Ë2 Ï3 Ñ3
#endif