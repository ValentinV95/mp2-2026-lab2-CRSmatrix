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
  vector<vector<int>> empty_dense;
  ASSERT_THROW(TCRSMatrix<int> m(empty_dense), out_of_range);

  vector<vector<int>> ragged_dense = {
    {1, 2, 3},
    {4, 5}
  };
  ASSERT_THROW(TCRSMatrix<int> m(ragged_dense), out_of_range);
}

TEST(TCRSMatrix, crs_arrays_are_formed_correctly_from_dense)
{
  vector<vector<int>> dense = {
    {0, 5, 0},
    {7, 0, 8},
    {0, 0, 9}
  };

  TCRSMatrix<int> tmp(dense);

  vector<int> expected_values = { 5, 7, 8, 9 };
  vector<size_t> expected_col_index = { 1, 0, 2, 2 };
  vector<size_t> expected_row_ptr = { 0, 1, 3, 4 };

  EXPECT_EQ(tmp.getValues(), expected_values);
  EXPECT_EQ(tmp.getColIndices(), expected_col_index);
  EXPECT_EQ(tmp.getRowPtr(), expected_row_ptr);
}

TEST(TCRSMatrix, can_convert_crs_matrix_back_to_dense)
{
  vector<vector<int>> dense = {
    {0, 5, 0},
    {7, 0, 8},
    {0, 0, 9}
  };

  TCRSMatrix<int> tmp(dense);
  vector<vector<int>> matrix_tmp = tmp.toDense();

  EXPECT_EQ(matrix_tmp, dense);
}

TEST(TCRSMatrix, can_create_copied_matrix)
{
  TCRSMatrix<int> m(7);
  ASSERT_NO_THROW(TCRSMatrix<int> m1(m));
}

TEST(TCRSMatrix, copied_matrix_is_equal_to_source_one)
{
  TCRSMatrix<int> m(5);
  TCRSMatrix<int> m1(m);

  EXPECT_EQ(m1, m);
}

TEST(TCRSMatrix, copied_matrix_has_its_own_memory)
{
  vector<vector<int>> dense = {
    {2, 0, 7},
    {0, 8, 0},
    {9, 0, 6}
  };
  TCRSMatrix<int> m(dense);
  TCRSMatrix<int> m1(m);

  EXPECT_NE(m1.getValues().data(), m.getValues().data());
  EXPECT_NE(m1.getColIndices().data(), m.getColIndices().data());
  EXPECT_NE(m1.getRowPtr().data(), m.getRowPtr().data());
}

TEST(TCRSMatrix, can_assign_matrix_to_itself)
{
  vector<vector<int>> dense = {
    {1, 0, 2},
    {0, 3, 0},
    {4, 0, 5}
  };

  TCRSMatrix<int> m(dense);

  ASSERT_NO_THROW(m = m);
}

TEST(TCRSMatrix, can_assign_matrices_of_equal_size)
{
  vector<vector<int>> dense1 = {
    {2, 0, 6},
    {0, 9, 0},
    {4, 0, 3}
  };
  vector<vector<int>> dense2 = {
    {0, 8, 0},
    {5, 0, 7},
    {0, 1, 0}
  };

  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);

  EXPECT_NO_THROW(m1 = m2);
  EXPECT_EQ(m1, m2);
}

TEST(TCRSMatrix, assign_operator_change_matrix_size)
{
  vector<vector<int>> dense1 = {
    {2, 0, 6},
    {0, 9, 0},
    {4, 0, 3}
  };
  vector<vector<int>> dense2 = {
    {0, 8},
    {5, 0}
  };

  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);

  m2 = m1;

  EXPECT_EQ(m2.getRows(), 3);
  EXPECT_EQ(m2.getCols(), 3);
  EXPECT_EQ(m2, m1);
}

TEST(TCRSMatrix, can_assign_matrices_of_different_size)
{
  vector<vector<int>> dense1 = {
    {2, 0, 6},
    {0, 9, 0},
    {4, 0, 3}
  };
  vector<vector<int>> dense2 = {
    {0, 8},
    {5, 0}
  };

  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);

  m2 = m1;

  EXPECT_EQ(m2.getRows(), 3);
  EXPECT_EQ(m2.getCols(), 3);
  EXPECT_EQ(m2.toDense(), m1.toDense());
}

