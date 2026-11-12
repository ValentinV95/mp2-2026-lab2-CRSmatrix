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
    vector<vector<int>> dense = {
    {1, 0, 2, 0},
    {0, 7, 0},
    {3, 0, 5, 0},
    {0, 6, 0, 8}
    };

    vector<vector<int>> empty_dense = {};
    vector<vector<int>> empty_row_dense = { {} };
    ASSERT_ANY_THROW(TCRSMatrix<int> m(empty_dense));
    ASSERT_ANY_THROW(TCRSMatrix<int> m1(dense));
    ASSERT_ANY_THROW(TCRSMatrix<int> m2(empty_row_dense));
}

TEST(TCRSMatrix, crs_arrays_are_formed_correctly_from_dense)
{
    vector<vector<int>> dense = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };
    TCRSMatrix<int> matrix(dense);

    vector<int> expected_values = { 1, 2, 6, 3, 7, 8, 6, 8 };
    vector<size_t> expected_col_index = { 0, 2, 4, 2, 4, 5, 4, 6 };
    vector<size_t> expected_row_ptr = { 0, 3, 3, 6, 8 };

    EXPECT_EQ(matrix.getNNZ(), 8);
    EXPECT_EQ(matrix.getRows(), 4);
    EXPECT_EQ(matrix.getCols(), 7);

    EXPECT_EQ(matrix.getValues(), expected_values);
    EXPECT_EQ(matrix.getColIndices(), expected_col_index);
    EXPECT_EQ(matrix.getRowPtr(), expected_row_ptr);
}

TEST(TCRSMatrix, can_convert_crs_matrix_back_to_dense)
{
    vector<vector<int>> dense = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };

    vector<int> CRS_values = { 1, 2, 6, 3, 7, 8, 6, 8 };
    vector<size_t> CRS_col_index = { 0, 2, 4, 2, 4, 5, 4, 6 };
    vector<size_t> CRS_row_ptr = { 0, 3, 3, 6, 8 };

    TCRSMatrix<int> matrix(4, 7, CRS_values, CRS_col_index, CRS_row_ptr);

    EXPECT_EQ(matrix.toDense(), dense);
}

TEST(TCRSMatrix, can_create_copied_matrix)
{
  TCRSMatrix<int> m(5);

  ASSERT_NO_THROW(TCRSMatrix<int> m1(m));
}

TEST(TCRSMatrix, copied_matrix_is_equal_to_source_one)
{
    vector<vector<int>> dense = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };

    TCRSMatrix<int> matrix(dense);

    TCRSMatrix<int> m(matrix);

    EXPECT_EQ(m.getNNZ(), matrix.getNNZ());
    EXPECT_EQ(m.getRows(), matrix.getRows());
    EXPECT_EQ(m.getCols(), matrix.getCols());
    EXPECT_EQ(m.toDense(), dense);
    EXPECT_EQ(m, matrix);
}

TEST(TCRSMatrix, copied_matrix_has_its_own_memory)
{
    vector<vector<int>> dense = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };

    TCRSMatrix<int> matrix(dense);

    TCRSMatrix<int> m(matrix);

    matrix = TCRSMatrix<int>(3, 6);

    EXPECT_EQ(m.toDense(), dense);
    EXPECT_EQ(m.getRows(), 4);
    EXPECT_EQ(m.getCols(), 7);
    EXPECT_EQ(m.getNNZ(), 8);

    EXPECT_NE(m.getNNZ(), matrix.getNNZ());
    EXPECT_NE(m.getRows(), matrix.getRows());
    EXPECT_NE(m.getCols(), matrix.getCols());
}

TEST(TCRSMatrix, can_assign_matrix_to_itself)
{
    vector<vector<int>> dense = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };
    TCRSMatrix<int> m(dense);

    ASSERT_NO_THROW(m = m);
}

