#include "tmatrix.h"
#include <sstream>
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
  ASSERT_ANY_THROW(TCRSMatrix<int> m(empty));
 
  vector<vector<int>> empty_row(1);            
  ASSERT_ANY_THROW(TCRSMatrix<int> m(empty_row));
 
  vector<vector<int>> ragged = {{1, 2, 3}, {4, 5}};
  ASSERT_ANY_THROW(TCRSMatrix<int> m(ragged));
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
  EXPECT_EQ(vector<int>({1, 2, 1, 3, 4}), m.getValues());
  EXPECT_EQ(vector<size_t>({1, 0, 2, 0, 3}), m.getColIndices());
  EXPECT_EQ(vector<size_t>({0, 1, 3, 4, 5}), m.getRowPtr());
}

TEST(TCRSMatrix, can_convert_crs_matrix_back_to_dense)
{
  vector<vector<int>> dense = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  TCRSMatrix<int> m(dense);
  EXPECT_EQ(dense, m.toDense());
}

TEST(TCRSMatrix, can_create_copied_matrix)
{
  TCRSMatrix<int> m(5);

  ASSERT_NO_THROW(TCRSMatrix<int> m1(m));
}

TEST(TCRSMatrix, copied_matrix_is_equal_to_source_one)
{
  vector<vector<int>> dense = {{1, 0, 2}, {0, 3, 0}};
  TCRSMatrix<int> m(dense);
  TCRSMatrix<int> copy(m);
  EXPECT_TRUE(copy == m);
}

TEST(TCRSMatrix, copied_matrix_has_its_own_memory)
{
  vector<vector<int>> dense = {{1, 0, 2}, {0, 3, 0}};
  TCRSMatrix<int> m(dense);
  TCRSMatrix<int> copy(m);
 
  copy = TCRSMatrix<int>(4, 4);
  EXPECT_EQ(dense, m.toDense());
}

TEST(TCRSMatrix, can_assign_matrix_to_itself)
{
  vector<vector<int>> dense = {{1, 0, 2}, {0, 3, 0}};
  TCRSMatrix<int> m(dense);
  ASSERT_NO_THROW(m = m);
  EXPECT_EQ(dense, m.toDense());
}

TEST(TCRSMatrix, can_assign_matrices_of_equal_size)
{
  vector<vector<int>> dense = {{1, 0, 2}, {0, 3, 0}};
  TCRSMatrix<int> m1(dense);
  TCRSMatrix<int> m2(2, 3);
 
  m2 = m1;
  EXPECT_TRUE(m2 == m1);
}

TEST(TCRSMatrix, assign_operator_change_matrix_size)
{
  vector<vector<int>> dense = {{1, 0, 2}, {0, 3, 0}};
  TCRSMatrix<int> m1(dense);
  TCRSMatrix<int> m2(5, 5);
 
  m2 = m1;
  EXPECT_EQ(2, m2.getRows());
  EXPECT_EQ(3, m2.getCols());
}

TEST(TCRSMatrix, can_assign_matrices_of_different_size)
{
  vector<vector<int>> dense = {{1, 0, 2}, {0, 3, 0}};
  TCRSMatrix<int> m1(dense);
  TCRSMatrix<int> m2(5, 5);
 
  ASSERT_NO_THROW(m2 = m1);
  EXPECT_EQ(dense, m2.toDense());
}

TEST(TCRSMatrix, can_get_rows_cols_and_nnz)
{
  vector<vector<int>> dense = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  TCRSMatrix<int> m(dense);
 
  EXPECT_EQ(3, m.getRows());
  EXPECT_EQ(3, m.getCols());
  EXPECT_EQ(4, m.getNNZ());
}

TEST(TCRSMatrix, can_get_nonzero_and_zero_elements_by_at)
{
  vector<vector<int>> dense = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  TCRSMatrix<int> m(dense);
 
  EXPECT_EQ(1, m.at(0, 0));
  EXPECT_EQ(2, m.at(0, 2));
  EXPECT_EQ(4, m.at(2, 1));
  EXPECT_EQ(0, m.at(0, 1));
  EXPECT_EQ(0, m.at(1, 1));
}

TEST(TCRSMatrix, throws_when_get_element_with_negative_or_too_large_index)
{
  TCRSMatrix<int> m(3, 4);
 
  ASSERT_ANY_THROW(m.at(-1, 0));
  ASSERT_ANY_THROW(m.at(0, -1));
  ASSERT_ANY_THROW(m.at(3, 0));
  ASSERT_ANY_THROW(m.at(0, 4)); 
}

TEST(TCRSMatrix, compare_equal_matrices_return_true)
{
  vector<vector<int>> dense = {{1, 0, 2}, {0, 3, 0}};
  TCRSMatrix<int> m1(dense);
  TCRSMatrix<int> m2(dense);
  EXPECT_TRUE(m1 == m2);
  EXPECT_FALSE(m1 != m2);
}

TEST(TCRSMatrix, compare_matrix_with_itself_return_true)
{
  vector<vector<int>> dense = {{1, 0, 2}, {0, 3, 0}};
  TCRSMatrix<int> m(dense);
  EXPECT_TRUE(m == m);
}