TEST(TCRSMatrix, can_get_rows_cols_and_nnz)
{
  vector<vector<int>> dense = {
    {2, 0, 6},
    {0, 9, 0},
    {4, 0, 3}
  };
  TCRSMatrix<int> m(dense);

  EXPECT_EQ(m.getRows(), 3);
  EXPECT_EQ(m.getCols(), 3);
  EXPECT_EQ(m.getNNZ(), 5);
}

TEST(TCRSMatrix, can_get_nonzero_and_zero_elements_by_at)
{
  vector<vector<int>> dense = {
    {2, 0, 6},
    {0, 9, 0},
    {4, 0, 3}
  };
  TCRSMatrix<int> m(dense);

  EXPECT_EQ(m.at(0, 0), 2);
  EXPECT_EQ(m.at(1, 1), 9);
  EXPECT_EQ(m.at(2, 0), 4);
}

TEST(TCRSMatrix, throws_when_get_element_with_negative_or_too_large_index)
{
  vector<vector<int>> dense = {
    {2, 0, 6},
    {0, 9, 0},
    {4, 0, 3}
  };
  TCRSMatrix<int> m(dense);

  EXPECT_ANY_THROW(m.at(3, 0));
  EXPECT_ANY_THROW(m.at(0, 3));
}

TEST(TCRSMatrix, compare_equal_matrices_return_true)
{
  vector<vector<int>> dense = {
    {2, 0, 6},
    {0, 9, 0},
    {4, 0, 3}
  };
  TCRSMatrix<int> a(dense);
  TCRSMatrix<int> b(dense);

  EXPECT_TRUE(a == b);
}

TEST(TCRSMatrix, compare_matrix_with_itself_return_true)
{
  vector<vector<int>> dense = {
    {2, 0, 6},
    {0, 9, 0},
    {4, 0, 3}
  };
  TCRSMatrix<int> m(dense);

  EXPECT_TRUE(m == m);
}

TEST(TCRSMatrix, matrices_with_different_size_are_not_equal)
{
  vector<vector<int>> dense1 = {
    {2, 0, 6},
    {0, 9, 0},
    {4, 0, 3}
  };
  vector<vector<int>> dense2 = {
    {0, 8},
    {5, 0}
  };
  TCRSMatrix<int> a(dense1);
  TCRSMatrix<int> b(dense2);

  EXPECT_FALSE(a == b);
}

TEST(TCRSMatrix, matrices_with_same_size_but_different_elements_are_not_equal)
{
  vector<vector<int>> dense1 = {
    {2, 0, 6},
    {0, 9, 0},
    {4, 0, 3}
  };
  vector<vector<int>> dense2 = {
    {2, 0, 6},
    {0, 8, 0},
    {4, 0, 3}
  };
  TCRSMatrix<int> a(dense1);
  TCRSMatrix<int> b(dense2);

  EXPECT_FALSE(a == b);
}

TEST(TCRSMatrix, can_transpose_square_matrix)
{
  vector<vector<int>> dense = {
    {1, 0, 2},
    {0, 3, 0},
    {4, 0, 5}
  };
  vector<vector<int>> expected = {
    {1, 0, 4},
    {0, 3, 0},
    {2, 0, 5}
  };

  TCRSMatrix<int> m(dense);
  TCRSMatrix<int> transposed = m.transpose();
  vector<vector<int>> result = transposed.toDense();

  EXPECT_EQ(result, expected);
}

TEST(TCRSMatrix, can_transpose_rectangular_matrix)
{
  vector<vector<int>> dense1 = {
    {2, 0, 5},
    {0, 7, 0}
  };
  vector<vector<int>> dense2 = {
    {2, 0},
    {0, 7},
    {5, 0}
  };
  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);

  m1 = m1.transpose();

  EXPECT_EQ(m1, m2);
}

TEST(TCRSMatrix, double_transpose_returns_original_matrix)
{
  vector<vector<int>> dense = {
    {2, 0, 6},
    {0, 9, 0},
    {4, 0, 3}
  };

  TCRSMatrix<int> m(dense);

  TCRSMatrix<int> t1 = m.transpose();
  TCRSMatrix<int> t2 = t1.transpose();

  EXPECT_EQ(m, t2);
}