TEST(TCRSMatrix, can_assign_matrices_of_equal_size)
{
    vector<vector<int>> dense1 = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };

    vector<vector<int>> dense2 = {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 7, 0, 0, 5, 0},
    {0, 0, 0, 0, 10, 9, 0},
    {0, 2, 0, 4, 0, 0, 30}
    };

    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);

    ASSERT_NE(m1, m2);

    m1 = m2;

    EXPECT_EQ(m1.getNNZ(), m2.getNNZ());
    EXPECT_EQ(m1.getRows(), m2.getRows());
    EXPECT_EQ(m1.getCols(), m2.getCols());

    EXPECT_EQ(m1, m2);
    EXPECT_EQ(m1.toDense(), dense2);
}

TEST(TCRSMatrix, assign_operator_change_matrix_size)
{
    vector<vector<int>> dense1 = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };

    vector<vector<int>> dense2 = {
    {0, 5, 0},
    {0, 0, 7}
    };

    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);

    ASSERT_NE(m1, m2);
    ASSERT_NE(m1.getRows(), m2.getRows());
    ASSERT_NE(m1.getCols(), m2.getCols());

    m1 = m2;

    EXPECT_EQ(m1, m2);
    EXPECT_EQ(m1.getNNZ(), m2.getNNZ());
    EXPECT_EQ(m1.getRows(), m2.getRows());
    EXPECT_EQ(m1.getCols(), m2.getCols());

    EXPECT_EQ(m1.toDense(), dense2);
}

TEST(TCRSMatrix, can_assign_matrices_of_different_size)
{
    vector<vector<int>> dense1 = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };

    vector<vector<int>> dense2 = {
    {0, 5, 0},
    {1, 0, 7},
    {0, 5, 0},
    {0, 0, 7},
    {0, 5, 0},
    {9, 0, 7},
    };

    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);

    ASSERT_NE(m1, m2);
    ASSERT_NE(m1.getRows(), m2.getRows());
    ASSERT_NE(m1.getCols(), m2.getCols());

    m1 = m2;

    EXPECT_EQ(m1, m2);
    EXPECT_EQ(m1.getNNZ(), m2.getNNZ());
    EXPECT_EQ(m1.getRows(), m2.getRows());
    EXPECT_EQ(m1.getCols(), m2.getCols());

    EXPECT_EQ(m1.toDense(), dense2);
}

TEST(TCRSMatrix, can_get_rows_cols_and_nnz)
{
    vector<vector<int>> dense = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };

    TCRSMatrix<int> m(dense);

    EXPECT_EQ(m.getNNZ(), 8);
	EXPECT_EQ(m.getRows(), 4);
	EXPECT_EQ(m.getCols(), 7);
}

TEST(TCRSMatrix, can_get_nonzero_and_zero_elements_by_at)
{
    vector<vector<int>> dense = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };

    TCRSMatrix<int> m(dense);

    ASSERT_NO_THROW(m.at(0, 4));
    EXPECT_EQ(m.at(0,4), 6);

	ASSERT_NO_THROW(m.at(1, 3));
    EXPECT_EQ(m.at(1, 3), 0);

}

TEST(TCRSMatrix, throws_when_get_element_with_negative_or_too_large_index)
{
    vector<vector<int>> dense = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };

    TCRSMatrix<int> m(dense);

    ASSERT_ANY_THROW(m.at(-1, 5));
    ASSERT_ANY_THROW(m.at(1, -5));
    ASSERT_ANY_THROW(m.at(-1, -5));

    ASSERT_ANY_THROW(m.at(4, 0));
    ASSERT_ANY_THROW(m.at(2, 7));
    ASSERT_ANY_THROW(m.at(4, 7));

    ASSERT_ANY_THROW(m.at(4, -5));
    ASSERT_ANY_THROW(m.at(-5, 7));
}

TEST(TCRSMatrix, compare_equal_matrices_return_true)
{
    vector<vector<int>> dense = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };

    TCRSMatrix<int> m1(dense);
    TCRSMatrix<int> m2(dense);

    EXPECT_EQ(m1, m2);

    EXPECT_TRUE(m1 == m2);
    EXPECT_FALSE(m2 != m1);
}

