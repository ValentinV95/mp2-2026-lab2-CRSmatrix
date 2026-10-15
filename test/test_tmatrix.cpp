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
  ADD_FAILURE();
}

TEST(TCRSMatrix, crs_arrays_are_formed_correctly_from_dense)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, can_convert_crs_matrix_back_to_dense)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, can_create_copied_matrix)
{
  TCRSMatrix<int> m(5);

  ASSERT_NO_THROW(TCRSMatrix<int> m1(m));
}

TEST(TCRSMatrix, copied_matrix_is_equal_to_source_one)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, copied_matrix_has_its_own_memory)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, can_assign_matrix_to_itself)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, can_assign_matrices_of_equal_size)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, assign_operator_change_matrix_size)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, can_assign_matrices_of_different_size)
{
  ADD_FAILURE();
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

