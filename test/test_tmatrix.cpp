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
  vector<vector<int>> empty = {};
  vector<vector<int>> inconsistent = {
    {1, 2, 1},
    {2, 3, 4, 5}
  };

  ASSERT_ANY_THROW(TCRSMatrix<int> m(empty));
  ASSERT_ANY_THROW(TCRSMatrix<int> m(inconsistent));
}

TEST(TCRSMatrix, crs_arrays_are_formed_correctly_from_dense)
{
  vector<vector<int>> dense = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  TCRSMatrix<int> m(dense);

  vector<int> expected_values = {1, 2, 1, 3, 4};
  vector<size_t> expected_col_index = {1, 0, 2, 0, 3};
  vector<size_t> expected_row_ptr = {0, 1, 3, 4, 5};

  EXPECT_EQ(m.getValues(), expected_values);
  EXPECT_EQ(m.getColIndices(), expected_col_index);
  EXPECT_EQ(m.getRowPtr(), expected_row_ptr);
}

TEST(TCRSMatrix, can_convert_crs_matrix_back_to_dense)
{
  vector<vector<int>> dense = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  TCRSMatrix<int> m(dense);

  vector<vector<int>> res = m.toDense();

  EXPECT_EQ(dense, res);
}

TEST(TCRSMatrix, can_create_copied_matrix)
{
  TCRSMatrix<int> m(5);

  ASSERT_NO_THROW(TCRSMatrix<int> m1(m));
}

TEST(TCRSMatrix, copied_matrix_is_equal_to_source_one)
{
  vector<vector<int>> dense = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  TCRSMatrix<int> m(dense);
  TCRSMatrix<int> m_copy(m);

  EXPECT_EQ(m, m_copy);
}

TEST(TCRSMatrix, copied_matrix_has_its_own_memory)
{
  vector<vector<int>> dense = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  TCRSMatrix<int> m(dense);
  TCRSMatrix<int> m_copy(m);

  EXPECT_NE(&m.getValues()[0], &m_copy.getValues()[0]);
  EXPECT_NE(&m.getColIndices()[0], &m_copy.getColIndices()[0]);
  EXPECT_NE(&m.getRowPtr()[0], &m_copy.getRowPtr()[0]);
}

TEST(TCRSMatrix, can_assign_matrix_to_itself)
{
  vector<vector<int>> dense = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  TCRSMatrix<int> m(dense);

  ASSERT_NO_THROW(m = m);
}

TEST(TCRSMatrix, can_assign_matrices_of_equal_size)
{
  vector<vector<int>> dense1 = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  vector<vector<int>> dense2 = {
    {0, 2, 3, 0},
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 0, 4}
  };

  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);

  ASSERT_NO_THROW(m1 = m2);
}

TEST(TCRSMatrix, assign_operator_change_matrix_size)
{
  vector<vector<int>> dense1 = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  vector<vector<int>> dense2 = {
    {1, 0, 1},
    {2, 0, 0},
    {0, 1, 0}
  };

  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);

  m1 = m2;

  EXPECT_EQ(m1.getRows(), 3);
  EXPECT_EQ(m1.getCols(), 3);
}

TEST(TCRSMatrix, can_assign_matrices_of_different_size)
{
  vector<vector<int>> dense1 = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  vector<vector<int>> dense2 = {
    {1, 0, 1},
    {2, 0, 0},
    {0, 1, 0}
  };

  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);

  ASSERT_NO_THROW(m1 = m2);
}

TEST(TCRSMatrix, can_get_rows_cols_and_nnz)
{
  vector<vector<int>> dense = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  TCRSMatrix<int> m(dense);

  EXPECT_EQ(m.getRows(), 4);
  EXPECT_EQ(m.getCols(), 4);
  EXPECT_EQ(m.getNNZ(), 5);
}

TEST(TCRSMatrix, can_get_nonzero_and_zero_elements_by_at)
{
  vector<vector<int>> dense = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  TCRSMatrix<int> m(dense);

  EXPECT_EQ(m.at(0, 1), 1);
  EXPECT_EQ(m.at(1, 1), 0);
}

TEST(TCRSMatrix, throws_when_get_element_with_negative_or_too_large_index)
{
  vector<vector<int>> dense = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  TCRSMatrix<int> m(dense);

  ASSERT_ANY_THROW(m.at(-1, 0));
  ASSERT_ANY_THROW(m.at(0, -3));
  ASSERT_ANY_THROW(m.at(10, 0));
  ASSERT_ANY_THROW(m.at(0, 10));
}

TEST(TCRSMatrix, compare_equal_matrices_return_true)
{
  vector<vector<int>> dense = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  TCRSMatrix<int> m1(dense);
  TCRSMatrix<int> m2(dense);

  EXPECT_TRUE(m1 == m2);
}

TEST(TCRSMatrix, compare_matrix_with_itself_return_true)
{
  vector<vector<int>> dense = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  TCRSMatrix<int> m(dense);

  EXPECT_TRUE(m == m);
}