TEST(TCRSMatrix, compare_matrix_with_itself_return_true)
{
    vector<vector<int>> dense = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };

    TCRSMatrix<int> m1(dense);

    EXPECT_TRUE(m1 == m1);
    EXPECT_FALSE(m1 != m1);
}

TEST(TCRSMatrix, matrices_with_different_size_are_not_equal)
{
    vector<vector<int>> dense = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };

    vector<vector<int>> dense2 = {
    {0, 5, 0},
    {1, 0, 7},
    {0, 5, 0},
    {0, 0, 7},
    {0, 5, 0},
    {9, 0, 7},
    };

    TCRSMatrix<int> m1(dense);
    TCRSMatrix<int> m2(dense2);

    EXPECT_NE(m1, m2);

    EXPECT_FALSE(m1 == m2);
    EXPECT_TRUE(m2 != m1);
}

TEST(TCRSMatrix, matrices_with_same_size_but_different_elements_are_not_equal)
{
    vector<vector<int>> dense = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };

    vector<vector<int>> dense2 = {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 7, 0, 0, 5, 0},
    {0, 0, 0, 0, 10, 9, 0},
    {0, 2, 0, 4, 0, 0, 30}
    };

    TCRSMatrix<int> m1(dense);
    TCRSMatrix<int> m2(dense2);

    EXPECT_NE(m1, m2);

    EXPECT_FALSE(m1 == m2);
    EXPECT_TRUE(m2 != m1);
}

TEST(TCRSMatrix, can_transpose_square_matrix)
{
    vector<vector<int>> dense = {
    {1, 0, 2, 0},
    {0, 0, 0, 0},
    {0, 0, 3, 0},
    {0, 5, 0, 0},
    };

    vector<vector<int>> dense_transposed = {
    {1, 0, 0, 0},
    {0, 0, 0, 5},
    {2, 0, 3, 0},
    {0, 0, 0, 0}
    };

    TCRSMatrix<int> m(dense);
	TCRSMatrix<int> transposed = m.transpose();

    EXPECT_NE(m, transposed);
    EXPECT_EQ(transposed.toDense(), dense_transposed);
}

TEST(TCRSMatrix, can_transpose_rectangular_matrix)
{
    vector<vector<int>> dense = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };

    vector<vector<int>> dense_transposed = {
    {1, 0, 0, 0},
    {0, 0, 0, 0},
    {2, 0, 3, 0},
    {0, 0, 0, 0},
    {6, 0, 7, 6},
    {0, 0, 8, 0},
    {0, 0, 0, 8}
    };

    TCRSMatrix<int> m(dense);
    TCRSMatrix<int> transposed = m.transpose();

    EXPECT_NE(m, transposed);
    EXPECT_EQ(transposed.toDense(), dense_transposed);

    EXPECT_EQ(transposed.getNNZ(), 8);
    EXPECT_EQ(transposed.getRows(), 7);
    EXPECT_EQ(transposed.getCols(), 4);
}

TEST(TCRSMatrix, transpose_null_matrix)
{
    TCRSMatrix<int> m1(5);
    TCRSMatrix<int> m2(3, 10);
    TCRSMatrix<int> m1_transposed(m1.transpose());
    TCRSMatrix<int> m2_transposed(m2.transpose());

    EXPECT_EQ(m1, m1_transposed);
    EXPECT_NE(m2, m2_transposed);

    EXPECT_EQ(m1.getNNZ(), 0);
    EXPECT_EQ(m2.getNNZ(), 0);

    EXPECT_EQ(m1_transposed.getNNZ(), m1.getNNZ());
    EXPECT_EQ(m2_transposed.getNNZ(), m2.getNNZ());

    EXPECT_EQ(m2.getRows(), m2_transposed.getCols());
    EXPECT_EQ(m2.getCols(), m2_transposed.getRows());
}


