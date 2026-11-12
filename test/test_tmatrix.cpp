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
    vector<vector<int>> emptyDense;
    ASSERT_ANY_THROW(TCRSMatrix<int> m(emptyDense));

    vector<vector<int>> inconsistentDense = {
        {1, 0, 0},
        {0, 2}
    };
    ASSERT_ANY_THROW(TCRSMatrix<int> m(inconsistentDense));
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
    vector<int> val = { 1, 2, 1, 3, 4 };
    vector<size_t> col = { 1, 0, 2, 0, 3 };
    vector<size_t> rowPtr = { 0, 1, 3, 4, 5 };
    EXPECT_EQ(val, m.getValues());
    EXPECT_EQ(col, m.getColIndices());
    EXPECT_EQ(rowPtr, m.getRowPtr());
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
  EXPECT_EQ(dense, m.toDense());
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
    TCRSMatrix<int> m1(m);
    EXPECT_EQ(m, m1);
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
    TCRSMatrix<int> m1(m);
    EXPECT_NE(&(m.getValues()), &(m1.getValues()));
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
    EXPECT_EQ(dense, m.toDense());
}

TEST(TCRSMatrix, can_assign_matrices_of_equal_size)
{
    vector<vector<int>> dense = {
         {0, 1},
         {2, 0}
    };
    vector<vector<int>> dense1 = {
         {1, 0},
         {0, 2}
    };
    TCRSMatrix<int> m(dense), m1(dense1);
    m = m1;
    EXPECT_EQ(m, m1);
}

TEST(TCRSMatrix, assign_operator_change_matrix_size)
{
    TCRSMatrix<int> m1(2, 2);
    TCRSMatrix<int> m2(4, 5);
    m1 = m2;
    EXPECT_EQ(4, m1.getRows());
    EXPECT_EQ(5, m1.getCols());
}

TEST(TCRSMatrix, can_assign_matrices_of_different_size)
{
    vector<vector<int>> dense = {
         {0, 1},
         {2, 0}
    };
    vector<vector<int>> dense1 = {
         {1, 0, 0},
         {0, 2, 0}
    };
    TCRSMatrix<int> m(dense), m1(dense1);
    m = m1;
    EXPECT_EQ(m, m1);
}

TEST(TCRSMatrix, can_get_rows_cols_and_nnz)
{
    vector<vector<int>> dense = {
         {1, 0, 0},
         {0, 2, 0}
    };
    TCRSMatrix<int> m(dense);
    EXPECT_EQ(2, m.getRows());
    EXPECT_EQ(3, m.getCols());
    EXPECT_EQ(2, m.getNNZ());
}

TEST(TCRSMatrix, can_get_nonzero_and_zero_elements_by_at)
{
    vector<vector<int>> dense = {
         {0, 1},
         {2, 0}
    };
    TCRSMatrix<int> m(dense);
    EXPECT_EQ(0, m.at(0, 0));
    EXPECT_EQ(1, m.at(0, 1));
    EXPECT_EQ(2, m.at(1, 0));
    EXPECT_EQ(0, m.at(1, 1));
}

TEST(TCRSMatrix, throws_when_get_element_with_negative_or_too_large_index)
{
    TCRSMatrix<int> m(3, 3);
    ASSERT_ANY_THROW(m.at(3, 0));
    ASSERT_ANY_THROW(m.at(0, 3));
}

TEST(TCRSMatrix, compare_equal_matrices_return_true)
{
    vector<vector<int>> dense = {
         {0, 1},
         {2, 0}
    };
    TCRSMatrix<int> m(dense), m1(dense);
    EXPECT_EQ(m, m1);
}

TEST(TCRSMatrix, compare_matrix_with_itself_return_true)
{
    TCRSMatrix<int> m(3, 3);
    EXPECT_EQ(m, m);
}

TEST(TCRSMatrix, matrices_with_different_size_are_not_equal)
{
    TCRSMatrix<int> m(3, 3), m1(2, 2);
    EXPECT_FALSE(m == m1);
}

TEST(TCRSMatrix, matrices_with_same_size_but_different_elements_are_not_equal)
{
    vector<vector<int>> dense = {
         {0, 1},
         {2, 0}
    };
    vector<vector<int>> dense1 = {
         {1, 0},
         {0, 2}
    };
    TCRSMatrix<int> m(dense), m1(dense1);
    EXPECT_FALSE(m == m1);
}

TEST(TCRSMatrix, can_transpose_square_matrix)
{
    vector<vector<int>> dense = {
         {0, 1, 0, 0},
         {2, 0, 1, 0},
         {3, 0, 0, 0},
         {0, 0, 0, 4}
    };
    TCRSMatrix<int> m(dense);
    TCRSMatrix<int> mt = m.transpose();

    vector<vector<int>> transp = {
         {0, 2, 3, 0},
         {1, 0, 0, 0},
         {0, 1, 0, 0},
         {0, 0, 0, 4}
    };
    TCRSMatrix<int> t(transp);
    EXPECT_EQ(mt, t);
}

