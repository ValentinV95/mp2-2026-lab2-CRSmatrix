#include "tmatrix.h"
#include <gtest.h>

const size_t size = 5;

TEST(TVector, can_create_vector_with_positive_length)
{
    ASSERT_NO_THROW(TVector<int> v(size));
}

TEST(TVector, cant_create_too_large_vector)
{
    ASSERT_ANY_THROW(TVector<int> v(MAX_VECTOR_SIZE + 1));
}

TEST(TVector, throws_when_create_vector_with_zero_size)
{
    ASSERT_ANY_THROW(TVector<int> v(0));
}

TEST(TVector, can_create_copied_vector)
{
    TVector<int> v(size);
    ASSERT_NO_THROW(TVector<int> v1(v));
}

TEST(TVector, can_create_vector_from_array)
{
    int arr[] = { 1, 2, 3, 4, 5 };
    TVector<int> v(arr, size);

    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(v[i], arr[i]);
    }
}

TEST(TVector, copied_vector_is_equal_to_source_one)
{
    TVector<int> v(size);
    for (size_t i = 0; i < size; ++i) {
        v[i] = i + 1;
    }
    TVector<int> v1(v);

    EXPECT_EQ(v, v1);
    EXPECT_EQ(v.size(), v1.size());
}

TEST(TVector, copied_vector_has_its_own_memory)
{
    TVector<int> v(size);
    for (size_t i = 0; i < size; ++i) {
        v[i] = (i + 1) * 10;
    }
    TVector<int> v1(v);

    EXPECT_NE(&v[0], &v1[0]);

    v1[0] = 20;
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v1[0], 20);

    v[1] = 200;
    EXPECT_EQ(v[1], 200);
    EXPECT_EQ(v1[1], 20);
}

TEST(TVector, can_move_assign_vector)
{
    TVector<int> v(size), v1(size);
    for (size_t i = 0; i < size; ++i) {
        v[i] = i + 1;
    }
    v1 = std::move(v);

    EXPECT_EQ(v1.size(), size);
    for (size_t i = 0; i < size; ++i)
        EXPECT_EQ(v1[i], i + 1);
    EXPECT_EQ(v.size(), 0); 
}

TEST(TMatrix, can_move_construct_vector)
{
    TVector<int> v(size);
    for (size_t i = 0; i < size; ++i) {
            v[i] = i + 1;
    }

    TVector<int> v1(std::move(v));

    EXPECT_EQ(v1.size(), size);
    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(v1[i], i + 1);
    }
}

TEST(TVector, can_get_size)
{
    TVector<int> v(size);
    EXPECT_EQ(size, v.size());
}

TEST(TVector, can_set_and_get_element)
{
    TVector<int> v(size);
    v[0] = 4;
    EXPECT_EQ(4, v[0]);
}

TEST(TVector, throws_when_set_element_with_negative_index)
{
    TVector<int> v(size);
    ASSERT_ANY_THROW(v.at(-1) = 1);
}

TEST(TVector, throws_when_set_element_with_too_large_index)
{
    TVector<int> v(size);
    ASSERT_ANY_THROW(v.at(size) = 1);
}

TEST(TVector, can_assign_vector_to_itself)
{
    TVector<int> v(size);
    for (size_t i = 0; i < size; ++i) {
        v[i] = i + 1;
    }

    v = v;

    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(v[i], i + 1);
    }
}

TEST(TVector, can_assign_vectors_of_equal_size)
{
    TVector<int> v(size), v1(size);
    for (size_t i = 0; i < size; ++i) {
        v[i] = i + 1;
    }
    v1 = v;

    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(v1[i], i + 1);
    }
}

TEST(TVector, assign_operator_change_vector_size)
{
    TVector<int> v(size), v1(size - 1);
    v1 = v;
    EXPECT_EQ(v1.size(), v.size());
}

