#include "tmatrix.h"

#include <gtest.h>
#include <sstream>

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

    vector<vector<int>> empty_rows = { {}, {} };
    ASSERT_ANY_THROW(TCRSMatrix<int> m(empty_rows));

    vector<vector<int>> inconsistent = {
        {1, 2},
        {3}
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

    TCRSMatrix<int> tmp(dense);

    vector<int> expected_values = { 1,2,3,4 };
    vector<size_t> expected_col_index = { 0,2,0,1 };
    vector<size_t> expected_row_ptr = { 0,2,2,4 };

    EXPECT_EQ(tmp.getValues(), expected_values);
    EXPECT_EQ(tmp.getColIndices(), expected_col_index);
    EXPECT_EQ(tmp.getRowPtr(), expected_row_ptr);


}

TEST(TCRSMatrix, can_convert_crs_matrix_back_to_dense)
{
    vector<int> values = { 1,2,3,4 };
    vector<size_t> col_index = { 0,2,0,1 };
    vector<size_t> row_ptr = { 0,2,2,4 };
    
    TCRSMatrix<int> tmp(3, 3, values, col_index, row_ptr);
    vector<vector<int>> matrix_tmp = tmp.toDense();

    vector<vector<int>> expection_matrix = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
    };

    EXPECT_EQ(matrix_tmp, expection_matrix);
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
    TCRSMatrix<int> m1(m);

    EXPECT_EQ(m, m1);
}

TEST(TCRSMatrix, copied_matrix_has_its_own_memory)
{
    vector<vector<int>> dense = {
        {1, 0, 2},
        {0, 0, 0},
        {3, 4, 0}
    };

    TCRSMatrix<int> m(dense);
    TCRSMatrix<int> m1(m);

    EXPECT_NE(m.getValues().data(), m1.getValues().data());
    EXPECT_NE(m.getColIndices().data(), m1.getColIndices().data());
    EXPECT_NE(m.getRowPtr().data(), m1.getRowPtr().data());
}

TEST(TCRSMatrix, can_assign_matrix_to_itself)
{
    vector<int> values = { 1,2,3,4 };
    vector<size_t> col_index = { 0,2,0,1 };
    vector<size_t> row_ptr = { 0,2,2,4 };

    TCRSMatrix<int> tmp(3, 3, values, col_index, row_ptr);
    tmp = tmp;

    EXPECT_EQ(tmp.getRows(), 3);
    EXPECT_EQ(tmp.getCols(), 3);
    EXPECT_EQ(tmp.getValues(), values);
    EXPECT_EQ(tmp.getColIndices(), col_index);
    EXPECT_EQ(tmp.getRowPtr(), row_ptr);

}

TEST(TCRSMatrix, can_assign_matrices_of_equal_size)
{
    vector<vector<int>> dense1 = {
      {1, 7, 2},
      {2, 0, 0},
      {3, 4, 0}
    };
    vector<vector<int>> dense2 = {
    {0, 0, 2},
    {0, 1, 0},
    {3, 4, 5}
    };

    TCRSMatrix<int> tmp1(dense1);
    TCRSMatrix<int> tmp2(dense2);

    EXPECT_NO_THROW(tmp1 = tmp2);
    EXPECT_EQ(tmp1, tmp2);
}

TEST(TCRSMatrix, assign_operator_change_matrix_size)
{
    vector<vector<int>> dense1 = {
        {1, 7},
        {2, 0},
        {3, 4}
    };
    vector<vector<int>> dense2 = {
    {0, 0, 2},
    {0, 1, 0},
    };

    TCRSMatrix<int> tmp1(dense1);
    TCRSMatrix<int> tmp2(dense2);

    tmp2 = tmp1;

    EXPECT_EQ(tmp2.getCols(), tmp1.getCols());
    EXPECT_EQ(tmp2.getRows(), tmp1.getRows());


}

TEST(TCRSMatrix, can_assign_matrices_of_different_size)
{
    vector<vector<int>> dense1 = {
          {1, 7, 2},
          {2, 0, 0},
    };
    vector<vector<int>> dense2 = {
    {0, 0},
    {0, 1},
    {3, 4}
    };

    TCRSMatrix<int> tmp1(dense1);
    TCRSMatrix<int> tmp2(dense2);

    EXPECT_NO_THROW(tmp1 = tmp2);
    EXPECT_EQ(tmp1, tmp2);
}

