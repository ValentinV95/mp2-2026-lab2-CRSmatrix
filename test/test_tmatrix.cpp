#include "tmatrix.h"
#include <gtest.h>

TEST(DynamicMatrix, CanCreateMatrixWithPositiveSize)
{
    ASSERT_NO_THROW(TDynamicMatrix<int> m(7));
}

TEST(DynamicMatrix, ThrowsExceptionOnTooLargeSize)
{
    ASSERT_ANY_THROW(TDynamicMatrix<int> m(MAX_MATRIX_LEN + 10));
}

TEST(DynamicMatrix, ThrowsExceptionOnZeroSize)
{
    ASSERT_ANY_THROW(TDynamicMatrix<int> m(0));
}

TEST(DynamicMatrix, CanCopyMatrix)
{
    TDynamicMatrix<int> src(5);
    ASSERT_NO_THROW(TDynamicMatrix<int> copy(src));
}

TEST(DynamicMatrix, CopyIsEqualToSource)
{
    TDynamicMatrix<int> m1(5);
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            m1[i][j] = i * j;

    TDynamicMatrix<int> m2(m1);
    EXPECT_EQ(m1, m2);
}

TEST(DynamicMatrix, CopyHasSeparateMemory)
{
    TDynamicMatrix<int> m1(3);
    TDynamicMatrix<int> m2(m1);
    
    m1[0][0] = 100;
    m2[0][0] = 200;

    EXPECT_NE(m1[0][0], m2[0][0]);
}

TEST(DynamicMatrix, GetSizeWorksCorrectly)
{
    TDynamicMatrix<int> m(12);
    EXPECT_EQ(12, m.get_size());
}

TEST(DynamicMatrix, CanAccessElements)
{
    TDynamicMatrix<int> m(4);
    m[1][2] = 55;
    EXPECT_EQ(55, m[1][2]);
}

TEST(DynamicMatrix, ThrowsOnIndexOutOfRange)
{
    TDynamicMatrix<int> m(5);
    ASSERT_ANY_THROW(m[6][0] = 0); 
}

TEST(DynamicMatrix, SelfAssignmentIsValid)
{
    TDynamicMatrix<int> m(5);
    ASSERT_NO_THROW(m = m);
}

TEST(DynamicMatrix, CanAssignMatrices)
{
    TDynamicMatrix<int> m1(5);
    TDynamicMatrix<int> m2(3);
    m1[0][0] = 10;
    
    m2 = m1;
    EXPECT_EQ(m2[0][0], 10);
    EXPECT_EQ(m2.get_size(), 5);
}

TEST(DynamicMatrix, MatricesComparison)
{
    TDynamicMatrix<int> m1(3);
    TDynamicMatrix<int> m2(3);
    
    EXPECT_TRUE(m1 == m2);
    
    m1[0][0] = 1;
    EXPECT_FALSE(m1 == m2);
    EXPECT_TRUE(m1 != m2);
}

TEST(DynamicMatrix, MatrixAddition)
{
    TDynamicMatrix<int> m1(3), m2(3);
    m1[0][0] = 1; m2[0][0] = 2;
    
    TDynamicMatrix<int> res = m1 + m2;
    EXPECT_EQ(res[0][0], 3);
}

TEST(DynamicMatrix, MatrixSubtraction)
{
    TDynamicMatrix<int> m1(3), m2(3);
    m1[0][0] = 10; m2[0][0] = 3;

    TDynamicMatrix<int> res = m1 - m2;
    EXPECT_EQ(res[0][0], 7);
}

TEST(DynamicMatrix, CantAddDifferentSizes)
{
    TDynamicMatrix<int> m1(2), m2(3);
    ASSERT_ANY_THROW(m1 + m2);
}

TEST(DynamicMatrix, MatrixMultiplication)
{
    TDynamicMatrix<int> m1(2), m2(2);
    m1[0][0] = 1; m1[0][1] = 2;
    m1[1][0] = 3; m1[1][1] = 4;
    
    m2[0][0] = 2; m2[0][1] = 0;
    m2[1][0] = 1; m2[1][1] = 2;
    
    TDynamicMatrix<int> res = m1 * m2;
    EXPECT_EQ(res[0][0], 4);
    EXPECT_EQ(res[0][1], 4);
    EXPECT_EQ(res[1][0], 10);
    EXPECT_EQ(res[1][1], 8);
}

TEST(DynamicMatrix, MatrixVectorMultiplication)
{
    TDynamicMatrix<int> m(2);
    TDynamicVector<int> v(2);
    m[0][0] = 1; m[0][1] = 2;
    m[1][0] = 3; m[1][1] = 4;
    v[0] = 1; v[1] = 2;
    
    TDynamicVector<int> res = m * v;
    EXPECT_EQ(res[0], 5);
    EXPECT_EQ(res[1], 11);
}

TEST(DynamicMatrix, ScalarMultiplication)
{
    TDynamicMatrix<int> m(2);
    m[0][0] = 1; m[0][1] = 2;
    m[1][0] = 3; m[1][1] = 4;
    
    TDynamicMatrix<int> res = m * 2;
    EXPECT_EQ(res[0][0], 2);
    EXPECT_EQ(res[0][1], 4);
    EXPECT_EQ(res[1][0], 6);
    EXPECT_EQ(res[1][1], 8);
}
