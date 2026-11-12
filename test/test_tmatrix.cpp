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
        {5, 6, 7},
        {2, 0, 3, 4}
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
    EXPECT_EQ(vector<int>({ 1, 2, 3, 4 }), m.getValues());
    EXPECT_EQ(vector<size_t>({ 0, 2, 0, 1 }), m.getColIndices());
    EXPECT_EQ(vector<size_t>({ 0, 2, 2, 4 }), m.getRowPtr());
}

TEST(TCRSMatrix, can_convert_crs_matrix_back_to_dense)
{
    vector<vector<int>> dense = {
      {1, 0, 2},
      {0, 7, 0},
      {0, 4, 0}
    };
    TCRSMatrix<int> m(dense);
    vector<vector<int>> matrix = m.toDense();
     
    ASSERT_EQ(dense.size(), matrix.size());
    for (size_t i = 0; i < dense.size(); i++) {
        ASSERT_EQ(dense[i].size(), matrix[i].size());
        for (size_t j = 0; j < dense[i].size(); j++) {
            ASSERT_EQ(dense[i][j], matrix[i][j]);
        }
    }
}

TEST(TCRSMatrix, can_create_copied_matrix)
{
  TCRSMatrix<int> m(5);

  ASSERT_NO_THROW(TCRSMatrix<int> m1(m));
}

TEST(TCRSMatrix, copied_matrix_is_equal_to_source_one)
{
    vector<vector<int>> dense = {
       {4, 6, 6},
       {0, 7, 0},
       {3, 0, 0}
    };
    TCRSMatrix<int> m(dense);
    TCRSMatrix<int> m1(m);
    EXPECT_TRUE(m1 == m);
}

TEST(TCRSMatrix, copied_matrix_has_its_own_memory)
{
    vector<vector<int>> dense = {
         {1, 1, 0},
         {0, 1, 0},
         {3, 0, 21}
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
           {1, 0, 1, 0},
           {0, 1, 4, 0},
           {3, 0, 6, 21}
    };
    TCRSMatrix<int> m(dense);
    ASSERT_NO_THROW(m = m);
    vector<vector<int>> matrix = m.toDense();
    ASSERT_EQ(dense, matrix);
}

TEST(TCRSMatrix, can_assign_matrices_of_equal_size)
{
    vector<vector<int>> dense1 = {
             {1, 0, 1, 0},
             {88, 1, 4, 0},
             {0, 0, 6, 1}
    };
    vector<vector<int>> dense2 = {
           {0, 67, 1, 0},
           {0, 0, 4, 0},
           {3, 22, 6, 21}
    };
    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);
    m1 = m2;
    vector<vector<int>> matrix = m1.toDense();
    ASSERT_EQ(dense2, matrix);
}

TEST(TCRSMatrix, assign_operator_change_matrix_size)
{
    vector<vector<int>> dense1 = {
           {1, 1, 1},
           {0, 1, 0},
           {1, 1, 2}
    };
    vector<vector<int>> dense2 = {
         {1, 1},
         {1, 0}
    };
    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);
    ASSERT_EQ(m1.getRows(), 3);
    ASSERT_EQ(m1.getCols(), 3);
    m1 = m2;
    ASSERT_EQ(m1.getRows(), 2);
    ASSERT_EQ(m1.getCols(), 2);
}

TEST(TCRSMatrix, can_assign_matrices_of_different_size)
{
    vector<vector<int>> dense1 = {
             {1, 1},
             {0, 1},
             {1, 2}
    };
    vector<vector<int>> dense2 = {
         {1, 1},
         {1, 0}
    };
    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);
    ASSERT_NO_THROW(m1 = m2);
    ASSERT_EQ(m1.getRows(), 2);
    ASSERT_EQ(m1.getCols(), 2);
}

TEST(TCRSMatrix, can_get_rows_cols_and_nnz)
{
    vector<vector<int>> dense = {
               {1, 0, 1, 0},
               {88, 1, 4, 0},
               {0, 0, 6, 1}
    };
    TCRSMatrix<int> m(dense);
    EXPECT_EQ(m.getRows(), 3);
    EXPECT_EQ(m.getCols(), 4);
    EXPECT_EQ(m.getNNZ(), 7);
}

