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
	TDynamicMatrix<int> m1(4);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m1[i][j] = i + j;
		}
	}
	TDynamicMatrix<int> m2(m1);
	EXPECT_EQ(m2[1][0], m1[1][0]);
}

TEST(TDynamicMatrix, copied_matrix_has_its_own_memory)
{
	TDynamicMatrix<int> m1(4);
	TDynamicMatrix<int> m2(m1);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m1[i][j] = i + j;
		}
	}
	EXPECT_NE(m2[2][0], m1[2][0]);
}


TEST(TDynamicMatrix, can_get_size)
{
	TDynamicMatrix<int> m(5);

	EXPECT_EQ(5,m.get_size());
}

TEST(TDynamicMatrix, can_set_and_get_element)
{
	TDynamicMatrix<int> m(5);
	m[1][1] = 1;
	EXPECT_EQ(1, m[1][1]);
}

TEST(TDynamicMatrix, throws_when_set_element_with_negative_index)
{
	TDynamicMatrix<int> m(5);
	ASSERT_ANY_THROW(m[-1][1] = 1);
}

TEST(TDynamicMatrix, throws_when_set_element_with_too_large_index)
{
	TDynamicMatrix<int> m(MAX_MATRIX_SIZE-1);
	ASSERT_ANY_THROW(m[1][MAX_MATRIX_SIZE+1] = 1);
}

TEST(TDynamicMatrix, can_assign_matrix_to_itself)
{
	TDynamicMatrix<int> m(MAX_MATRIX_SIZE-1);
	ASSERT_NO_THROW(m = m);
}

TEST(TDynamicMatrix, can_assign_matrices_of_equal_size)
{
	TDynamicMatrix<int> m(MAX_MATRIX_SIZE-1);
	TDynamicMatrix<int> m2(MAX_MATRIX_SIZE-1);
	ASSERT_NO_THROW(m = m2);
}

TEST(TDynamicMatrix, assign_operator_change_matrix_size)
{
		TDynamicMatrix<int> m(1);
		TDynamicMatrix<int> m2(MAX_MATRIX_SIZE-1);
		m2 = m;
	EXPECT_EQ(1, m2.get_size());
}

TEST(TDynamicMatrix, can_assign_matrices_of_different_size)
{
	TDynamicMatrix<int> m(1);
	TDynamicMatrix<int> m2(MAX_MATRIX_SIZE-1);
	m2 = m;
	EXPECT_EQ(1, m2.get_size());
}

TEST(TDynamicMatrix, compare_equal_matrices_return_true)
{
	TDynamicMatrix<int> m(1);
	TDynamicMatrix<int> m2(MAX_MATRIX_SIZE-1);
	m2 = m;
	EXPECT_EQ(m== m2,1);
}

TEST(TDynamicMatrix, compare_matrix_with_itself_return_true)
{
	TDynamicMatrix<int> m(1);
	EXPECT_EQ(m == m, 1);
}

TEST(TDynamicMatrix, matrices_with_different_size_are_not_equal)
{
	TDynamicMatrix<int> m(1);
	TDynamicMatrix<int> m2(MAX_MATRIX_SIZE-1);
	EXPECT_EQ(m == m2, 0);
}

TEST(TDynamicMatrix, can_add_matrices_with_equal_size)
{
	TDynamicMatrix<int> m(3);
	TDynamicMatrix<int> m2(3);
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			m[i][j] = i + j;
			m2[i][j] = i - j;
		}
	}

	ASSERT_NO_THROW(m + m2);
}

TEST(TDynamicMatrix, cant_add_matrices_with_not_equal_size)
{
	TDynamicMatrix<int> m(3);
	TDynamicMatrix<int> m2(5);
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			m[i][j] = i + j;
			m2[i][j] = i - j;
		}
	}

	ASSERT_ANY_THROW(m + m2);
}

TEST(TDynamicMatrix, can_subtract_matrices_with_equal_size)
{
	TDynamicMatrix<int> m(3);
	TDynamicMatrix<int> m2(3);
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			m[i][j] = i + j;
			m2[i][j] = i - j;
		}
	}

	ASSERT_NO_THROW(m - m2);
}

TEST(TDynamicMatrix, cant_subtract_matrixes_with_not_equal_size)
{
	TDynamicMatrix<int> m(3);
	TDynamicMatrix<int> m2(5);
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			m[i][j] = i + j;
			m2[i][j] = i - j;
		}
	}

	ASSERT_ANY_THROW(m - m2);
}

