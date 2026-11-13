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
    ASSERT_ANY_THROW(TCRSMatrix<int> m(empty));

    vector<vector<int>> inconsistent = {
        {1, 1},
        {1}
    };
    ASSERT_ANY_THROW(TCRSMatrix<int> m(inconsistent));
}

TEST(TCRSMatrix, crs_arrays_are_formed_correctly_from_dense)
{
    vector<vector<int>> dense = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    TCRSMatrix<int> m(dense);

    vector<int> values = { 1,2,3,4 };
    vector<size_t> col_index = { 0,2,0,1 };
    vector<size_t> row_ptr = { 0,2,2,4 };

    EXPECT_EQ(m.getValues(), values);
    EXPECT_EQ(m.getColIndices(), col_index);
    EXPECT_EQ(m.getRowPtr(), row_ptr);
}

TEST(TCRSMatrix, can_convert_crs_matrix_back_to_dense)
{
    vector<vector<int>> dense = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    TCRSMatrix<int> m(dense);
    vector<vector<int>>expected = m.toDense();
    EXPECT_EQ(dense, expected);
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
      {0, 0, 0},
      {3, 4, 0}
    };
    TCRSMatrix<int> m(dense);

    TCRSMatrix<int> m_copy(m);

    EXPECT_EQ(m, m_copy);
}

TEST(TCRSMatrix, copied_matrix_has_its_own_memory)
{
    vector<vector<int>> dense = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    TCRSMatrix<int> m(dense);

    TCRSMatrix<int> m_copy(m);

    EXPECT_NE(&m.getValues(), &m_copy.getValues());
    EXPECT_NE(&m.getColIndices(), &m_copy.getColIndices());
    EXPECT_NE(&m.getRowPtr(), &m_copy.getRowPtr());
}

TEST(TCRSMatrix, can_assign_matrix_to_itself)
{
    vector<vector<int>> dense = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    TCRSMatrix<int> m(dense);

    ASSERT_NO_THROW(m = m);
}

TEST(TCRSMatrix, can_assign_matrices_of_equal_size)
{
    vector<vector<int>> dense1 = {
        {1, 0},
        {0, 2}
    };
    vector<vector<int>> dense2 = {
        {0, 1},
        {2, 0}
    };
    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);

    ASSERT_NO_THROW(m1 = m2);
}

TEST(TCRSMatrix, assigned_matrices_of_equal_size_equals)
{
    vector<vector<int>> dense1 = {
        {1, 0},
        {0, 2}
    };
    vector<vector<int>> dense2 = {
        {0, 1},
        {2, 0}
    };
    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);
    m1 = m2;

    EXPECT_EQ(m1, m2);
}

TEST(TCRSMatrix, assign_operator_change_matrix_size)
{
    TCRSMatrix<int> m1(1, 2);
    TCRSMatrix<int> m2(3, 4);
    m1 = m2;

    EXPECT_EQ(m1.getRows(), 3);
    EXPECT_EQ(m1.getCols(), 4);
}

TEST(TCRSMatrix, can_assign_matrices_of_different_size)
{
    vector<vector<int>> dense1 = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    vector<vector<int>> dense2 = {
        {4, 3},
        {2, 1}
    };
    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);

    ASSERT_NO_THROW(m1 = m2);
}

TEST(TCRSMatrix, assigned_matrices_of_different_size_equals)
{
    vector<vector<int>> dense1 = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    vector<vector<int>> dense2 = {
        {4, 3},
        {2, 1}
    };
    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);
    m1 = m2;

    EXPECT_EQ(m1, m2);
}


TEST(TCRSMatrix, can_get_rows_cols_and_nnz)
{
    vector<vector<int>> dense = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    TCRSMatrix<int> m(dense);

    EXPECT_EQ(m.getRows(), 3);
    EXPECT_EQ(m.getCols(), 3);
    EXPECT_EQ(m.getNNZ(), 4);
}

