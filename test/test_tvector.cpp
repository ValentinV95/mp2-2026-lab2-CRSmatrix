#include "tmatrix.h"
#include <gtest.h>

TEST(DynamicVector, CreateVectorPositive)
{
    ASSERT_NO_THROW(TDynamicVector<int> v(10));
}

TEST(DynamicVector, CreateVectorTooLarge)
{
    ASSERT_ANY_THROW(TDynamicVector<int> v(MAX_VECTOR_LEN + 1));
}

TEST(DynamicVector, CopyConstructorWorks)
{
    TDynamicVector<int> v(5);
    v[0] = 99;
    TDynamicVector<int> copy(v);
    
    EXPECT_EQ(copy[0], 99);
}

TEST(DynamicVector, CopyIsDeep)
{
    TDynamicVector<int> v1(5);
    TDynamicVector<int> v2(v1);
    
    v1[1] = 10;
    v2[1] = 20;
    
    EXPECT_NE(v1[1], v2[1]);
}

TEST(DynamicVector, GetSizeTest)
{
    TDynamicVector<int> v(7);
    EXPECT_EQ(7, v.length());
}

TEST(DynamicVector, AccessBoundaryCheck)
{
    TDynamicVector<int> v(5);
    ASSERT_ANY_THROW(v.at(10));
}

TEST(DynamicVector, AssignmentOperator)
{
    TDynamicVector<int> v1(3), v2(5);
    v1[0] = 7;
    v2 = v1;
    
    EXPECT_EQ(v2.length(), 3);
    EXPECT_EQ(v2[0], 7);
}

TEST(DynamicVector, ComparisonEqual)
{
    TDynamicVector<int> v1(4), v2(4);
    EXPECT_TRUE(v1 == v2);
}

TEST(DynamicVector, ComparisonNotEqual)
{
    TDynamicVector<int> v1(4), v2(4);
    v1[0] = 5;
    EXPECT_TRUE(v1 != v2);
}

TEST(DynamicVector, ScalarOperations)
{
    TDynamicVector<int> v(3);
    v[0] = 1;
    
    v = v + 5;
    EXPECT_EQ(v[0], 6);
    
    v = v - 2;
    EXPECT_EQ(v[0], 4);
    
    v = v * 2;
    EXPECT_EQ(v[0], 8);
}

TEST(DynamicVector, VectorAddition)
{
    TDynamicVector<int> v1(3), v2(3);
    v1[0] = 1; v2[0] = 2;
    
    TDynamicVector<int> sum = v1 + v2;
    EXPECT_EQ(sum[0], 3);
}

TEST(DynamicVector, VectorSubtraction)
{
    TDynamicVector<int> v1(3), v2(3);
    v1[0] = 5; v2[0] = 2;

    TDynamicVector<int> diff = v1 - v2;
    EXPECT_EQ(diff[0], 3);
}

TEST(DynamicVector, DotProduct)
{
    TDynamicVector<int> v1(2), v2(2);
    v1[0] = 1; v1[1] = 1;
    v2[0] = 2; v2[1] = 2;
    
    int res = v1 * v2;
    EXPECT_EQ(res, 4);
}

TEST(DynamicVector, OperationsThrowOnMismatch)
{
    TDynamicVector<int> v1(2), v2(3);
    ASSERT_ANY_THROW(v1 + v2);
    ASSERT_ANY_THROW(v1 - v2);
    ASSERT_ANY_THROW(v1 * v2);
}