TEST(TCRSMatrix, double_transpose_returns_original_matrix)
{
    vector<vector<int>> dense = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };

    vector<vector<int>> dense2 = {
    {1, 0, 2, 0},
    {0, 0, 0, 0},
    {0, 0, 3, 0},
    {0, 5, 0, 0},
    };

    TCRSMatrix<int> m1(dense);
    TCRSMatrix<int> m2(dense2);
    
    EXPECT_EQ(m1, m1.transpose().transpose());
    EXPECT_EQ(m2, m2.transpose().transpose());
}

TEST(TCRSMatrix, can_multiply_matrix_by_vector_of_compatible_size)
{
    // прямоугольная матрица
    vector<vector<int>> dense = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };

    // квадратная матрица
    vector<vector<int>> dense2 = {
    {1, 0, 2, 0},
    {0, 0, 0, 0},
    {0, 0, 3, 0},
    {0, 5, 0, 0},
    };

    TCRSMatrix<int> m1(dense);
    TCRSMatrix<int> m2(dense2);

    vector<int> vector_1 = {30, 40, 50, 60, 70, 80, 90};
    vector<int> vector_2 = {10, 20, 30, 40};
    vector<int> vector_3_1 = {0, 0, 0, 0, 0, 0, 0};
    vector<int> vector_3_2 = {0, 0, 0, 0};

    vector<int> res_1 = m1 * vector_1;
    vector<int> res_2 = m2 * vector_2;
    vector<int> res_3_1 = m1 * vector_3_1;
    vector<int> res_3_2 = m2 * vector_3_2;

    vector<int> expected_res_1 = {550, 0, 1280, 1140};
    vector<int> expected_res_2 = {70, 0, 90, 100};
    vector<int> expected_res_3 = {0, 0, 0, 0};

    EXPECT_NE(vector_1, res_1);
    EXPECT_NE(vector_2, res_2);

    EXPECT_EQ(res_1, expected_res_1);
    EXPECT_EQ(res_2, expected_res_2);
    EXPECT_EQ(res_3_1, expected_res_3);
    EXPECT_EQ(res_3_2, expected_res_3);
}

TEST(TCRSMatrix, cant_multiply_matrix_by_vector_of_incompatible_size)
{
    vector<vector<int>> dense = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };

    vector<vector<int>> dense2 = {
    {1, 0, 2, 0},
    {0, 0, 0, 0},
    {0, 0, 3, 0},
    {0, 5, 0, 0},
    };

    TCRSMatrix<int> m1(dense);
    TCRSMatrix<int> m2(dense2);

    vector<int> vector_1_1 = {1, 2, 3, 4, 5, 6, 7, 8};
    vector<int> vector_1_2 = {1, 2, 3, 4, 5, 6};
    vector<int> vector_2_1 = {1, 2, 3};
    vector<int> vector_2_2 = {1, 2, 3, 4, 5};
    vector<int> vector_3 = {};


    ASSERT_ANY_THROW(m1 * vector_1_1);
    ASSERT_ANY_THROW(m1 * vector_1_2);
    ASSERT_ANY_THROW(m2 * vector_2_1);
    ASSERT_ANY_THROW(m2 * vector_2_2);
    ASSERT_ANY_THROW(m1 * vector_3);
    ASSERT_ANY_THROW(m2 * vector_3);
}

TEST(TCRSMatrix, multiply_zero_matrix_by_vector_returns_zero_vector)
{
    TCRSMatrix<int> m1(5);
    TCRSMatrix<int> m2(3, 10);

    vector<int> vector_1 = {3, 4, 5, 6, 7};
    vector<int> vector_2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    vector<int> res_1 = m1 * vector_1;
    vector<int> res_2 = m2 * vector_2;

    vector<int> expected_res_1 = {0, 0, 0, 0 , 0};
    vector<int> expected_res_2 = {0, 0, 0};

    EXPECT_NE(vector_1, res_1);
    EXPECT_NE(vector_2, res_2);

    EXPECT_EQ(res_1, expected_res_1);
    EXPECT_EQ(res_2, expected_res_2);
}