TEST(TCRSMatrix, can_get_rows_cols_and_nnz)
{
    
    vector<int> values = { 1,2,3,4 };
    vector<size_t> col_index = { 0,2,0,1 };
    vector<size_t> row_ptr = { 0,2,2,4,4 };

    TCRSMatrix<int> tmp(4, 3, values, col_index, row_ptr);

   
    EXPECT_EQ(tmp.getCols(), 3);
    EXPECT_EQ(tmp.getRows(), 4);
    EXPECT_EQ(tmp.getNNZ(), 4);



}

TEST(TCRSMatrix, can_get_nonzero_and_zero_elements_by_at)
{
    vector<vector<int>> dense = {
        {1, 0, 2},
        {0, 0, 0},
        {3, 4, 0}
    };

    TCRSMatrix<int> tmp(dense);
    
    EXPECT_EQ(tmp.at(0, 2), 2);
    EXPECT_EQ(tmp.at(1, 0), 0);

}

TEST(TCRSMatrix, throws_when_get_element_with_negative_or_too_large_index)
{
    vector<vector<int>> dense = {
          {1, 0, 2},
          {0, 0, 0},
          {3, 4, 0}
    };

    TCRSMatrix<int> tmp(dense);

    EXPECT_ANY_THROW(tmp.at(-1, 1));
    EXPECT_ANY_THROW(tmp.at(1, -1));
    EXPECT_ANY_THROW(tmp.at(1000000, 1));
    EXPECT_ANY_THROW(tmp.at(1, 10000000));

}

TEST(TCRSMatrix, compare_equal_matrices_return_true)
{
    vector<vector<int>> dense = {
              {1, 0, 2},
              {0, 0, 0},
              {3, 4, 0}
    };
    TCRSMatrix<int> tmp1(dense);
    TCRSMatrix<int> tmp2(dense);

    EXPECT_TRUE(tmp1 == tmp2);

}

TEST(TCRSMatrix, compare_matrix_with_itself_return_true)
{
    vector<vector<int>> dense = {
                  {1, 0, 2},
                  {0, 0, 0},
                  {3, 4, 0}
    };
    TCRSMatrix<int> tmp(dense);

    EXPECT_TRUE(tmp == tmp);

}

TEST(TCRSMatrix, matrices_with_different_size_are_not_equal)
{
    vector<vector<int>> dense1 = {
              {1, 7, 2},
              {2, 0, 0},
    };
    vector<vector<int>> dense2 = {
    {0, 0},
    {0, 1},
    {3, 4}
    };

    TCRSMatrix<int> tmp1(dense1);
    TCRSMatrix<int> tmp2(dense2);

    EXPECT_FALSE(tmp1 == tmp2);
}

TEST(TCRSMatrix, matrices_with_same_size_but_different_elements_are_not_equal)
{
    vector<vector<int>> dense1 = {
                  {1, 0, 2},
                  {0, 0, 0},
                  {3, 4, 0}
    }; vector<vector<int>> dense2 = {
                  {1, 0, 2},
                  {0, 5, 0},
                  {3, 4, 0}
    };
    TCRSMatrix<int> tmp1(dense1);
    TCRSMatrix<int> tmp2(dense2);

    EXPECT_FALSE(tmp1 == tmp2);

}

TEST(TCRSMatrix, can_transpose_square_matrix)
{
    vector<vector<int>> dense1 = {
                {1, 0, 2},
                {0, 0, 0},
                {3, 4, 0}
    }; vector<vector<int>> dense2 = {
                  {1, 0, 3},
                  {0, 0, 4},
                  {2, 0, 0}
    };
    TCRSMatrix<int> tmp1(dense1);
    TCRSMatrix<int> tmp2(dense2);
    
    tmp1 = tmp1.transpose();

    EXPECT_EQ(tmp1,tmp2);
}

