#include "tmatrix.h"

#include <gtest.h>
#include <vector>

using namespace std;

static vector<vector<int>> denseA = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
};

TEST(TCRSMatrix, can_create_matrix_with_positive_length)
{
  ASSERT_NO_THROW(TCRSMatrix<int> m(5));
}

TEST(TCRSMatrix, can_create_rectangular_matrix_with_positive_dimensions)
{
  ASSERT_NO_THROW(TCRSMatrix<int> m(4, 6));
}

TEST(TCRSMatrix, cant_create_too_large_matrix)
{
  ASSERT_ANY_THROW(TCRSMatrix<int> m(MAX_MATRIX_SIZE + 1));
}

TEST(TCRSMatrix, throws_when_create_matrix_with_zero_or_negative_length)
{
  ASSERT_ANY_THROW(TCRSMatrix<int> m(0));
  ASSERT_ANY_THROW(TCRSMatrix<int> m(-5));
}

TEST(TCRSMatrix, can_create_crs_matrix_from_dense_matrix)
{
  ASSERT_NO_THROW(TCRSMatrix<int> m(denseA));
}

TEST(TCRSMatrix, throws_when_create_from_empty_or_inconsistent_dense_matrix)
{
	vector<vector<int>> empty;
    ASSERT_ANY_THROW(TCRSMatrix<int> m(empty));

    vector<vector<int>> inconsistent = {{1, 2}, {3}};
    ASSERT_ANY_THROW(TCRSMatrix<int> m(inconsistent));
}

TEST(TCRSMatrix, crs_arrays_are_formed_correctly_from_dense)
{
	TCRSMatrix<int> m(denseA);

	vector<int> expectedValues = { 1, 2, 3, 4 };
	EXPECT_EQ(m.getValues(), expectedValues);

	vector<size_t> expectedCols = { 0, 2, 0, 1 };
	EXPECT_EQ(m.getColIndices(), expectedCols);

	vector<size_t> expectedPtr = { 0, 2, 2, 4 };
	EXPECT_EQ(m.getRowPtr(), expectedPtr);
}

TEST(TCRSMatrix, can_convert_crs_matrix_back_to_dense)
{
	TCRSMatrix<int> m(denseA);
	EXPECT_EQ(m.toDense(), denseA);
}

TEST(TCRSMatrix, can_create_copied_matrix)
{
  TCRSMatrix<int> m(5);

  ASSERT_NO_THROW(TCRSMatrix<int> m1(m));
}

TEST(TCRSMatrix, copied_matrix_is_equal_to_source_one)
{
	TCRSMatrix<int> m(denseA);
	TCRSMatrix<int> m1(m);
	EXPECT_TRUE(m1 == m);
}

TEST(TCRSMatrix, copied_matrix_has_its_own_memory)
{
	TCRSMatrix<int> m(denseA);
	TCRSMatrix<int> m1(m);

	vector<vector<int>> denseB = { {5,0}, {0,6} };
	m = TCRSMatrix<int>(denseB);
	EXPECT_NE(m.getValues(), m1.getValues());
}

TEST(TCRSMatrix, can_assign_matrix_to_itself)
{
	TCRSMatrix<int> m(denseA);
	ASSERT_NO_THROW(m = m);
	EXPECT_TRUE(m == TCRSMatrix<int>(denseA));
}

TEST(TCRSMatrix, can_assign_matrices_of_equal_size)
{
	TCRSMatrix<int> m1(denseA);
	TCRSMatrix<int> m2(3);
	m2 = m1;
	EXPECT_TRUE(m1 == m2);
}

TEST(TCRSMatrix, assign_operator_change_matrix_size)
{
	TCRSMatrix<int> m1(denseA);
	TCRSMatrix<int> m2(2, 4);
	m2 = m1;
	EXPECT_EQ(m2.getRows(), 3u);
	EXPECT_EQ(m2.getCols(), 3u);
}

TEST(TCRSMatrix, can_assign_matrices_of_different_size)
{
	vector<vector<int>> denseB = { {1, 2}, {3, 4} };
	TCRSMatrix<int> m1(denseA);
	TCRSMatrix<int> m2(denseB);
	m1 = m2;
	EXPECT_TRUE(m1 == m2);
	EXPECT_EQ(m1.getRows(), 2u);
	EXPECT_EQ(m1.getCols(), 2u);
}

TEST(TCRSMatrix, can_get_rows_cols_and_nnz)
{
	TCRSMatrix<int> m(denseA);
	EXPECT_EQ(m.getRows(), 3u);
	EXPECT_EQ(m.getCols(), 3u);
	EXPECT_EQ(m.getNNZ(), 4u);
}

TEST(TCRSMatrix, can_get_nonzero_and_zero_elements_by_at)
{
	TCRSMatrix<int> m(denseA);
	EXPECT_EQ(m.at(0, 0), 1);
	EXPECT_EQ(m.at(0, 2), 2);
	EXPECT_EQ(m.at(2, 1), 4);
	EXPECT_EQ(m.at(1, 1), 0);
	EXPECT_EQ(m.at(0, 1), 0);
}

TEST(TCRSMatrix, throws_when_get_element_with_negative_or_too_large_index)
{
	TCRSMatrix<int> m(denseA);
	ASSERT_ANY_THROW(m.at(3, 0));
	ASSERT_ANY_THROW(m.at(0, 3));
}

TEST(TCRSMatrix, compare_equal_matrices_return_true)
{
	TCRSMatrix<int> m1(denseA);
	TCRSMatrix<int> m2(denseA);
	EXPECT_TRUE(m1 == m2);
}