TEST(TCRSMatrix, can_multiply_square_crs_matrices_of_equal_size)
{
    vector<vector<int>> dense1 = {
    {2, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 3, 0, 0, 2, 0, 0},
    {1, 0, 0, 0, 0, 4, 0},
    {0, 0, 5, 0, 0, 0, 1},
    {0, 2, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 3, 2} 
    };

    vector<vector<int>> dense2 = {
    {0, 1, 0, 2, 0, 0, 0},
    {0, 0, 0, 1, 0, 3, 0},
    {0, 2, 0, 0, 0, 0, 4},
    {0, 0, 0, 0, 3, 1, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 2, 0, 0},
    {0, 0, 0, 2, 0, 0, 1} 
    };

    vector<vector<int>> res_dense= {
    {0, 4, 0, 4, 0, 0, 4},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 3, 0, 9, 0},
    {0, 5, 0, 2, 8, 0, 0},
    {0, 10, 0, 2, 0, 0, 21},
    {0, 0, 0, 2, 0, 6, 0},
    {0, 3, 0, 4, 6, 0, 2}
    };

    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);

    TCRSMatrix<int> res = m1 * m2;

    vector<int> expected_values = {4, 4, 4, 3, 9, 5, 2, 8, 10, 2, 21, 2, 6, 3, 4, 6, 2};
    vector<size_t> expected_col_index = {1, 3, 6, 3, 5, 1, 3, 4, 1, 3, 6, 3, 5, 1, 3, 4, 6};
    vector<size_t> expected_row_ptr = {0, 3, 3, 5, 8, 11, 13, 17};


    EXPECT_EQ(res.toDense(), res_dense);

    EXPECT_EQ(res.getNNZ(), 17);
    EXPECT_EQ(res.getCols(), 7);
    EXPECT_EQ(res.getRows(), 7);

    EXPECT_EQ(res.getValues(), expected_values);
    EXPECT_EQ(res.getColIndices(), expected_col_index);
    EXPECT_EQ(res.getRowPtr(), expected_row_ptr);
}