TEST(TCRSMatrix, can_get_nonzero_and_zero_elements_by_at)
{
    vector<vector<int>> dense = {
               {1, 1},
               {0, 1},
               {1, 2}
    };
    TCRSMatrix<int> m(dense);
    EXPECT_EQ(m.at(0, 0), 1);
    EXPECT_EQ(m.at(0, 1), 1); 
    EXPECT_EQ(m.at(1, 0), 0);
    EXPECT_EQ(m.at(1, 1), 1);
    EXPECT_EQ(m.at(2, 0), 1);
    EXPECT_EQ(m.at(2, 1), 2);
}

TEST(TCRSMatrix, throws_when_get_element_with_negative_or_too_large_index)
{
    vector<vector<int>> dense = {
             {4, 0, 0},
             {0, 5, 0},
             {0, 0, 6}
    };
    TCRSMatrix<int> m(dense);
    ASSERT_ANY_THROW(m.at(5, 31));
    ASSERT_ANY_THROW(m.at(0, 10));
    ASSERT_ANY_THROW(m.at(10, 0));
}

TEST(TCRSMatrix, compare_equal_matrices_return_true)
{
    vector<vector<int>> dense = {
               {4, 0, 0},
               {0, 5, 0},
               {0, 0, 6}
    };
    TCRSMatrix<int> m(dense);
    TCRSMatrix<int> m1(dense);
    EXPECT_TRUE(m == m1);
}

TEST(TCRSMatrix, compare_matrix_with_itself_return_true)
{
    vector<vector<int>> dense = {
                 {4, 1, 9},
                 {3, 5, 1},
                 {1, 1, 6}
    };
    TCRSMatrix<int> m(dense);
    EXPECT_TRUE(m == m);
}

TEST(TCRSMatrix, matrices_with_different_size_are_not_equal)
{
    vector<vector<int>> dense1 = {
               {1, 1},
               {0, 1},
               {1, 2}
    };
    vector<vector<int>> dense2 = {
         {1, 1},
         {1, 0}
    };
    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);
    EXPECT_FALSE(m1 == m2);
}

TEST(TCRSMatrix, matrices_with_same_size_but_different_elements_are_not_equal)
{
  vector<vector<int>> dense1 = {
               {1, 1},
               {1, 0}
  };
  vector<vector<int>> dense2 = {
       {5, 1},
       {1, 0}
  };
  TCRSMatrix<int> m1(dense1);
  TCRSMatrix<int> m2(dense2);
  EXPECT_FALSE(m1 == m2);
}

TEST(TCRSMatrix, can_transpose_square_matrix)
{
    vector<vector<int>> dense = {
                   {0, 0, 9},
                   {3, 0, 1},
                   {11, 0, 62}
    };
    TCRSMatrix<int> m(dense);
    TCRSMatrix<int> t = m.transpose();
    vector<vector<int>> m1 = {
                   {0, 3, 11},
                   {0, 0, 0},
                   {9, 1, 62}
    };
    vector<vector<int>> m2 = t.toDense();
    EXPECT_EQ(m1, m2);
}

TEST(TCRSMatrix, can_transpose_rectangular_matrix)
{
    vector<vector<int>> dense = {
               {4, 0},
               {0, 1},
               {1, 2}
    };
    TCRSMatrix<int> m(dense);
    TCRSMatrix<int> t = m.transpose();
    vector<vector<int>> m1 = {
                   {4, 0, 1},
                   {0, 1, 2}
    };
    vector<vector<int>> m2 = t.toDense();
    EXPECT_EQ(m1, m2);

}

TEST(TCRSMatrix, double_transpose_returns_original_matrix)
{
    vector<vector<int>> dense = {
                     {0, 0, 9},
                     {3, 0, 1},
                     {11, 0, 62}
    };
    TCRSMatrix<int> m(dense);
    TCRSMatrix<int> t1 = m.transpose();
    TCRSMatrix<int> t2 = t1.transpose();
    EXPECT_TRUE(m == t2);
}

