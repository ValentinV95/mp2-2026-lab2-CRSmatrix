#include "utmatrix.h"

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

TEST(TMatrix, copied_matrix_is_equal_to_source_one)
{
	TMatrix<int> m(5);
	TMatrix<int> m1(m);
	ASSERT_TRUE(m == m1);
}

TEST(TMatrix, copied_matrix_has_its_own_memory)
{
	TMatrix<int> m(5);
	TMatrix<int> m1(m);
	EXPECT_NE(&m, &m1);
}

TEST(TMatrix, can_get_size)
{
	TMatrix<int> m(5);
	int s = m.GetSize();
	EXPECT_EQ(s, 5);
}

TEST(TMatrix, can_set_and_get_element)
{
	TMatrix<int> m(5);
	m[0][1] = 4;

	EXPECT_EQ(4, m[0][1]);

	int a = m[0][1];
	EXPECT_EQ(a, 4);

}

TEST(TMatrix, throws_when_set_element_with_negative_index)
{
	TMatrix<int> m(4);
	ASSERT_ANY_THROW(m[-1][0] = 4);
}

TEST(TMatrix, throws_when_set_element_with_too_large_index)
{
	TMatrix<int> m(4);
	int s = MAX_MATRIX_SIZE;
	EXPECT_ANY_THROW(m[s * s + 1][0] = 4);
}

TEST(TMatrix, can_assign_matrix_to_itself)
{
	TMatrix<int> m(5);
	for (int i = 0; i < 5; i++) {
		for (int j = i; j < 5; j++)
			m[i][j] = 1;
	}
	TMatrix<int> m1(m);
	//m = m;
	m = m1;
	for (int i = 0; i < 5; i++) {
		for (int j = i; j < 5; j++)
			EXPECT_EQ(1, m[i][j]);
	}
}

TEST(TMatrix, can_assign_matrices_of_equal_size)
{
	TMatrix<int> m(5);
	TMatrix<int> m1(5);
	for (int i = 0; i < 5; i++) {
		for (int j = i; j < 5; j++)
			m[i][j] = 1;
	}
	m1 = m;
	for (int i = 0; i < 5; i++) {
		for (int j = i; j < 5; j++)
			EXPECT_EQ(1, m1[i][j]);
	}

}

TEST(TMatrix, assign_operator_change_matrix_size)
{
	TMatrix<int> m(5);
	TMatrix<int> m1(6);
	m1 = m;
	int s = m1.GetSize();
	EXPECT_EQ(5, s);
	TMatrix<int> m2(6);
	m = m2;
	s = m.GetSize();
	EXPECT_EQ(6, s);
}

TEST(TMatrix, can_assign_matrices_of_different_size)
{
	TMatrix<int> m(5);
	TMatrix<int> m1(6);
	for (int i = 0; i < 6; i++) {
		for (int j = i; j < 6; j++)
			m1[i][j] = 1;
	}
	m = m1;
	for (int i = 0; i < 6; i++) {
		for (int j = i; j < 6; j++)
			EXPECT_EQ(1, m[i][j]);
	}
	TMatrix<int> m2(5);
	m1 = m2;
	for (int i = 0; i < 5; i++) {
		for (int j = i; j < 5; j++)
			EXPECT_EQ(0, m1[i][j]);
	}

}

TEST(TMatrix, compare_equal_matrices_return_true)
{
	TMatrix<int> m(5);
	TMatrix<int> m1(5);
	ASSERT_TRUE(m == m1);

}

TEST(TMatrix, compare_matrix_with_itself_return_true)
{
	TMatrix<int> m(5);
	ASSERT_TRUE(m == m);
}

TEST(TMatrix, matrices_with_different_size_are_not_equal)
{
	TMatrix<int> m(5);
	TMatrix<int> m1(6);
	ASSERT_FALSE(m == m1);
}

TEST(TMatrix, can_add_matrices_with_equal_size)
{
	TMatrix<int> m(5);
	TMatrix<int> m1(5);
	for (int i = 0; i < 5; i++) {
		for (int j = i; j < 5; j++)
			m[i][j] = 1;
	}
	for (int i = 0; i < 5; i++) {
		for (int j = i; j < 5; j++)
			m1[i][j] = 2;
	}

	TMatrix<int> m2(5);
	m2 = m + m1;


	TMatrix<int> m3(5);
	for (int i = 0; i < 5; i++) {
		for (int j = i; j < 5; j++)
			m3[i][j] = 3;
	}

	for (int i = 0; i < 5; i++) {
		for (int j = i; j < 5; j++)
			EXPECT_TRUE(m3 == m2);
	}
}

TEST(TMatrix, cant_add_matrices_with_not_equal_size)
{

	TMatrix<int> m(5);
	TMatrix<int> m1(6);
	ASSERT_ANY_THROW(m + m1);
}

TEST(TMatrix, can_subtract_matrices_with_equal_size)
{
	TMatrix<int> m(5);
	TMatrix<int> m1(5);

	for (int i = 0; i < 5; i++) {
		for (int j = i; j < 5; j++)
			m[i][j] = 6;
	}
	for (int i = 0; i < 5; i++) {
		for (int j = i; j < 5; j++)
			m1[i][j] = 2;
	}
	TMatrix<int> m2(5);
	m2 = m - m1;

	TMatrix<int> m3(5);
	for (int i = 0; i < 5; i++) {
		for (int j = i; j < 5; j++)
			m3[i][j] = 4;
	}

	for (int i = 0; i < 5; i++) {
		for (int j = i; j < 5; j++)
			EXPECT_TRUE(m3 == m2);
	}


}

TEST(TMatrix, cant_subtract_matrixes_with_not_equal_size)
{
	TMatrix<int> m(5);
	TMatrix<int> m1(6);
	ASSERT_ANY_THROW(m - m1);

}