TEST(TCRSMatrix, can_multiply_rectangular_crs_matrices_of_compatible_size)
{
    // прямоугольная матрица
    vector<vector<int>> dense = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };

    vector<vector<int>> dense2 = {
    {0, 5, 0},
    {1, 0, 7},
    {0, 5, 0},
    {0, 0, 7},
    {0, 5, 0},
    {9, 0, 7},
    {0, 10, 0}
    };

    vector<vector<int>> dense3 = {
    {0, 5, 0, 0},
    {1, 0, 7, 0},
    {0, 0, 0, 0},
    {0, 0, 7, 6},
    {0, 5, 0, 9},
    {9, 0, 7, 0},
    {0, 5, 0, 10}
    };

    vector<vector<int>> dense4 = {
    {2, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 3, 0, 0, 2, 0, 0},
    {1, 0, 0, 0, 0, 4, 0},
    {0, 0, 5, 0, 0, 0, 1},
    {0, 2, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 3, 2}
    };

    // прямоугольная * прямоугольная = прямоугольная
    vector<vector<int>> expected_res_1 = {
    {0, 45, 0},
    {0, 0, 0},
    {72, 50, 56},
    {0, 110, 0}
    };

    // прямоугольная * прямоугольная = квадратная
    vector<vector<int>> expected_res_2 = {
    {0, 35, 0, 54},
    {0, 0, 0, 0},
    {72, 35, 56, 63},
    {0, 70, 0, 134}
    };

    // прямоугольная * квадратная = прямоугольная
    vector<vector<int>> expected_res_3 = {
    {2, 6, 31, 0, 4, 0, 6},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 25, 35, 0, 14, 0, 7},
    {0, 0, 30, 0, 0, 24, 22}
    };

    //expected_res_1 в CRS
    vector<int> expected_values_1 = { 45, 72, 50, 56, 110 };
    vector<size_t> expected_col_index_1 = { 1, 0, 1, 2, 1 };
    vector<size_t> expected_row_ptr_1 = { 0, 1, 1, 4, 5 };

    //expected_res_2 в CRS
    vector<int> expected_values_2 = { 35, 54, 72, 35, 56, 63, 70, 134 };
    vector<size_t> expected_col_index_2 = { 1, 3, 0, 1, 2, 3, 1, 3 };
    vector<size_t> expected_row_ptr_2 = { 0, 2, 2, 6, 8 };

    //expected_res_3 в CRS
    vector<int> expected_values_3 = { 2, 6, 31, 4, 6, 25, 35, 14, 7, 30, 24, 22 };
    vector<size_t> expected_col_index_3 = { 0, 1, 2, 4, 6, 1, 2, 4, 6, 2, 5, 6 };
    vector<size_t> expected_row_ptr_3 = { 0, 5, 5, 9, 12 };

    TCRSMatrix<int> m1(dense);
    TCRSMatrix<int> m2(dense2);
    TCRSMatrix<int> m3(dense3);
    TCRSMatrix<int> m4(dense4);

	TCRSMatrix<int> res_1 = m1 * m2;
	TCRSMatrix<int> res_2 = m1 * m3;
	TCRSMatrix<int> res_3 = m1 * m4;

    EXPECT_NE(m1, res_1);
    EXPECT_NE(res_1, m2);
    EXPECT_NE(m1, res_2);
    EXPECT_NE(res_2, m3);
    EXPECT_NE(m1, res_3);
    EXPECT_NE(res_3, m4);

    EXPECT_EQ(res_1.toDense(), expected_res_1);
    EXPECT_EQ(res_1.getNNZ(), 5);
    EXPECT_EQ(res_1.getCols(), 3);
    EXPECT_EQ(res_1.getRows(), 4);
    EXPECT_EQ(res_1.getValues(), expected_values_1);
    EXPECT_EQ(res_1.getColIndices(), expected_col_index_1);
    EXPECT_EQ(res_1.getRowPtr(), expected_row_ptr_1);


    EXPECT_EQ(res_2.toDense(), expected_res_2);
    EXPECT_EQ(res_2.getNNZ(), 8);
    EXPECT_EQ(res_2.getCols(), 4);
    EXPECT_EQ(res_2.getRows(), 4);
    EXPECT_EQ(res_2.getValues(), expected_values_2);
    EXPECT_EQ(res_2.getColIndices(), expected_col_index_2);
    EXPECT_EQ(res_2.getRowPtr(), expected_row_ptr_2);

    EXPECT_EQ(res_3.toDense(), expected_res_3);
    EXPECT_EQ(res_3.getNNZ(), 12);
    EXPECT_EQ(res_3.getCols(), 7);
    EXPECT_EQ(res_3.getRows(), 4);
    EXPECT_EQ(res_3.getValues(), expected_values_3);
    EXPECT_EQ(res_3.getColIndices(), expected_col_index_3);
    EXPECT_EQ(res_3.getRowPtr(), expected_row_ptr_3);
}

TEST(TCRSMatrix, cant_multiply_crs_matrices_with_incompatible_sizes)
{
    vector<vector<int>> dense = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };

    // на одну строку меньше
    vector<vector<int>> dense2 = {
    {0, 5, 0},
    {1, 0, 7},
    {0, 5, 0},
    {0, 0, 7},
    {0, 5, 0},
    {9, 0, 7}
    };

    // на одну строку больше
    vector<vector<int>> dense3 = {
    {0, 5, 0},
    {1, 0, 7},
    {0, 5, 0},
    {0, 0, 7},
    {0, 5, 0},
    {9, 0, 7},
    {0, 35, 0},
    {0, 0, 10}
    };

    TCRSMatrix<int> m1(dense);
    TCRSMatrix<int> m2(dense2);
    TCRSMatrix<int> m3(dense3);

    ASSERT_ANY_THROW(m1 * m2);
    ASSERT_ANY_THROW(m1 * m3);
}