TEST(TCRSMatrix, matrices_with_different_size_are_not_equal)
{
  TCRSMatrix<int> m1(3, 3);
  TCRSMatrix<int> m2(3, 4);
  EXPECT_TRUE(m1 != m2);
}

TEST(TCRSMatrix, matrices_with_same_size_but_different_elements_are_not_equal)
{
  vector<vector<int>> d1 = {{1, 0}, {0, 2}};
  vector<vector<int>> d2 = {{1, 0}, {0, 3}};
  TCRSMatrix<int> m1(d1);
  TCRSMatrix<int> m2(d2);
  EXPECT_TRUE(m1 != m2);
}

TEST(TCRSMatrix, can_transpose_square_matrix)
{
  vector<vector<int>> dense = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };
  vector<vector<int>> expected = {
    {0, 2, 3, 0},
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 0, 4}
  };
  TCRSMatrix<int> m(dense);
  EXPECT_EQ(expected, m.transpose().toDense());
}

TEST(TCRSMatrix, can_transpose_rectangular_matrix)
{
  vector<vector<int>> dense = {
    {1, 0, 2},
    {0, 3, 0}
  };
  vector<vector<int>> expected = {
    {1, 0},
    {0, 3},
    {2, 0}
  };
  TCRSMatrix<int> m(dense);
  TCRSMatrix<int> t = m.transpose();
 
  EXPECT_EQ(3, t.getRows());
  EXPECT_EQ(2, t.getCols());
  EXPECT_EQ(expected, t.toDense());
}

TEST(TCRSMatrix, double_transpose_returns_original_matrix)
{
    vector<vector<int>> dense = {
    {1, 0, 2},
    {0, 3, 0}
  };
  TCRSMatrix<int> m(dense);
  EXPECT_TRUE(m.transpose().transpose() == m);
}

TEST(TCRSMatrix, can_multiply_matrix_by_vector_of_compatible_size)
{
  vector<vector<int>> dense = {
    {0, 1, 0, 0},
    {2, 0, 1, 0},
    {3, 0, 0, 0},
    {0, 0, 0, 4}
  };
  TCRSMatrix<int> m(dense);
  vector<int> x = {1, 2, 3, 4};
  EXPECT_EQ(vector<int>({2, 5, 3, 16}), m*x);
}

TEST(TCRSMatrix, cant_multiply_matrix_by_vector_of_incompatible_size)
{
  TCRSMatrix<int> m(3, 4);
  vector<int> x = {1, 2, 3};
  ASSERT_ANY_THROW(m*x);
}

TEST(TCRSMatrix, multiply_zero_matrix_by_vector_returns_zero_vector)
{
  TCRSMatrix<int> m(3, 3);
  vector<int> x = {1, 2, 3};
 
  EXPECT_EQ(vector<int>({0, 0, 0}), m*x);
}

TEST(TCRSMatrix, can_multiply_square_crs_matrices_of_equal_size)
{
  vector<vector<int>> d1 = {
    {1, 2},
    {0, 3}
  };
  vector<vector<int>> d2 = {
    {4, 0},
    {5, 6}
  };
  vector<vector<int>> expected = {
    {14, 12},
    {15, 18}
  };
  TCRSMatrix<int> m1(d1);
  TCRSMatrix<int> m2(d2);
  EXPECT_EQ(expected, (m1 * m2).toDense());
}

TEST(TCRSMatrix, can_multiply_rectangular_crs_matrices_of_compatible_size)
{
  vector<vector<int>> d1 = {
    {1, 0, 2},
    {0, 3, 0}
  };
  vector<vector<int>> d2 = {
    {1, 0},
    {0, 1},
    {2, 0}
  };
  vector<vector<int>> expected = {
    {5, 0},
    {0, 3}
  };
  TCRSMatrix<int> m1(d1);
  TCRSMatrix<int> m2(d2);
  TCRSMatrix<int> res = m1 * m2;
  EXPECT_EQ(2, res.getRows());
  EXPECT_EQ(2, res.getCols());
  EXPECT_EQ(expected, res.toDense());

}

TEST(TCRSMatrix, cant_multiply_crs_matrices_with_incompatible_sizes)
{
  TCRSMatrix<int> m1(2, 3);
  TCRSMatrix<int> m2(2, 3);
  ASSERT_ANY_THROW(m1 * m2);
}

TEST(TCRSMatrix, multiply_by_identity_matrix_returns_original_matrix)
{
  vector<vector<int>> dense = {
    {1, 0, 2},
    {0, 3, 0},
    {4, 0, 5}
  };
  vector<vector<int>> identity = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
  };
  TCRSMatrix<int> m(dense);
  TCRSMatrix<int> e(identity);
  EXPECT_TRUE(m * e == m);
  EXPECT_TRUE(e * m == m);
}

TEST(TCRSMatrix, multiply_matrix_with_empty_rows_works_correctly)
{
  vector<vector<int>> d1 = {
    {1, 2},
    {0, 0},
    {0, 3}
  };
  vector<vector<int>> d2 = {
    {1, 0},
    {0, 1}
  };
  TCRSMatrix<int> m1(d1);
  TCRSMatrix<int> m2(d2);
  EXPECT_EQ(d1, (m1 * m2).toDense());
}