TEST(TCRSMatrix, matrices_with_different_size_are_not_equal)
{
  vector<vector<int>> dense1 = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  vector<vector<int>> dense2 = {
    {1, 0, 1},
    {2, 0, 0},
    {0, 1, 0}
  };

  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);

  EXPECT_TRUE(m1 != m2);
}

TEST(TCRSMatrix, matrices_with_same_size_but_different_elements_are_not_equal)
{
  vector<vector<int>> dense1 = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  vector<vector<int>> dense2 = {
    {0, 2, 3, 0},
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 0, 4}
  };

  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);

  EXPECT_TRUE(m1 != m2);
}

TEST(TCRSMatrix, can_transpose_square_matrix)
{
  vector<vector<int>> dense1 = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  vector<vector<int>> dense2 = {
    {0, 2, 3, 0},
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 0, 4}
  };

  TCRSMatrix<int> m(dense1);

  TCRSMatrix<int> transposed = m.transpose();

  EXPECT_EQ(transposed.toDense(), dense2);
}

TEST(TCRSMatrix, can_transpose_rectangular_matrix)
{
  vector<vector<int>> dense1 = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0}
  };

  vector<vector<int>> dense2 = {
    {0, 2, 3},
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 0}
  };

  TCRSMatrix<int> m(dense1);
  TCRSMatrix<int> transposed = m.transpose();

  EXPECT_EQ(transposed.toDense(), dense2);
}

TEST(TCRSMatrix, double_transpose_returns_original_matrix)
{
  vector<vector<int>> dense = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  TCRSMatrix<int> m(dense);

  TCRSMatrix<int> transposed1 = m.transpose();
  TCRSMatrix<int> transposed2 = transposed1.transpose();

  EXPECT_EQ(m, transposed2);
}

TEST(TCRSMatrix, can_multiply_matrix_by_vector_of_compatible_size)
{
  vector<vector<int>> dense = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  vector<int> v = {1, 2, 3, 4};
  vector<int> result1 = {2, 5, 3, 16};

  TCRSMatrix<int> m(dense);

  vector<int> result2 = m * v;

  EXPECT_EQ(result1, result2);
}

TEST(TCRSMatrix, cant_multiply_matrix_by_vector_of_incompatible_size)
{
  vector<vector<int>> dense = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  vector<int> v = {1, 2, 3};

  TCRSMatrix<int> m(dense);

  ASSERT_ANY_THROW(m * v);
}

TEST(TCRSMatrix, multiply_zero_matrix_by_vector_returns_zero_vector)
{
  vector<vector<int>> dense = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  };

  vector<int> v = {1, 2, 3, 4};
  vector<int> result1 = {0, 0, 0, 0};

  TCRSMatrix<int> m(dense);

  vector<int> result2 = m * v;

  EXPECT_EQ(result1, result2);
}

TEST(TCRSMatrix, can_multiply_square_crs_matrices_of_equal_size)
{
  vector<vector<int>> dense1 = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  vector<vector<int>> dense2 = {
    {0, 2, 3, 0},
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 0, 4}
  };

  vector<vector<int>> result1 = {
    {1, 0, 0, 0},
    {0, 5, 6, 0},
    {0, 6, 9, 0},
    {0, 0, 0, 16}
  };

  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);

  TCRSMatrix<int> result2 = m1 * m2;

  EXPECT_EQ(result1, result2.toDense());
}

TEST(TCRSMatrix, can_multiply_rectangular_crs_matrices_of_compatible_size)
{
  vector<vector<int>> dense1 = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0}
  };

  vector<vector<int>> dense2 = {
    {0, 2, 3},
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 0}
  };

  vector<vector<int>> result1 = {
    {1, 0, 0},
    {0, 5, 6},
    {0, 6, 9}
  };

  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);

  TCRSMatrix<int> result2 = m1 * m2;

  EXPECT_EQ(result1, result2.toDense());
}

TEST(TCRSMatrix, cant_multiply_crs_matrices_with_incompatible_sizes)
{
  vector<vector<int>> dense1 = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  vector<vector<int>> dense2 = {
    {1, 0, 1},
    {2, 0, 0},
    {0, 1, 0}
  };

  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);

  ASSERT_ANY_THROW(m1 * m2);
}

TEST(TCRSMatrix, multiply_by_identity_matrix_returns_original_matrix)
{
  vector<vector<int>> dense1 = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  vector<vector<int>> dense2 = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  };

  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);

  TCRSMatrix<int> result = m1 * m2;

  EXPECT_EQ(result, m1);
}

TEST(TCRSMatrix, multiply_matrix_with_empty_rows_works_correctly)
{
  vector<vector<int>> dense1 = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 4}
  };

  vector<vector<int>> dense2 = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };

  vector<vector<int>> result1 = {
    {2, 0, 1, 0},
    {3, 2, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 16}
  };

  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);

  TCRSMatrix<int> result2 = m1 * m2;

  EXPECT_EQ(result1, result2.toDense());
}

// TEST(TDynamicMatrix, cant_subtract_matrixes_with_not_equal_size)
// {
//   ADD_FAILURE();
// }