TEST(TCRSMatrix, can_get_nonzero_and_zero_elements_by_at)
{
    vector<vector<int>> dense = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    TCRSMatrix<int> m(dense);

    EXPECT_EQ(m.at(0, 0), 1);
    EXPECT_EQ(m.at(0, 1), 0);
    EXPECT_EQ(m.at(0, 2), 2);
    EXPECT_EQ(m.at(1, 0), 0);
    EXPECT_EQ(m.at(1, 1), 0);
    EXPECT_EQ(m.at(1, 2), 0);
    EXPECT_EQ(m.at(2, 0), 3);
    EXPECT_EQ(m.at(2, 1), 4);
    EXPECT_EQ(m.at(2, 2), 0);
}

TEST(TCRSMatrix, throws_when_get_element_with_negative_or_too_large_index)
{
    TCRSMatrix<int> m(3, 3);

    ASSERT_ANY_THROW(m.at(4, 0));
    ASSERT_ANY_THROW(m.at(0, 4));
    ASSERT_ANY_THROW(m.at(-4, 0));
    ASSERT_ANY_THROW(m.at(0, -4));
}

TEST(TCRSMatrix, compare_equal_matrices_return_true)
{
    vector<vector<int>> dense = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    TCRSMatrix<int> m1(dense);
    TCRSMatrix<int> m2(dense);

    EXPECT_TRUE(m1 == m2);

    EXPECT_FALSE(m1 != m2);
}

TEST(TCRSMatrix, compare_matrix_with_itself_return_true)
{
    vector<vector<int>> dense = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    TCRSMatrix<int> m(dense);

    EXPECT_TRUE(m == m);
}

TEST(TCRSMatrix, matrices_with_different_size_are_not_equal)
{
    TCRSMatrix<int> m1(1, 2);
    TCRSMatrix<int> m2(3, 4);

    EXPECT_TRUE(m1 != m2);
}

TEST(TCRSMatrix, matrices_with_same_size_but_different_elements_are_not_equal)
{
    vector<vector<int>> dense1 = {
        {1, 0},
        {0, 2}
    };
    vector<vector<int>> dense2 = {
        {2, 0},
        {0, 3}
    };
    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);

    EXPECT_FALSE(m1 == m2);
    EXPECT_TRUE(m1 != m2);
}

TEST(TCRSMatrix, can_transpose_square_matrix)
{
    vector<vector<int>> dense = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    vector<vector<int>> expected = {
      {1, 0, 3},
      {0, 0, 4},
      {2, 0, 0}
    };
    TCRSMatrix<int> m(dense);
    TCRSMatrix<int> m_expected = m.transpose();

    EXPECT_EQ(m_expected.toDense(), expected);
}

TEST(TCRSMatrix, can_transpose_rectangular_matrix)
{
    vector<vector<int>> dense = {
        {1, 0, 2},
        {3, 4, 0}
    };
    vector<vector<int>> expected = {
        {1, 3},
        {0, 4},
        {2, 0}
    };
    TCRSMatrix<int> m(dense);
    TCRSMatrix<int> m_expected = m.transpose();

    EXPECT_EQ(m_expected.toDense(), expected);
}

TEST(TCRSMatrix, double_transpose_returns_original_matrix)
{
    vector<vector<int>> dense = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    TCRSMatrix<int> m(dense);
    TCRSMatrix<int> m_double_tranposed = m.transpose().transpose();

    EXPECT_EQ(m, m_double_tranposed);
}

TEST(TCRSMatrix, can_multiply_matrix_by_vector_of_compatible_size)
{
    vector<vector<int>> dense = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    TCRSMatrix<int> m(dense);
    vector<int> vec = {1, 2, 3};
    vector<int> expected = {7, 0, 11};
    vector<int> result = m * vec;

    EXPECT_EQ(result, expected);
}

TEST(TCRSMatrix, cant_multiply_matrix_by_vector_of_incompatible_size)
{
    TCRSMatrix<int> m(3, 3);
    vector<int> vec = {1, 2};

    ASSERT_ANY_THROW(m * vec);
}

