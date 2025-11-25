#include "tmatrix.h"
#include <gtest.h>

TEST(TVector, can_create_vector_with_positive_length)
{
  ASSERT_NO_THROW(TVector<int> v(5));
}

TEST(TVector, cant_create_too_large_vector)
{
  ASSERT_ANY_THROW(TVector<int> v(MAX_VECTOR_SIZE + 1));
}

TEST(TVector, throws_when_create_vector_with_negative_length)
{
  ASSERT_ANY_THROW(TVector<int> v(-5));
}

TEST(TVector, can_create_copied_vector)
{
  TVector<int> v(10);

  ASSERT_NO_THROW(TVector<int> v1(v));
}

TEST(TVector, copied_vector_is_equal_to_source_one) // С этого момента свои тесты
{
  size_t size = 3;
  TVector<int> v(size);
  for (size_t i = 0; i < size; ++i) {
    v[i] = i*2;
  }
  TVector<int> v1(v);

  for (size_t i = 0; i < size; ++i) {
    EXPECT_EQ(v[i], v1[i]);
  }
}

TEST(TVector, copied_vector_has_its_own_memory)
{
  size_t size = 3;
  TVector<int> v(size);
  for (size_t i = 0; i < size; ++i) {
    v[i] = i*2;
  }

  TVector<int> v1(v);

  v1[0] = 20;
  EXPECT_EQ(v[0], 0);

  v[1] = 30;
  EXPECT_EQ(v1[1], 2);
}

TEST(TVector, can_get_size)
{
  size_t size = 3;
  TVector<int> v(size);

  EXPECT_EQ(size, v.size());
}

TEST(TVector, can_set_and_get_element)
{
  size_t size = 3;
  TVector<int> v(size);
  v[0] = 4;

  EXPECT_EQ(4, v[0]);
}

TEST(TVector, throws_when_set_element_with_negative_index)
{
  size_t size = 3;
  TVector<int> v(size);

  ASSERT_ANY_THROW(v.at(-10) = 45);
}

TEST(TVector, throws_when_set_element_with_too_large_index)
{
  size_t size = 3;
  TVector<int> v(size);

  ASSERT_ANY_THROW(v.at(size + 1) = 45);
}

TEST(TVector, can_assign_vector_to_itself)
{
  size_t size = 3;
  TVector<int> v(size);
  for (size_t i = 0; i < size; ++i) {
    v[i] = i*2;
  }

  EXPECT_NO_THROW(v = v);
  for (size_t i = 0; i < size; ++i) {
    EXPECT_EQ(v[i], i*2);
  }
}

TEST(TVector, can_assign_vectors_of_equal_size)
{
  size_t size = 3;
  TVector<int> v(size), v1(size);

  for (size_t i = 0; i < size; ++i) {
    v[i] = i*2;
  }

  EXPECT_NO_THROW(v1 = v);

  for (size_t i = 0; i < size; ++i) {
    EXPECT_EQ(v1[i], i*2);
  }
  
  v1[0] = 20;
  EXPECT_EQ(v[0], 0);

  v[1] = 20;
  EXPECT_EQ(v1[1], 2);
}

TEST(TVector, assign_operator_change_vector_size)
{
  size_t size = 3;
  TVector<int> v(size), v1(size*2);

  EXPECT_NO_THROW(v1 = v);

  EXPECT_EQ(v1.size(), size);
}

TEST(TVector, can_assign_vectors_of_different_size)
{
  size_t size = 3;
  TVector<int> v(size), v1(size*2);

  for (size_t i = 0; i < size; ++i) {
    v[i] = i*2;
  }

  EXPECT_NO_THROW(v1 = v);

  for (size_t i = 0; i < size; ++i) {
    EXPECT_EQ(v1[i], i*2);
  }
  
  v1[0] = 20;
  EXPECT_EQ(v[0], 0);

  v[1] = 20;
  EXPECT_EQ(v1[1], 2);
}