TEST(TCRSMatrix, can_transpose_rectangular_matrix)
{
    vector<vector<int>> dense = {
         {1, 0, 0},
         {0, 2, 0}
    };
    TCRSMatrix<int> m(dense);
    TCRSMatrix<int> mt = m.transpose();

    vector<vector<int>> transp = {
        {1, 0},
        {0, 2},
        {0, 0}
    };
    TCRSMatrix<int> t(transp);
    EXPECT_EQ(mt, t);
}

TEST(TCRSMatrix, double_transpose_returns_original_matrix)
{
    vector<vector<int>> dense = {
         {1, 0, 0},
         {0, 2, 0}
    };
    TCRSMatrix<int> m(dense);
    TCRSMatrix<int> mt = m.transpose();
    TCRSMatrix<int> mtt = mt.transpose();
    EXPECT_EQ(mtt, m);
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
    vector<int> v = { 1, 2, 1, 2 };
    vector<int> res = { 2, 3, 3, 8 };
    vector<int> r = m * v;
    EXPECT_EQ(res, r);
}

TEST(TCRSMatrix, cant_multiply_matrix_by_vector_of_incompatible_size)
{
    TCRSMatrix<int> m(3, 3);
    vector<int> v = { 1, 2 };
    ASSERT_ANY_THROW(m*v);
}

TEST(TCRSMatrix, multiply_zero_matrix_by_vector_returns_zero_vector)
{
    TCRSMatrix<int> m(3, 3);
    vector<int> v = { 1, 2, 1 };
    vector<int> res = { 0, 0, 0 };
    vector<int> r = m * v;
    EXPECT_EQ(res, r);
}

TEST(TCRSMatrix, can_multiply_square_crs_matrices_of_equal_size)
{
    vector<vector<int>> dense = {
         {0, 1, 0, 0},
         {2, 0, 1, 0},
         {3, 0, 0, 0},
         {0, 0, 0, 4}
    };
    vector<vector<int>> dense1 = {
         {0, 2, 3, 0},
         {1, 0, 0, 0},
         {0, 1, 0, 0},
         {0, 0, 0, 4}
    };
    TCRSMatrix<int> m(dense);
    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> b = m * m1;

    vector<vector<int>> res = {
         {1, 0, 0, 0},
         {0, 5, 6, 0},
         {0, 6, 9, 0},
         {0, 0, 0, 16}
    };
    TCRSMatrix<int> r(res);
    EXPECT_EQ(r, b);
}

TEST(TCRSMatrix, can_multiply_rectangular_crs_matrices_of_compatible_size)
{
    vector<vector<int>> dense = {
        {1, 2, 0},
        {0, 1, 1}
    };
    vector<vector<int>> dense1 = {
        {1, 0},
        {0, 2},
        {1, 1}
    };
    TCRSMatrix<int> m(dense);
    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> b = m * m1;

    vector<vector<int>> res = {
        {1, 4},
        {1, 3}
    };
    TCRSMatrix<int> r(res);
    EXPECT_EQ(r, b);
}

TEST(TCRSMatrix, cant_multiply_crs_matrices_with_incompatible_sizes)
{
    TCRSMatrix<int> m(3, 3);
    TCRSMatrix<int> m1(2, 4);
    ASSERT_ANY_THROW(m * m1);
}

TEST(TCRSMatrix, multiply_by_identity_matrix_returns_original_matrix)
{
    vector<vector<int>> dense = {
         {0, 1, 0, 0},
         {2, 0, 1, 0},
         {3, 0, 0, 0},
         {0, 0, 0, 4}
    };
    vector<vector<int>> id = {
         {1, 0, 0, 0},
         {0, 1, 0, 0},
         {0, 0, 1, 0},
         {0, 0, 0, 1}
    };
    TCRSMatrix<int> m(dense);
    TCRSMatrix<int> i(id);
    TCRSMatrix<int> r = i * m;
    EXPECT_EQ(r, m);
}

TEST(TCRSMatrix, multiply_matrix_with_empty_rows_works_correctly)
{
    vector<vector<int>> dense = {
        {1, 2},
        {0, 0}
    };
    vector<vector<int>> dense1 = {
        {3, 0},
        {0, 4}
    };
    TCRSMatrix<int> m(dense);
    TCRSMatrix<int> m1(dense1);
    
    vector<vector<int>> res = {
        {3, 8},
        {0, 0}
    };
    TCRSMatrix<int> r(res);
    TCRSMatrix<int> result = m * m1;
    EXPECT_EQ(result, r);
}