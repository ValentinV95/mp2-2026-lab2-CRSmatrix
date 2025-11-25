#include "tmatrix.h"

#include <gtest.h>

TEST(TMatrix, can_create_matrix_with_positive_length)
{
  ASSERT_NO_THROW(TMatrix<int> m(5));
}

TEST(TMatrix, cant_create_too_large_matrix)
{
  ASSERT_ANY_THROW(TMatrix<int> m(MAX_MATRIX_SIZE + 1));
}

TEST(TMatrix, throws_when_create_matrix_with_negative_length)
{
  ASSERT_ANY_THROW(TMatrix<int> m(-5));
}

TEST(TMatrix, can_create_copied_matrix)
{
  TMatrix<int> m(5);

  ASSERT_NO_THROW(TMatrix<int> m1(m));
}

TEST(TMatrix, copied_matrix_is_equal_to_source_one) // С этого момента свои тесты
{
  size_t size = 2;
  TMatrix<int> m(size);
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      m[i][j] = i * j;
    }
  }

  TMatrix<int> m1(m);
  for (size_t i = 0; i < size; ++i) {
    EXPECT_EQ(m[i], m1[i]);
  }
}

TEST(TMatrix, copied_matrix_has_its_own_memory)
{
  size_t size = 2;
  TMatrix<int> m(size);
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      m[i][j] = i * j;
    }
  }

  TMatrix<int> m1(m);

  m[0][0] = 20;
  EXPECT_EQ(m1[0][0], 0);

  m1[1][0] = 20;
  EXPECT_EQ(m[1][0], 0);
}

TEST(TMatrix, can_get_size)
{
  size_t size = 2;
  TMatrix<int> m(size);
  EXPECT_EQ(m.size(), size);
}

TEST(TMatrix, can_set_and_get_element)
{
  size_t size = 2;
  TMatrix<int> m(size);

  m[0][0] = 4;
  EXPECT_EQ(m[0][0], 4);
}

TEST(TMatrix, throws_when_set_element_with_negative_index)
{
  size_t size = 2;
  TMatrix<int> m(size);

  ASSERT_ANY_THROW(m.at(-1).at(-1) = 20);
}

TEST(TMatrix, throws_when_set_element_with_too_large_index)
{
  size_t size = 2;
  TMatrix<int> m(size);

  ASSERT_ANY_THROW(m.at(size + 1).at(size + 1) = 20);
}

TEST(TMatrix, can_assign_matrix_to_itself)
{
  size_t size = 2;
  TMatrix<int> m(size);
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      m[i][j] = i * j;
    }
  }

  ASSERT_NO_THROW(m = m);
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      EXPECT_EQ(m[i][j], i * j);
    }
  }
}

TEST(TMatrix, can_assign_matrices_of_equal_size)
{
  size_t size = 2;
  TMatrix<int> m(size), m1(size);
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      m[i][j] = i * j;
    }
  }

  ASSERT_NO_THROW(m1 = m);
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      EXPECT_EQ(m1[i][j], i * j);
    }
  }

  m[0][0] = 20;
  EXPECT_EQ(m1[0][0], 0);

  m1[1][0] = 20;
  EXPECT_EQ(m[1][0], 0);
}

TEST(TMatrix, assign_operator_change_matrix_size)
{
  size_t size = 2;
  TMatrix<int> m(size), m1(size * 2);

  m1 = m;
  EXPECT_EQ(m1.size(), size);
}

TEST(TMatrix, can_assign_matrices_of_different_size)
{
  size_t size = 2;
  TMatrix<int> m(size), m1(size * 2);
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      m[i][j] = i * j;
    }
  }

  ASSERT_NO_THROW(m1 = m);
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      EXPECT_EQ(m1[i][j], i * j);
    }
  }

  m[0][0] = 20;
  EXPECT_EQ(m1[0][0], 0);

  m1[1][0] = 20;
  EXPECT_EQ(m[1][0], 0);
}