TEST(TVector, compare_equal_vectors_return_true)
{
  size_t size = 3;
  TVector<int> v(size);
  for (size_t i = 0; i < size; ++i) {
    v[i] = i*2;
  }

  TVector<int> v1(v);

  ASSERT_TRUE(v == v1);
}

TEST(TVector, compare_vector_with_itself_return_true)
{
  size_t size = 3;
  TVector<int> v(size);
  ASSERT_TRUE(v == v);
}

TEST(TVector, vectors_with_different_size_are_not_equal)
{
  size_t size = 3;
  TVector<int> v(size), v1(size*2);
  ASSERT_FALSE(v == v1);
}

TEST(TVector, can_add_scalar_to_vector)
{
  size_t size = 3;
  TVector<int> v(size), summ(size);

  ASSERT_NO_THROW(summ = v + 1);
  for (size_t i = 0; i < size; ++i)
  {
    EXPECT_EQ(summ[i], 1);
    EXPECT_EQ(v[i], 0);
  }
}

TEST(TVector, can_subtract_scalar_from_vector)
{
  size_t size = 3;
  TVector<int> v(size), summ(size);

  ASSERT_NO_THROW(summ = v - 1);
  for (size_t i = 0; i < size; ++i)
  {
    EXPECT_EQ(summ[i], -1);
    EXPECT_EQ(v[i], 0);
  }
}

TEST(TVector, can_multiply_scalar_by_vector)
{
  size_t size = 3;
  TVector<int> v(size), mult(size);

  v = v + 1;

  ASSERT_NO_THROW(mult = v * 5);
  for (size_t i = 0; i < size; ++i)
  {
    EXPECT_EQ(mult[i], 5);
    EXPECT_EQ(v[i], 1);
  }
}

TEST(TVector, can_add_vectors_with_equal_size)
{
  size_t size = 3;
  TVector<int> v(size), v1(size), summ(size);

  for (size_t i = 0; i < size; ++i) {
    v[i] = i*2;
    v1[i] = i*4;
  }

  ASSERT_NO_THROW(summ = v + v1);
  for (size_t i = 0; i < size; ++i)
  {
    EXPECT_EQ(summ[i], v[i] + v1[i]);
    EXPECT_EQ(v[i], i*2);
    EXPECT_EQ(v1[i], i*4);
  }
}

TEST(TVector, cant_add_vectors_with_not_equal_size)
{
  size_t size = 3;
  TVector<int> v(size), v1(size*2);

  ASSERT_ANY_THROW(v + v1);
}

TEST(TVector, can_subtract_vectors_with_equal_size)
{
  size_t size = 3;
  TVector<int> v(size), v1(size), summ(size);

  for (size_t i = 0; i < size; ++i) {
    v[i] = i*4;
    v1[i] = i*2;
  }

  ASSERT_NO_THROW(summ = v - v1);
  for (size_t i = 0; i < size; ++i)
  {
    EXPECT_EQ(summ[i], v[i] - v1[i]);
    EXPECT_EQ(v[i], i*4);
    EXPECT_EQ(v1[i], i*2);
  }
}

TEST(TVector, cant_subtract_vectors_with_not_equal_size)
{
  size_t size = 3;
  TVector<int> v(size), v1(size*2);

  ASSERT_ANY_THROW(v - v1);
}

TEST(TVector, can_multiply_vectors_with_equal_size)
{
  size_t size = 3;
  TVector<int> v(size), v1(size);
  int check = 0, smult;

  for (size_t i = 0; i < size; ++i) {
    v[i] = i*2;
    v1[i] = i*4;
  }

  TVector<int> copy(v);
  TVector<int> copy1(v1);

  for (size_t i = 0; i < size; ++i)
  {
    check += v[i] * v1[i];
  }
  
  ASSERT_NO_THROW(smult = v * v1);
  EXPECT_EQ(smult, check);
  EXPECT_EQ(v, copy);
  EXPECT_EQ(v1, copy1);
}

TEST(TVector, cant_multiply_vectors_with_not_equal_size)
{
  size_t size = 3;
  TVector<int> v(size), v1(size*2);

  ASSERT_ANY_THROW(v * v1);
}