TEST(TCRSMatrix, can_multiply_matrix_by_vector_of_compatible_size)
{
    vector<vector<int>> dense = {
                 {1, 1},
                 {0, 1},
                 {1, 2}
    };
    TCRSMatrix<int> m(dense);
    vector<int> v = { 0, 4};
    vector<int> res1 = m * v;
    vector<int> res2 = { 4, 4, 8 };
    EXPECT_EQ(res1, res2);
}

TEST(TCRSMatrix, cant_multiply_matrix_by_vector_of_incompatible_size)
{
    vector<vector<int>> dense = {
                 {1, 0, 1, 0},
                 {88, 1, 4, 0},
                 {0, 0, 6, 1}
    };
    TCRSMatrix<int> m(dense);
    vector<int> v = { 0, 4, 3, 8, 9, 10, 6};
    ASSERT_ANY_THROW(m * v);
}

TEST(TCRSMatrix, multiply_zero_matrix_by_vector_returns_zero_vector)
{
    TCRSMatrix<int> m(3, 3);
    vector<int> v = {8, 10, 6};
    vector<int> res1 = m * v;
    vector<int> res2 = { 0, 0, 0 };
    EXPECT_EQ(res1, res2);
}

TEST(TCRSMatrix, can_multiply_square_crs_matrices_of_equal_size)
{
    vector<vector<int>> dense1 = {
                       {0, 0, 9},
                       {3, 0, 1},
                       {1, 0, 7}
    };
    vector<vector<int>> dense2 = {
                       {0, 3, 2},
                       {0, 0, 1},
                       {1, 0, 1}
    };
    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);
    TCRSMatrix<int> res = m1 * m2;
    vector<vector<int>> res1 = {
                       {9, 0, 9},
                       {1, 9, 7},
                       {7, 3, 9}
    };
    vector<vector<int>> res2 = res.toDense();
    EXPECT_EQ(res1, res2);
}

TEST(TCRSMatrix, can_multiply_rectangular_crs_matrices_of_compatible_size)
{
    vector<vector<int>> dense1 = {
                         {0, 0, 2},
                         {1, 0, 7}
    };
    vector<vector<int>> dense2 = {
                       {0, 3},
                       {0, 0},
                       {1, 0}
    };
    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);
    TCRSMatrix<int> res = m1 * m2;
    vector<vector<int>> res1 = {
                       {2, 0},
                       {7, 3}
    };
    EXPECT_EQ(res.getRows(), 2);
    EXPECT_EQ(res.getCols(), 2);
    vector<vector<int>> res2 = res.toDense();
    EXPECT_EQ(res1, res2);
}

TEST(TCRSMatrix, cant_multiply_crs_matrices_with_incompatible_sizes)
{
    vector<vector<int>> dense1 = {
                       {0, 9},
                       {3, 0},
                       {0, 7}
    };
    vector<vector<int>> dense2 = {
                       {0, 3, 2},
                       {0, 0, 1},
                       {1, 0, 1}
    };
    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);
    ASSERT_ANY_THROW(m1 * m2);
}

TEST(TCRSMatrix, multiply_by_identity_matrix_returns_original_matrix)
{
    vector<vector<int>> dense1 = {
                       {0, 0, 9},
                       {3, 0, 1},
                       {11, 0, 62}
    };
    vector<vector<int>> dense2 = {
                     {1, 0, 0},
                     {0, 1, 0},
                     {0, 0, 1}
    };
    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);
    TCRSMatrix<int> res = m1 * m2;
    EXPECT_TRUE(m1 == res);
}

TEST(TCRSMatrix, multiply_matrix_with_empty_rows_works_correctly)
{
    vector<vector<int>> dense1 = {
                           {0, 0, 9},
                           {0, 0, 0},
                           {1, 0, 2}
    };
    vector<vector<int>> dense2 = {
                     {1, 0, 0},
                     {2, 0, 0},
                     {0, 0, 1}
    };
    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);
    TCRSMatrix<int> res = m1 * m2;
    vector<vector<int>> res1 = {
                     {0, 0, 9},
                     {0, 0, 0},
                     {1, 0, 2}
    };
    EXPECT_EQ(res.getRows(), 3);
    EXPECT_EQ(res.getCols(), 3);
    vector<vector<int>> res2 = res.toDense();
    EXPECT_EQ(res1, res2);
}