TEST(TMatrix, compare_equal_matrices_return_true)
{
  size_t size = 2;
  TMatrix<int> m(size);
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      m[i][j] = i * j;
    }
  }
  TMatrix<int> m1(m);
  ASSERT_TRUE(m1 == m);
}

TEST(TMatrix, compare_matrix_with_itself_return_true)
{
  size_t size = 2;
  TMatrix<int> m(size);
  ASSERT_TRUE(m == m);
}

TEST(TMatrix, matrices_with_different_size_are_not_equal)
{
  size_t size = 2;
  TMatrix<int> m(size), m1(size * 2);
  ASSERT_FALSE(m1 == m);
}

TEST(TMatrix, can_add_matrices_with_equal_size)
{
  size_t size = 2;
  TMatrix<int> m(size), m1(size), summ(size);
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      m[i][j] = i * j;
      m1[i][j] = 2 * i * j;
    }
  }
  ASSERT_NO_THROW(summ = m + m1);
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      EXPECT_EQ(summ[i][j], m[i][j] + m1[i][j]);
      EXPECT_EQ(m[i][j], i * j);
      EXPECT_EQ(m1[i][j], 2 * i * j);
    }
  }
}

TEST(TMatrix, cant_add_matrices_with_not_equal_size)
{
  size_t size = 2;
  TMatrix<int> m(size), m1(size*2);
  ASSERT_ANY_THROW(m + m1);
}

TEST(TMatrix, can_subtract_matrices_with_equal_size)
{
  size_t size = 2;
  TMatrix<int> m(size), m1(size), summ(size);
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      m[i][j] = i * j;
      m1[i][j] = 2 * i * j;
    }
  }
  ASSERT_NO_THROW(summ = m1 - m);
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      EXPECT_EQ(summ[i][j], m1[i][j] - m[i][j]);
      EXPECT_EQ(m[i][j], i * j);
      EXPECT_EQ(m1[i][j], 2 * i * j);
    }
  }
}

TEST(TMatrix, cant_subtract_matrixes_with_not_equal_size)
{
  size_t size = 2;
  TMatrix<int> m(size), m1(size*2);
  ASSERT_ANY_THROW(m - m1);
}

TEST(TMatrix, can_multiply_matrix_and_vector_with_equal_sizes)
{
  size_t size = 2;
  TMatrix<int> m(size);
  TVector<int> v(size), mult(size);
  for (size_t i = 0; i < size; ++i) {
    v[i] = i * 2;
    for (size_t j = 0; j < size; ++j) {
      m[i][j] = i * j;
    }
  }

  ASSERT_NO_THROW(mult = m * v);
  for (size_t i = 0; i < size; ++i) {
    EXPECT_EQ(v[i], i * 2);
    EXPECT_EQ(mult[i], m[i] * v);
    for (size_t j = 0; j < size; ++j) {
      EXPECT_EQ(m[i][j], i * j);
    }
  }
}

TEST(TMatrix, cant_multiply_matrix_and_vector_with_not_equal_sizes)
{
  size_t size = 2;
  TMatrix<int> m(size);
  TVector<int> v(size * 2);
  ASSERT_ANY_THROW(m * v);
}

TEST(TMatrix, can_multiply_matrixes_with_equal_sizes)
{
  size_t size = 2;
  TMatrix<int> m(size), m1(size), mmult(size);
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      m[i][j] = i * j;
      m1[i][j] = 2 * i * j;
    }
  }

  ASSERT_NO_THROW(mmult = m * m1);
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {

      EXPECT_EQ(m[i][j], i * j);
      EXPECT_EQ(m1[i][j], 2 * i * j);
      int elem = int();
      for (size_t k = 0; k < size; ++k)
      {
        elem += m[i][k] * m1[k][j];
      }
      EXPECT_EQ(mmult[i][j], elem);
    }
  }
}

TEST(TMatrix, cant_multiply_matrixes_with_not_equal_sizes)
{
  size_t size = 2;
  TMatrix<int> m(size), m1(size*2);
  ASSERT_ANY_THROW(m * m1);
}