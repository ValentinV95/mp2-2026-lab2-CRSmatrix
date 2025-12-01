#include "tmatrix.h"

#include <gtest.h>

TEST(TDynamicMatrix, can_create_matrix_with_positive_length)
{
  ASSERT_NO_THROW(TDynamicMatrix<int> m(5));
}

TEST(TDynamicMatrix, cant_create_too_large_matrix)
{
  ASSERT_ANY_THROW(TDynamicMatrix<int> m(MAX_MATRIX_SIZE + 1));
}

TEST(TDynamicMatrix, throws_when_create_matrix_with_negative_length)
{
  ASSERT_ANY_THROW(TDynamicMatrix<int> m(-5));
}

TEST(TDynamicMatrix, can_create_copied_matrix)
{
  TDynamicMatrix<int> m(5);

  ASSERT_NO_THROW(TDynamicMatrix<int> m1(m));
}

TEST(TDynamicMatrix, copied_matrix_is_equal_to_source_one)
{
	TDynamicMatrix<int> a(10);
	a[0][0] = 1;
	TDynamicMatrix<int> b(a);
	EXPECT_EQ(b[0][0], 1);
}

TEST(TDynamicMatrix, copied_matrix_has_its_own_memory)
{
	TDynamicMatrix<int> a(10);
	a[0][0] = 1;
	TDynamicMatrix<int> b(a);
	b[0][0] = 2;
	EXPECT_EQ(a[0][0], 1);
}

TEST(TDynamicMatrix, can_get_size)
{
	TDynamicMatrix<int> a(10);
	EXPECT_EQ(a.size(), 10);
}

TEST(TDynamicMatrix, can_set_and_get_element)
{
	TDynamicMatrix<int> a(10);
	a[0][0] = 10;
	EXPECT_EQ(a[0][0], 10);
}

TEST(TDynamicMatrix, throws_when_set_element_with_negative_index)
{
	TDynamicMatrix<int> a(10);
	ASSERT_ANY_THROW(a.at(-1).at(0) = 0);
	ASSERT_ANY_THROW(a.at(0).at(-1) = 10);
	ASSERT_ANY_THROW(a.at(-2).at(-3) = 10);
}

TEST(TDynamicMatrix, throws_when_set_element_with_too_large_index)
{
	TDynamicMatrix<int> a(10);
	ASSERT_ANY_THROW(a.at(11).at(0) = 0);
	ASSERT_ANY_THROW(a.at(0).at(24) = 10);
	ASSERT_ANY_THROW(a.at(10).at(10) = 10);
}

TEST(TDynamicMatrix, can_assign_matrix_to_itself)
{
	TDynamicMatrix<int> a(10);
	ASSERT_NO_THROW(a = a);
}

TEST(TDynamicMatrix, can_assign_matrices_of_equal_size)
{
	TDynamicMatrix<int> a(10);
	TDynamicMatrix<int> b(10);
	a[0][0] = 1;
	ASSERT_NO_THROW(b = a);
	EXPECT_EQ(b[0][0], 1);
}

TEST(TDynamicMatrix, assign_operator_change_matrix_size)
{
	TDynamicMatrix<int> a(10);
	TDynamicMatrix<int> b(5);
	a = b;
	EXPECT_EQ(a.size(), 5);
}

TEST(TDynamicMatrix, can_assign_matrices_of_different_size)
{
	TDynamicMatrix<int> a(10);
	TDynamicMatrix<int> b(5);
	a[6][6] = 1;
	ASSERT_NO_THROW(b = a);
	EXPECT_EQ(b[6][6], 1);
}

TEST(TDynamicMatrix, compare_equal_matrices_return_true)
{
	TDynamicMatrix<int> a(5);
	TDynamicMatrix<int> b(5);
	a[2][3] = 1;
	b[2][3] = 1;
	EXPECT_EQ(a == b, 1);
}

TEST(TDynamicMatrix, compare_matrix_with_itself_return_true)
{
	TDynamicMatrix<int> a(5);
	a[2][3] = 1;
	EXPECT_EQ(a == a, 1);
}

TEST(TDynamicMatrix, matrices_with_different_size_are_not_equal)
{
	TDynamicMatrix<int> a(5);
	TDynamicMatrix<int> b(6);
	a[2][3] = 1;
	b[2][3] = 1;
	EXPECT_EQ(a == b, 0);
}

TEST(TDynamicMatrix, can_add_matrices_with_equal_size)
{
	TDynamicMatrix<int> a(5);
	TDynamicMatrix<int> b(5);
	a[2][3] = 1;
	b[2][3] = 1;
	ASSERT_NO_THROW(a = (a + b));
	EXPECT_EQ(a[2][3], 2);
}

TEST(TDynamicMatrix, cant_add_matrices_with_not_equal_size)
{
	TDynamicMatrix<int> a(5);
	TDynamicMatrix<int> b(7);
	a[2][3] = 1;
	b[2][3] = 1;
	ASSERT_ANY_THROW(a + b);
}

TEST(TDynamicMatrix, can_subtract_matrices_with_equal_size)
{
	TDynamicMatrix<int> a(5);
	TDynamicMatrix<int> b(5);
	a[2][3] = 2;
	b[2][3] = 1;
	ASSERT_NO_THROW(a = (a - b));
	EXPECT_EQ(a[2][3], 1);
}

TEST(TDynamicMatrix, cant_subtract_matrixes_with_not_equal_size)
{
	TDynamicMatrix<int> a(5);
	TDynamicMatrix<int> b(7);
	a[2][3] = 1;
	b[2][3] = 1;
	ASSERT_ANY_THROW(a - b);
}