TEST(TCRSMatrix, multiply_by_identity_matrix_returns_original_matrix)
{
    //прямоугольная матрица
    vector<vector<int>> dense = {
    {1, 0, 2, 0, 6, 0, 0},
    {0, 0, 0, 8, 0, 0, 0},
    {0, 0, 3, 0, 7, 8, 0},
    {0, 0, 0, 0, 6, 0, 8}
    };

    //квадратная матрица
    vector<vector<int>> dense2 = {
    {2, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 5, 0, 0, 7},
    {0, 3, 0, 0, 2, 0, 0},
    {1, 0, 0, 0, 0, 4, 0},
    {0, 0, 5, 8, 0, 0, 1},
    {0, 2, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 3, 2}
    };

    //единичная матрица
    vector<vector<int>> identity_matrix = {
    {1, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 1}
    };

    TCRSMatrix<int> m1(dense);
    TCRSMatrix<int> m2(dense2);
    TCRSMatrix<int> id_matrix(identity_matrix);

    TCRSMatrix<int> res_1 = m1 * id_matrix;
    TCRSMatrix<int> res_2 = m2 * id_matrix;

    EXPECT_EQ(m1, res_1);
    EXPECT_EQ(m2, res_2);
    EXPECT_EQ(m1.toDense(), res_1.toDense());
    EXPECT_EQ(m2.toDense(), res_2.toDense());
}

TEST(TCRSMatrix, multiply_matrix_with_empty_rows_works_correctly)
{
    //пустая строка 1 и 3
    vector<vector<int>> dense1 = {
    {0, 0, 0, 0},
    {2, 0, 3, 0},
    {0, 0, 0, 0},
    {0, 4, 0, 5}
    };

    //пустая строка 2 и 3 столбец
    vector<vector<int>> dense2 = {
    {1, 2, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 5},
    {0, 6, 0, 0}
    };

    vector<vector<int>> expected_res_1 = {
    {0, 0, 0, 0},
    {2, 4, 0, 15},
    {0, 0, 0, 0},
    {0, 30, 0, 0}
    };

    vector<vector<int>> expected_res_2 = {
    {4, 0, 6, 0},
    {0, 0, 0, 0},
    {0, 20, 0, 25},
    {12, 0, 18, 0}
    };

    TCRSMatrix<int> m1(dense1);
    TCRSMatrix<int> m2(dense2);

    TCRSMatrix<int> res_1 = m1 * m2;
    TCRSMatrix<int> res_2 = m2 * m1;

    vector<int> expected_values_res_1 = {2, 4, 15, 30};
    vector<size_t> expected_cols_res_1 = {0, 1, 3, 1};
    vector<size_t> expected_row_ptr_res_1 = {0, 0, 3, 3, 4};

    vector<int> expected_values_res_2 = {4, 6, 20, 25, 12, 18};
    vector<size_t> expected_cols_res_2 = {0, 2, 1, 3, 0, 2};
    vector<size_t> expected_row_ptr_res_2 = {0, 2, 2, 4, 6};

    EXPECT_EQ(res_1.toDense(), expected_res_1);
    EXPECT_EQ(res_1.getNNZ(), 4);
    EXPECT_EQ(res_1.getRows(), 4);
    EXPECT_EQ(res_1.getCols(), 4);
    EXPECT_EQ(res_1.getValues(), expected_values_res_1);
    EXPECT_EQ(res_1.getColIndices(), expected_cols_res_1);
    EXPECT_EQ(res_1.getRowPtr(), expected_row_ptr_res_1);

    EXPECT_EQ(res_2.toDense(), expected_res_2);
    EXPECT_EQ(res_2.getNNZ(), 6);
    EXPECT_EQ(res_2.getRows(), 4);
    EXPECT_EQ(res_2.getCols(), 4);
    EXPECT_EQ(res_2.getValues(), expected_values_res_2);
    EXPECT_EQ(res_2.getColIndices(), expected_cols_res_2);
    EXPECT_EQ(res_2.getRowPtr(), expected_row_ptr_res_2);
}

/*
TEST(TDynamicMatrix, cant_subtract_matrixes_with_not_equal_size)
{
  ADD_FAILURE();
}
*/