TEST(TCRSMatrix, can_transpose_rectangular_matrix)
{
    vector<vector<int>> dense1 = {
                    {1, 0, 2},
                    {3, 4, 0}
    }; vector<vector<int>> dense2 = {
                  {1, 3},
                  {0, 4},
                  {2, 0}
    };
    TCRSMatrix<int> tmp1(dense1);
    TCRSMatrix<int> tmp2(dense2);

    tmp1 = tmp1.transpose();

    EXPECT_EQ(tmp1, tmp2);
}

TEST(TCRSMatrix, double_transpose_returns_original_matrix)
{
    vector<vector<int>> dense = {
                      {1, 0, 2},
                      {0, 0, 0},
                      {3, 4, 0}
    };
    TCRSMatrix<int> tmp1(dense);
    TCRSMatrix<int> tmp2 = tmp1;

    EXPECT_EQ((tmp2.transpose()).transpose(), tmp1);
}

TEST(TCRSMatrix, can_multiply_matrix_by_vector_of_compatible_size)
{
    vector<vector<int>> dense = {
                      {1, 0, 2},
                      {0, 0, 0},
                      {3, 4, 0}
    };
    vector<int> vec = { 3,1,2 };
    vector<int> exp_rex = { 7,0,13 };

    TCRSMatrix<int> tmp(dense);

    EXPECT_EQ(tmp * vec, exp_rex);
}

TEST(TCRSMatrix, cant_multiply_matrix_by_vector_of_incompatible_size)
{
    vector<vector<int>> dense = {
                          {1, 0, 2},
                          {0, 0, 0},
                          {3, 4, 0}
    };
    vector<int> vec = { 3,2 };

    TCRSMatrix<int> tmp(dense);

    EXPECT_ANY_THROW(tmp * vec);
}

TEST(TCRSMatrix, multiply_zero_matrix_by_vector_returns_zero_vector)
{
    vector<vector<int>> dense = {
                          {0, 0, 0},
                          {0, 0, 0},
                          {0, 0, 0}
    };
    vector<int> vec = { 3,1,2 };
    vector<int> exp_rex = { 0,0,0 };

    TCRSMatrix<int> tmp(dense);

    EXPECT_EQ(tmp * vec, exp_rex);
}

TEST(TCRSMatrix, can_multiply_square_crs_matrices_of_equal_size)
{
    vector<vector<int>> dense1 = {
                          {1, 0, 2},
                          {0, 0, 0},
                          {3, 4, 0}
    };
    vector<vector<int>> dense2 = {
                      {1, 0, 2},
                      {0, 6, 0},
                      {3, 4, 0}
    };
    vector<vector<int>> exp_matrix = {
                      {7, 8, 2},
                      {0, 0, 0},
                      {3, 24, 6}
    };
    TCRSMatrix<int> tmp1(dense1);
    TCRSMatrix<int> tmp2(dense2);
    TCRSMatrix<int> exp_mat(exp_matrix);

    EXPECT_EQ(tmp1 * tmp2, exp_mat);

}

TEST(TCRSMatrix, can_multiply_rectangular_crs_matrices_of_compatible_size)
{
    vector<vector<int>> dense1 = {
                              {1, 0, 2},
                              {0, 0, 0},
    };
    vector<vector<int>> dense2 = {
                      {1, 0},
                      {0, 6},
                      {3, 4}
    };
    vector<vector<int>> exp_matrix = {
                      {7, 8},
                      {0, 0},    
    };
    TCRSMatrix<int> tmp1(dense1);
    TCRSMatrix<int> tmp2(dense2);
    TCRSMatrix<int> exp_mat(exp_matrix);

    EXPECT_EQ(tmp1 * tmp2, exp_mat);

}

TEST(TCRSMatrix, cant_multiply_crs_matrices_with_incompatible_sizes)
{
    vector<vector<int>> dense1 = {
                      {1, 0, 2},
                      {0, 0, 0},
    };
    vector<vector<int>> dense2 = {
                      {1, 0, 2},
                      {0, 6, 0},
    };
   
    TCRSMatrix<int> tmp1(dense1);
    TCRSMatrix<int> tmp2(dense2);

    EXPECT_ANY_THROW(tmp1 * tmp2);
}