TEST(TCRSMatrix, can_multiply_matrix_by_vector_of_compatible_size)
{
  vector<vector<int>> dense = {
    {2, 0, 6},
    {0, 9, 0},
    {4, 0, 3}
  };
  vector<int> vec = {5, 6, 7};
  vector<int> expected = {52, 54, 41};

  TCRSMatrix<int> m(dense);
  vector<int> res = m * vec;

  EXPECT_EQ(res, expected);
}

TEST(TCRSMatrix, cant_multiply_matrix_by_vector_of_incompatible_size)
{
  vector<vector<int>> dense = {
    {2, 0, 6},
    {0, 9, 0},
    {4, 0, 3}
  };
  vector<int> vec = {5, 6};
  TCRSMatrix<int> m(dense);

  EXPECT_ANY_THROW(m * vec);
}

TEST(TCRSMatrix, multiply_zero_matrix_by_vector_returns_zero_vector)
{
  vector<vector<int>> dense = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
  };
  vector<int> vec = {5, 6, 7};
  vector<int> expected = {0, 0, 0};

  TCRSMatrix<int> m(dense);
  vector<int> res = m * vec;

  EXPECT_EQ(res, expected);
}

TEST(TCRSMatrix, can_multiply_square_crs_matrices_of_equal_size)
{
  vector<vector<int>> dense1 = {
    {2, 0, 6},
    {0, 9, 0},
    {4, 0, 3}
  };

  vector<vector<int>> dense2 = {
    {1, 0, 2},
    {0, 6, 0},
    {3, 4, 0}
  };

  vector<vector<int>> res1 = {
    {20, 24, 4},
    {0, 54, 0},
    {13, 12, 8}
  };

  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);

  TCRSMatrix<int> res2 = m1 * m2;

  EXPECT_EQ(res1, res2.toDense());
}

TEST(TCRSMatrix, can_multiply_rectangular_crs_matrices_of_compatible_size)
{
  vector<vector<int>> dense1 = {
    {2, 0, 6},
    {0, 9, 0},
    {4, 0, 3}
  };

  vector<vector<int>> dense2 = {
    {1, 0},
    {0, 6},
    {3, 4}
  };

  vector<vector<int>> res1 = {
    {20, 24},
    {0, 54},
    {13, 12}
  };

  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);

  TCRSMatrix<int> res2 = m1 * m2;

  EXPECT_EQ(res1, res2.toDense());
}

TEST(TCRSMatrix, cant_multiply_crs_matrices_with_incompatible_sizes)
{
  vector<vector<int>> dense1 = {
    {2, 0, 6},
    {0, 9, 0},
    {4, 0, 3}
  };

  vector<vector<int>> dense2 = {
    {1, 2},
    {3, 4}
  };

  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);

  ASSERT_ANY_THROW(m1 * m2);
}

TEST(TCRSMatrix, multiply_by_identity_matrix_returns_original_matrix)
{
  vector<vector<int>> dense1 = {
    {1, 0, 2},
    {0, 3, 0},
    {4, 5, 0}
  };
  
  vector<vector<int>> dense2 = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
  };

  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);

  TCRSMatrix<int> res = m1 * m2;

  EXPECT_EQ(res, m1);
}

TEST(TCRSMatrix, multiply_matrix_with_empty_rows_works_correctly)
{
  vector<vector<int>> dense1 = {
    {0, 0, 0}, 
    {1, 0, 2}, 
    {0, 0, 0}
  };
  
  vector<vector<int>> dense2 = {
    {3, 0}, 
    {0, 4}, 
    {5, 0}
  };
  
  vector<vector<int>> expected = {
    {0, 0}, 
    {13, 0}, 
    {0, 0}
  };

  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);
  TCRSMatrix<int> m3(expected);

  // Умножаем и сохраняем результат в res
  TCRSMatrix<int> res = m1 * m2;

  // Сравниваем результат с ожидаемой матрицей
  EXPECT_EQ(res, m3);
}