TEST(TCRSMatrix, multiply_zero_matrix_by_vector_returns_zero_vector)
{
    TCRSMatrix<int> m(3, 3);
    vector<int> v = {1, 2, 3};
    vector<int> expected = {0, 0, 0};
    vector<int> result = m * v;

    EXPECT_EQ(result, expected);
}

TEST(TCRSMatrix, can_multiply_square_crs_matrices_of_equal_size)
{
    vector<vector<int>> dense1 = {
      {1, 0, 2},
      {0, 1, 0},
      {3, 4, 0}
    };
    vector<vector<int>> dense2 = {
      {1, 2, 0},
      {0, 0, 4},
      {0, 3, 0}
    };
    vector<vector<int>> expected = {
      {1, 8, 0},
      {0, 0, 4},
      {3, 6, 16}
    };
    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);
    TCRSMatrix<int> result = m1 * m2;

    EXPECT_EQ(result.toDense(), expected);
}

TEST(TCRSMatrix, can_multiply_rectangular_crs_matrices_of_compatible_size)
{
    vector<vector<int>> dense1 = {
      {1, 0, 2},
      {3, 4, 0}
    };
    vector<vector<int>> dense2 = {
      {1, 2},
      {0, 4},
      {0, 3}
    };
    vector<vector<int>> expected = {
      {1, 8},
      {3, 22}
    };
    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);
    TCRSMatrix<int> result = m1 * m2;

    EXPECT_EQ(result.toDense(), expected);
}

TEST(TCRSMatrix, cant_multiply_crs_matrices_with_incompatible_sizes)
{
    TCRSMatrix<int> m1(2, 3);
    TCRSMatrix<int> m2(2, 2);

    ASSERT_ANY_THROW(m1 * m2);
}

TEST(TCRSMatrix, multiply_by_identity_matrix_returns_original_matrix)
{
    vector<vector<int>> dense = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    vector<vector<int>> identity = {
      {1, 0, 0},
      {0, 1, 0},
      {0, 0, 1}
    };
    TCRSMatrix<int> m(dense);
    TCRSMatrix<int> one(identity);

    EXPECT_EQ((m * one), m);
    EXPECT_EQ((one * m), m);
}

TEST(TCRSMatrix, multiply_matrix_with_empty_rows_works_correctly)
{
    vector<vector<int>> dense1 = {
      {0, 0, 0},
      {1, 0, 2},
      {0, 0, 0}
    };
    vector<vector<int>> dense2 = {
      {1, 1, 1},
      {1, 1, 1},
      {1, 2, 3}
    };
    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);

    vector<vector<int>> expected_dense = {
      {0, 0, 0},
      {3, 5, 7},
      {0, 0, 0}
    };

    TCRSMatrix<int> res = m1 * m2;
    EXPECT_EQ(res.toDense(), expected_dense);
}

TEST(TCRSMatrix, can_correctly_create_matrix_from_components)
{
    size_t rows = 2;
    size_t cols = 2;
    vector<int> values = {2, 3};
    vector<size_t> c_idx = {0, 1};
    vector<size_t> r_ptr = {0, 1, 2};

    ASSERT_NO_THROW(TCRSMatrix<int> m(2, 2, values, c_idx, r_ptr));

    ASSERT_ANY_THROW(TCRSMatrix<int> m(2, 2, {2}, c_idx, r_ptr));
    ASSERT_ANY_THROW(TCRSMatrix<int> m(2, 2, values, {0}, r_ptr));

    ASSERT_ANY_THROW(TCRSMatrix<int> m(2, 2, values, c_idx, {0, 1}));

    ASSERT_ANY_THROW(TCRSMatrix<int> m(0, 2, values, c_idx, r_ptr));

    ASSERT_ANY_THROW(TCRSMatrix<int> m(2, 1, values, c_idx, r_ptr));
}

//TEST(TDynamicMatrix, cant_subtract_matrixes_with_not_equal_size)
//{
//  ADD_FAILURE();
//}