TEST(TCRSMatrix, multiply_by_identity_matrix_returns_original_matrix)
{
    vector<vector<int>> dense1 = {
                          {1, 0, 2},
                          {0, 0, 0},
                          {3, 4, 0}
    };
    vector<vector<int>> dense2 = {
                      {1, 0, 0},
                      {0, 1, 0},
                      {0, 0, 1}
    };
    
    TCRSMatrix<int> tmp1(dense1);
    TCRSMatrix<int> tmp2(dense2);

    EXPECT_EQ(tmp1 * tmp2, tmp1);
    EXPECT_EQ(tmp2 * tmp1, tmp1);

}

TEST(TCRSMatrix, multiply_matrix_with_empty_rows_works_correctly)
{
    vector<vector<int>> dense1 = {
        {1, 5, 2},
        {0, 3, 1}
    };

    vector<vector<int>> dense2 = {
        {1, 0},
        {0, 0},  
        {3, 4}
    };

    vector<vector<int>> expected = {
        {7, 8},
        {3, 4}
    };

    TCRSMatrix<int> tmp1(dense1);
    TCRSMatrix<int> tmp2(dense2);
    TCRSMatrix<int> result(expected);

    EXPECT_EQ(tmp1*tmp2, result);
}

TEST(TCRSMatrix, incorect_values_col_index_row_ptr) {
        
    vector<int> values1 = { 1,2,3,4 };
    vector<size_t> col_index1 = { 0,2,0,1,0 };
    vector<size_t> row_ptr1 = { 0,2,2,4,4 };

    vector<int> values2 = { 1,2,3,4,6 };
    vector<size_t> col_index2 = { 0,2,0,1 };
    vector<size_t> row_ptr2 = { 0,2,2,4,4 };

    vector<int> values3 = { 1,2,3,4 };
    vector<size_t> col_index3 = { 0,2,0,1 };
    vector<size_t> row_ptr3 = { 0,2,2,6,4 };

    vector<int> values4 = { 1,2,3,4 };
    vector<size_t> col_index4 = { 0,2,0,1 };
    vector<size_t> row_ptr4 = { 0,2,2,4,4,6 };

    vector<int> values5 = { 1,2,3,4 };
    vector<size_t> col_index5 = { 0,10,0,1 };
    vector<size_t> row_ptr5 = { 0,2,2,4,4 };


   EXPECT_ANY_THROW(TCRSMatrix<int> tmp(4, 3, values1, col_index1, row_ptr1));
   EXPECT_ANY_THROW(TCRSMatrix<int> tmp(4, 3, values2, col_index2, row_ptr2));
   EXPECT_ANY_THROW( TCRSMatrix<int> tmp(4, 3, values3, col_index3, row_ptr3));
   EXPECT_ANY_THROW(TCRSMatrix<int> tmp(4, 3, values4, col_index4, row_ptr4));
   EXPECT_ANY_THROW(TCRSMatrix<int> tmp(4, 3, values5, col_index5, row_ptr5));

}

TEST(TCRSMatrix, can_read_matrix_from_stream)
{
    istringstream input(
        "3 3 4\n"
        "1 2 3 4\n"
        "0 2 0 1\n"
        "0 2 2 4\n"
    );

    vector<vector<int>> dense = {
        {1, 0, 2},
        {0, 0, 0},
        {3, 4, 0}
    };

    TCRSMatrix<int> tmp;
    TCRSMatrix<int> exp_mat(dense);

    EXPECT_NO_THROW(input >> tmp);
    EXPECT_EQ(tmp, exp_mat);
}

TEST(TCRSMatrix, can_write_matrix_to_stream)
{
    vector<vector<int>> dense = {
        {1, 0, 2},
        {0, 0, 0},
        {3, 4, 0}
    };

    TCRSMatrix<int> tmp(dense);
    ostringstream output;

    string exp_res =
        "Values: 1 2 3 4 \n"
        "Col Index: 0 2 0 1 \n"
        "Row ptr: 0 2 2 4 \n";

    EXPECT_NO_THROW(output << tmp);
    EXPECT_EQ(output.str(), exp_res);
}

TEST(TCRSMatrix, throws_when_read_matrix_with_incorrect_size)
{
    istringstream input("0 3 0");
    TCRSMatrix<int> tmp;

    EXPECT_ANY_THROW(input >> tmp);
}
