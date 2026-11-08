#include "tmatrix.h"

#include <gtest.h>

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
  vector<vector<int>> dense = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  ASSERT_NO_THROW(TCRSMatrix<int> m(dense));
}

TEST(TCRSMatrix, throws_when_create_from_empty_or_inconsistent_dense_matrix)
{
  vector<vector<int>> empty;
  vector<vector<int>> inconsistent = {
    {1, 2, 3},
    {4, 5},
    {6, 7, 8}
  };
  ASSERT_THROW(TCRSMatrix<int> m(empty), out_of_range);
  ASSERT_THROW(TCRSMatrix<int> m(inconsistent), out_of_range);
}

TEST(TCRSMatrix, crs_arrays_are_formed_correctly_from_dense)
{
  vector<vector<int>> dense = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  TCRSMatrix<int> m(dense);
  vector<int> expected_values = {1, 2, 3, 4};
  vector<size_t> expected_col = {0, 2, 0, 1};
  vector<size_t> expected_row = {0, 2, 2, 4};
  ASSERT_EQ(expected_values, m.getValues());
  ASSERT_EQ(expected_col, m.getColIndices());
  ASSERT_EQ(expected_row, m.getRowPtr());
}

TEST(TCRSMatrix, can_convert_crs_matrix_back_to_dense)
{
  vector<vector<int>> dense = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };

  TCRSMatrix<int> m(dense);
  ASSERT_EQ(dense, m.toDense());
}

TEST(TCRSMatrix, can_create_copied_matrix)
{
  TCRSMatrix<int> m(5);

  ASSERT_NO_THROW(TCRSMatrix<int> m1(m));
}

TEST(TCRSMatrix, copied_matrix_is_equal_to_source_one)
{
  vector<vector<int>> dense = {
    {1, 0, 2},
    {0, 3, 0}
  };
  TCRSMatrix<int> m(dense);
  TCRSMatrix<int> m1(m);
}

TEST(TCRSMatrix, copied_matrix_has_its_own_memory)
{
  vector<vector<int>> dense = {
    {1, 0},
    {0, 2}
  };
  TCRSMatrix<int> m(dense);
  TCRSMatrix<int> m1(m);

  vector<int> changed_values = {10, 20};
  vector<int> changed_col = {0, 1};
  vector<size_t> changed_row = {0, 1, 2};

  TCRSMatrix<int> changed(2, 2, changed_values, changed_col, changed_row);
  m1 = changed;
  ASSERT_NE(m, m1);
  ASSERT_EQ(dense, m.toDense());
}

TEST(TCRSMatrix, can_assign_matrix_to_itself)
{
  vector<vector<int>> dense = {
    {1, 0},
    {0, 2}
  };
  TCRSMatrix<int> m(dense);
  m = m;
  ASSERT_EQ(dense, m.toDense());
}

TEST(TCRSMatrix, can_assign_matrices_of_equal_size)
{
  vector<vector<int>> dense1 = {
    {1, 0},
    {0, 2}
  };
  vector<vector<int>> dense2 = {
    {3,4},
    {5, 6}
  };
  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);
  m1 = m2;
  ASSERT_EQ(dense2, m1.toDense());
}

TEST(TCRSMatrix, assign_operator_change_matrix_size)
{
  vector<vector<int>> dense1 = {
    {1, 2},
    {3, 4}
  };
  vector<vector<int>> dense2 = {
    {5, 0, 6},
    {0, 7, 0},
    {8, 9, 10}
  };
  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);
  m1 = m2;
  ASSERT_EQ(3, m1.getRows());
  ASSERT_EQ(3, m1.getCols());
  ASSERT_EQ(dense2, m1.toDense());
}

TEST(TCRSMatrix, can_assign_matrices_of_different_size)
{
  vector<vector<int>> dense1 = {
    {1, 2, 3},
    {4, 5, 6}
  };
  vector<vector<int>> dense2 = {
    {7, 8},
    {9, 10},
    {11, 12}
  };
  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);
  m1 = m2;
  ASSERT_EQ(dense2, m1.toDense());
}

TEST(TCRSMatrix, can_get_rows_cols_and_nnz)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, can_get_nonzero_and_zero_elements_by_at)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, throws_when_get_element_with_negative_or_too_large_index)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, compare_equal_matrices_return_true)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, compare_matrix_with_itself_return_true)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, matrices_with_different_size_are_not_equal)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, matrices_with_same_size_but_different_elements_are_not_equal)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, can_transpose_square_matrix)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, can_transpose_rectangular_matrix)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, double_transpose_returns_original_matrix)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, can_multiply_matrix_by_vector_of_compatible_size)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, cant_multiply_matrix_by_vector_of_incompatible_size)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, multiply_zero_matrix_by_vector_returns_zero_vector)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, can_multiply_square_crs_matrices_of_equal_size)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, can_multiply_rectangular_crs_matrices_of_compatible_size)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, cant_multiply_crs_matrices_with_incompatible_sizes)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, multiply_by_identity_matrix_returns_original_matrix)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, multiply_matrix_with_empty_rows_works_correctly)
{
  ADD_FAILURE();
}

TEST(TDynamicMatrix, cant_subtract_matrixes_with_not_equal_size)
{
  ADD_FAILURE();
}