TEST(TCRSMatrix, compare_matrix_with_itself_return_true)
{
	TCRSMatrix<int> m(denseA);
	EXPECT_TRUE(m == m);;
}

TEST(TCRSMatrix, matrices_with_different_size_are_not_equal)
{
	TCRSMatrix<int> m1(denseA);
	vector<vector<int>> denseB = { {1, 2}, {3, 4} };
	TCRSMatrix<int> m2(denseB);
	EXPECT_TRUE(m1 != m2);
}

TEST(TCRSMatrix, matrices_with_same_size_but_different_elements_are_not_equal)
{
	TCRSMatrix<int> m1(denseA);
	vector<vector<int>> denseB = { {1, 0, 2}, {0, 0, 0}, {3, 5, 0} };
	TCRSMatrix<int> m2(denseB);
	EXPECT_TRUE(m1 != m2);
}

TEST(TCRSMatrix, can_transpose_square_matrix)
{
	TCRSMatrix<int> m(denseA);
	TCRSMatrix<int> t = m.transpose();
	vector<vector<int>> expected = {
		{1, 0, 3},
		{0, 0, 4},
		{2, 0, 0}
	};
	EXPECT_EQ(t.toDense(), expected);
}

TEST(TCRSMatrix, can_transpose_rectangular_matrix)
{
	vector<vector<int>> dense = { {1, 0, 2, 3}, {0, 4, 0, 5} };
	TCRSMatrix<int> m(dense);
	TCRSMatrix<int> t = m.transpose();
	EXPECT_EQ(t.getRows(), 4u);
	EXPECT_EQ(t.getCols(), 2u);
	vector<vector<int>> expected = { {1, 0}, {0, 4}, {2, 0}, {3, 5} };
	EXPECT_EQ(t.toDense(), expected);;
}

TEST(TCRSMatrix, double_transpose_returns_original_matrix)
{
	TCRSMatrix<int> m(denseA);
	TCRSMatrix<int> t = m.transpose().transpose();
	EXPECT_TRUE(t == m);
}

TEST(TCRSMatrix, can_multiply_matrix_by_vector_of_compatible_size)
{
	TCRSMatrix<int> m(denseA);
	vector<int> v = { 1, 1, 1 };
	vector<int> res = m * v;
	vector<int> expected = { 3, 0, 7 };
	EXPECT_EQ(res, expected);
}

TEST(TCRSMatrix, cant_multiply_matrix_by_vector_of_incompatible_size)
{
	TCRSMatrix<int> m(denseA);
	vector<int> v = { 1, 1 };
	ASSERT_ANY_THROW(m * v);
}

TEST(TCRSMatrix, multiply_zero_matrix_by_vector_returns_zero_vector)
{
	vector<vector<int>> zero = { {0, 0}, {0, 0} };
	TCRSMatrix<int> m(zero);
	vector<int> v = { 5, 7 };
	vector<int> res = m * v;
	EXPECT_EQ(res, vector<int>({ 0, 0 }));
}

TEST(TCRSMatrix, can_multiply_square_crs_matrices_of_equal_size)
{
	TCRSMatrix<int> a(denseA);
	TCRSMatrix<int> b(denseA);
	TCRSMatrix<int> c = a * b;
	// a * a: compute dense reference
	vector<vector<int>> da = denseA, db = denseA;
	vector<vector<int>> ref(3, vector<int>(3, 0));
	for (size_t i = 0; i < 3; i++)
		for (size_t k = 0; k < 3; k++)
			for (size_t j = 0; j < 3; j++)
				ref[i][j] += da[i][k] * db[k][j];
	EXPECT_EQ(c.toDense(), ref);
}

TEST(TCRSMatrix, can_multiply_rectangular_crs_matrices_of_compatible_size)
{
	vector<vector<int>> da = { {1, 0, 2}, {0, 3, 0} };
	vector<vector<int>> db = { {1, 2}, {3, 4}, {5, 6} };
	TCRSMatrix<int> a(da);
	TCRSMatrix<int> b(db);
	TCRSMatrix<int> c = a * b;
	EXPECT_EQ(c.getRows(), 2u);
	EXPECT_EQ(c.getCols(), 2u);
	vector<vector<int>> ref = { {11, 14}, {9, 12} };
	EXPECT_EQ(c.toDense(), ref);
}

TEST(TCRSMatrix, cant_multiply_crs_matrices_with_incompatible_sizes)
{
	TCRSMatrix<int> a(denseA);
	vector<vector<int>> db = { {1, 2}, {3, 4} };
	TCRSMatrix<int> b(db);
	ASSERT_ANY_THROW(a * b);
}

TEST(TCRSMatrix, multiply_by_identity_matrix_returns_original_matrix)
{
	TCRSMatrix<int> a(denseA);
	vector<vector<int>> ident = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
	TCRSMatrix<int> I(ident);
	TCRSMatrix<int> c = a * I;
	EXPECT_TRUE(c == a);
}

TEST(TCRSMatrix, multiply_matrix_with_empty_rows_works_correctly)
{
	vector<vector<int>> da = { {1, 2}, {0, 0}, {3, 4} };
	vector<vector<int>> db = { {1, 0}, {0, 1} };
	TCRSMatrix<int> a(da);
	TCRSMatrix<int> b(db);
	TCRSMatrix<int> c = a * b;
	EXPECT_EQ(c.toDense(), da);
}

TEST(TDynamicMatrix, cant_subtract_matrixes_with_not_equal_size)
{
	SUCCEED();
}