TEST(TVector, can_assign_vectors_of_different_size)
{
    TVector<int> v(size), v1(size - 1);
    v1 = v;
    EXPECT_EQ(v1.size(), v.size());
    EXPECT_EQ(v1, v);

    TVector<int> v2(size - 1), v3(size);
    v3 = v2;
    EXPECT_EQ(v3.size(), v2.size());
    EXPECT_EQ(v3, v2);
}

TEST(TVector, compare_equal_vectors_return_true)
{
    TVector<int> v(size), v1(size);
    for (size_t i = 0; i < size; ++i) {
        v[i] = i + 1;
    }
    v1 = v;
    EXPECT_TRUE(v == v1);
}

TEST(TVector, compare_vector_with_itself_return_true)
{
    TVector<int> v(size);
    for (size_t i = 0; i < size; ++i) {
        v[i] = i + 1;
    }
    EXPECT_TRUE(v == v);
}

TEST(TVector, vectors_with_different_size_are_not_equal)
{
    TVector<int> v(size - 1), v1(size);
    EXPECT_FALSE(v == v1);
}

TEST(TVector, can_add_scalar_to_vector)
{
    TVector<int> v(size);
    for (size_t i = 0; i < size; ++i) {
        v[i] = i + 1;
    }
    int scalar = 5;

    TVector<int> res = v + scalar;

    EXPECT_EQ(res.size(), size);
    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(res[i], i + 6);
    }
    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(v[i], i + 1);
    }
}

TEST(TVector, can_sub_scalar_from_vector)
{
    TVector<int> v(size);
    for (size_t i = 0; i < size; ++i) {
        v[i] = i + 1;
    }
    int scalar = 5;

    TVector<int> res = v - scalar;

    EXPECT_EQ(res.size(), size);
    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(res[i], i - 4);
    }
    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(v[i], i + 1);
    }
}

TEST(TVector, can_mul_scalar_by_vector)
{
    TVector<int> v(size);
    for (size_t i = 0; i < size; ++i) {
        v[i] = i + 1;
    }
    int scalar = 5;

    TVector<int> res = v * scalar;

    EXPECT_EQ(res.size(), size);
    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(res[i], (i + 1) * scalar);
    }
    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(v[i], i + 1);
    }
}

TEST(TVector, can_add_vectors_with_equal_size)
{
    TVector<int> v(size), v1(size);
    for (size_t i = 0; i < size; ++i) {
        v[i] = i + 1;
        v1[i] = i + 2;
    }
    TVector<int> res = v + v1;
    EXPECT_EQ(res.size(), size);
    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(res[i], v[i] + v1[i]);
    }
}

TEST(TVector, cant_add_vectors_with_not_equal_size)
{
    TVector<int> v(size), v1(size + 1);
    ASSERT_ANY_THROW(v + v1);
}

TEST(TVector, can_sub_vectors_with_equal_size)
{
    TVector<int> v(size), v1(size);
    for (size_t i = 0; i < size; ++i) {
        v[i] = i + 1;
        v1[i] = i + 2;
    }
    TVector<int> res = v - v1;
    EXPECT_EQ(res.size(), size);
    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(res[i], v[i] - v1[i]);
    }
}

TEST(TVector, cant_sub_vectors_with_not_equal_size)
{
    TVector<int> v(size), v1(size + 1);
    ASSERT_ANY_THROW(v - v1);
}

TEST(TVector, can_mul_vectors_with_equal_size)
{
    TVector<int> v(size), v1(size);
    for (size_t i = 0; i < size; ++i) {
        v[i] = i + 1;
        v1[i] = i + 2;
    }
    int res = v * v1;
    int sum = 0;
    for (size_t i = 0; i < size; ++i) {
        sum += v[i] * v1[i];
    }
    EXPECT_EQ(res, sum);
}

TEST(TVector, cant_mul_vectors_with_not_equal_size)
{
    TVector<int> v(size), v1(size + 1);
    ASSERT_ANY_THROW(v * v1);
}