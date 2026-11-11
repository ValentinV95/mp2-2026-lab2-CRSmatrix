#include "tmatrix.h"

#include <gtest.h>
using namespace std;
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
  vector<vector<int>> dense_empty;
  vector<vector<int>> dense_zero_columns(1);
  vector<vector<int>> dense_inconsistent = {
    {1 , 2},
    {2 , 3, 5},
    {0 , 1}
  };
  ASSERT_ANY_THROW(TCRSMatrix<int> m(dense_empty));
  ASSERT_ANY_THROW(TCRSMatrix<int> m(dense_zero_columns));
  ASSERT_ANY_THROW(TCRSMatrix<int> m(dense_inconsistent));
}

TEST(TCRSMatrix, crs_arrays_are_formed_correctly_from_dense)
{
  vector<vector<int>> dense = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  TCRSMatrix<int> m(dense);

  vector<int> exp_val = {1, 2, 3, 4};
  vector<size_t> exp_col = {0, 2, 0, 1};
  vector<size_t> exp_row = {0, 2, 2, 4};

  EXPECT_EQ(m.getValues(), exp_val);
  EXPECT_EQ(m.getColIndices(), exp_col);
  EXPECT_EQ(m.getRowPtr(), exp_row);

}

TEST(TCRSMatrix, can_convert_crs_matrix_back_to_dense)
{
  vector<vector<int>> dense = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  TCRSMatrix<int> m(dense);
  EXPECT_EQ(m.toDense(), dense);
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
  EXPECT_EQ(m.getNNZ(), m1.getNNZ());
  EXPECT_EQ(m.getValues(), m1.getValues());
}

TEST(TCRSMatrix, copied_matrix_has_its_own_memory)
{
  std::vector<std::vector<int>> dense = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  
  TCRSMatrix<int> m(dense);
  TCRSMatrix<int> m1(m);
  
  ASSERT_FALSE(m.getValues().empty());
  ASSERT_FALSE(m1.getValues().empty());

  EXPECT_NE(&m.getValues()[0], &m1.getValues()[0]);
  EXPECT_NE(&m.getColIndices()[0], &m1.getColIndices()[0]);
  EXPECT_NE(&m.getRowPtr()[0], &m1.getRowPtr()[0]);
}

TEST(TCRSMatrix, can_assign_matrix_to_itself)
{
  std::vector<std::vector<int>> dense = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  
  TCRSMatrix<int> m(dense);
  size_t nnz = m.getNNZ();
  vector<int> val = m.getValues();

  m = m;

  EXPECT_EQ(m.getRows(), 3);
  EXPECT_EQ(m.getCols(), 3);
  EXPECT_EQ(m.getNNZ(), nnz);
  EXPECT_EQ(m.getValues(), val);

  EXPECT_EQ(m.at(0, 0), 1);
}

TEST(TCRSMatrix, can_assign_matrices_of_equal_size)
{
  std::vector<std::vector<int>> dense1 = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  TCRSMatrix<int> m1(dense1);

  std::vector<std::vector<int>> dense2 = {
    {1, 1, 1},
    {1, 1, 1},
    {1, 1, 1}
  };
  TCRSMatrix<int> m2(dense2);

  m2 = m1;

  EXPECT_EQ(m2.getRows(), m1.getRows());
  EXPECT_EQ(m2.getCols(), m1.getCols());
  EXPECT_EQ(m2.getNNZ(), m1.getNNZ());
  EXPECT_EQ(m2.getValues(), m1.getValues());
  EXPECT_EQ(m2.getColIndices(), m1.getColIndices());
  EXPECT_EQ(m2.getRowPtr(), m1.getRowPtr());
}

TEST(TCRSMatrix, assign_operator_change_matrix_size)
{
  std::vector<std::vector<int>> dense1 = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  TCRSMatrix<int> m1(dense1);

  std::vector<std::vector<int>> dense2 = {
    {1, 1, 1, 1},
    {1, 1, 1, 1},
    {1, 1, 1, 1}
  };
  TCRSMatrix<int> m2(dense2);

  ASSERT_NE(m1.getCols(), m2.getCols());

  m2 = m1;

  EXPECT_EQ(m2.getRows(), m1.getRows());
  EXPECT_EQ(m2.getCols(), m1.getCols());
  EXPECT_EQ(m2.getNNZ(), m1.getNNZ());
  EXPECT_EQ(m2.getValues(), m1.getValues());
  EXPECT_EQ(m2.getColIndices(), m1.getColIndices());
  EXPECT_EQ(m2.getRowPtr(), m1.getRowPtr());
}

TEST(TCRSMatrix, can_assign_matrices_of_different_size)
{
  std::vector<std::vector<int>> dense1 = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  TCRSMatrix<int> m1(dense1);

  std::vector<std::vector<int>> dense2 = {
    {1, 1, 1, 1},
    {1, 1, 1, 1},
  };
  TCRSMatrix<int> m2(dense2);

  m2 = m1;

  EXPECT_EQ(m2.getRows(), m1.getRows());
  EXPECT_EQ(m2.getCols(), m1.getCols());
  EXPECT_EQ(m2.getNNZ(), m1.getNNZ());
  EXPECT_EQ(m2.getValues(), m1.getValues());
  EXPECT_EQ(m2.getColIndices(), m1.getColIndices());
  EXPECT_EQ(m2.getRowPtr(), m1.getRowPtr());
}

TEST(TCRSMatrix, can_get_rows_cols_and_nnz)
{
  std::vector<std::vector<int>> dense = {
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
  std::vector<std::vector<int>> dense = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  TCRSMatrix<int> m(dense);

  EXPECT_EQ(m.at(0, 0), 1);
  EXPECT_EQ(m.at(0, 2), 2);
  EXPECT_EQ(m.at(2, 0), 3);
  EXPECT_EQ(m.at(2, 1), 4);

  EXPECT_EQ(m.at(0, 1), 0);
  EXPECT_EQ(m.at(1, 0), 0);
  EXPECT_EQ(m.at(1, 1), 0);
  EXPECT_EQ(m.at(1, 2), 0);
  EXPECT_EQ(m.at(2, 2), 0);
}

TEST(TCRSMatrix, throws_when_get_element_with_negative_or_too_large_index)
{
  TCRSMatrix<int> m(5);

  ASSERT_ANY_THROW(m.at(5, 0));
  ASSERT_ANY_THROW(m.at(0, 5));
  ASSERT_ANY_THROW(m.at(1000, 0));
  ASSERT_ANY_THROW(m.at(0, 1000));
  ASSERT_ANY_THROW(m.at(-1, 0));
  ASSERT_ANY_THROW(m.at(0, -1));
}

TEST(TCRSMatrix, compare_equal_matrices_return_true)
{
  std::vector<std::vector<int>> dense = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  TCRSMatrix<int> m(dense);
  TCRSMatrix<int> m1(dense);

  EXPECT_EQ(m, m1);
}

TEST(TCRSMatrix, compare_matrix_with_itself_return_true)
{
  std::vector<std::vector<int>> dense = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  TCRSMatrix<int> m(dense);

  EXPECT_EQ(m, m);
}

TEST(TCRSMatrix, matrices_with_different_size_are_not_equal)
{
  std::vector<std::vector<int>> dense = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  TCRSMatrix<int> m(dense);

  std::vector<std::vector<int>> dense1 = {
    {1, 0, 2},
    {0, 0, 0},
  };
  TCRSMatrix<int> m1(dense1);

  EXPECT_NE(m, m1);
}

TEST(TCRSMatrix, matrices_with_same_size_but_different_elements_are_not_equal)
{
  std::vector<std::vector<int>> dense1 = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  TCRSMatrix<int> m1(dense1);

  std::vector<std::vector<int>> dense2 = {
    {1, 1, 1},
    {1, 1, 1},
    {1, 1, 1}
  };
  TCRSMatrix<int> m2(dense2);

  EXPECT_NE(m1, m2);
}

TEST(TCRSMatrix, can_transpose_square_matrix)
{
  std::vector<std::vector<int>> dense = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  TCRSMatrix<int> m(dense);

  ASSERT_NO_THROW(m.transpose());

  TCRSMatrix<int> mt = m.transpose();

  EXPECT_EQ(mt.getRows(), 3);
  EXPECT_EQ(mt.getCols(), 3);

  EXPECT_EQ(mt.at(0, 0), 1);
  EXPECT_EQ(mt.at(0, 1), 0);
  EXPECT_EQ(mt.at(0, 2), 3);
  
  EXPECT_EQ(mt.at(1, 0), 0);
  EXPECT_EQ(mt.at(1, 1), 0);
  EXPECT_EQ(mt.at(1, 2), 4);
  
  EXPECT_EQ(mt.at(2, 0), 2);
  EXPECT_EQ(mt.at(2, 1), 0);
  EXPECT_EQ(mt.at(2, 2), 0);
}

TEST(TCRSMatrix, can_transpose_rectangular_matrix)
{
  std::vector<std::vector<int>> dense = {
    {1, 0, 0},
    {1, 1, 0},
  };
  TCRSMatrix<int> m(dense);

  ASSERT_NO_THROW(m.transpose());

  TCRSMatrix<int> mt = m.transpose();

  EXPECT_EQ(mt.getRows(), 3);
  EXPECT_EQ(mt.getCols(), 2);

  EXPECT_EQ(mt.at(0, 0), 1);
  EXPECT_EQ(mt.at(0, 1), 1);
  
  EXPECT_EQ(mt.at(1, 0), 0);
  EXPECT_EQ(mt.at(1, 1), 1);
  
  EXPECT_EQ(mt.at(2, 0), 0);
  EXPECT_EQ(mt.at(2, 1), 0);
}

TEST(TCRSMatrix, double_transpose_returns_original_matrix)
{
  std::vector<std::vector<int>> dense = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  TCRSMatrix<int> m(dense);
  TCRSMatrix<int> mt = m.transpose();
  TCRSMatrix<int> mt2 = mt.transpose();

  EXPECT_EQ(mt2, m);
  EXPECT_EQ(mt2.toDense(), dense);
}

TEST(TCRSMatrix, can_multiply_matrix_by_vector_of_compatible_size)
{
  std::vector<std::vector<int>> dense = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  TCRSMatrix<int> m(dense);
  vector<int> v = {1, 1, 1};

  ASSERT_NO_THROW(m*v);

  vector<int> mv = m*v;

  ASSERT_EQ(mv.size(), 3);

  EXPECT_EQ(mv[0], 3);
  EXPECT_EQ(mv[1], 0);
  EXPECT_EQ(mv[2], 7);
}

TEST(TCRSMatrix, cant_multiply_matrix_by_vector_of_incompatible_size)
{
  std::vector<std::vector<int>> dense = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  TCRSMatrix<int> m(dense);
  vector<int> v = {1, 1, 1, 1};

  ASSERT_ANY_THROW(m*v);
}

TEST(TCRSMatrix, multiply_zero_matrix_by_vector_returns_zero_vector)
{
  TCRSMatrix<int> m(3);
  vector<int> v = {1, 1, 1};
  vector<int> mv = m*v;

  ASSERT_EQ(mv.size(), 3);

  EXPECT_EQ(mv[0], 0);
  EXPECT_EQ(mv[1], 0);
  EXPECT_EQ(mv[2], 0);
}

TEST(TCRSMatrix, can_multiply_square_crs_matrices_of_equal_size)
{
  std::vector<std::vector<int>> dense1 = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  TCRSMatrix<int> m1(dense1);

  std::vector<std::vector<int>> dense2 = {
    {0, 1, 1},
    {1, 0, 1},
    {1, 1, 0}
  };
  TCRSMatrix<int> m2(dense2);

  TCRSMatrix<int> mm = m1 * m2;

  EXPECT_EQ(mm.getRows(), 3);
  EXPECT_EQ(mm.getCols(), 3);

  EXPECT_EQ(mm.at(0, 0), 2);
  EXPECT_EQ(mm.at(0, 1), 3);
  EXPECT_EQ(mm.at(0, 2), 1);

  EXPECT_EQ(mm.at(1, 0), 0);
  EXPECT_EQ(mm.at(1, 1), 0);
  EXPECT_EQ(mm.at(1, 2), 0);

  EXPECT_EQ(mm.at(2, 0), 4);
  EXPECT_EQ(mm.at(2, 1), 3);
  EXPECT_EQ(mm.at(2, 2), 7);

}

TEST(TCRSMatrix, can_multiply_rectangular_crs_matrices_of_compatible_size)
{
  std::vector<std::vector<int>> dense1 = {
    {1, 0, 2, 0},
    {0, 0, 0, 0},
    {3, 4, 0, 1}
  };
  TCRSMatrix<int> m1(dense1);

  std::vector<std::vector<int>> dense2 = {
    {0, 1, 1},
    {1, 0, 1},
    {1, 1, 0},
    {1, 1, 1}
  };
  TCRSMatrix<int> m2(dense2);

  ASSERT_NO_THROW(m1 * m2);

  TCRSMatrix<int> mm = m1 * m2;

  EXPECT_EQ(mm.getRows(), 3);
  EXPECT_EQ(mm.getCols(), 3);

  EXPECT_EQ(mm.at(0, 0), 2);
  EXPECT_EQ(mm.at(0, 1), 3);
  EXPECT_EQ(mm.at(0, 2), 1);

  EXPECT_EQ(mm.at(1, 0), 0);
  EXPECT_EQ(mm.at(1, 1), 0);
  EXPECT_EQ(mm.at(1, 2), 0);

  EXPECT_EQ(mm.at(2, 0), 5);
  EXPECT_EQ(mm.at(2, 1), 4);
  EXPECT_EQ(mm.at(2, 2), 8);
}

TEST(TCRSMatrix, cant_multiply_crs_matrices_with_incompatible_sizes)
{
  std::vector<std::vector<int>> dense1 = {
    {1, 0, 2, 0},
    {0, 0, 0, 0},
    {3, 4, 0, 1}
  };
  TCRSMatrix<int> m1(dense1);

  std::vector<std::vector<int>> dense2 = {
    {0, 1, 1},
    {1, 0, 1}
  };
  TCRSMatrix<int> m2(dense2);

  ASSERT_ANY_THROW(m1 * m2);
}

TEST(TCRSMatrix, multiply_by_identity_matrix_returns_original_matrix)
{
  std::vector<std::vector<int>> dense = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  TCRSMatrix<int> m(dense);

  std::vector<std::vector<int>> e = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
  };
  TCRSMatrix<int> id(e);

  TCRSMatrix<int> me = m * id;

  EXPECT_EQ(me, m);
}

TEST(TCRSMatrix, multiply_matrix_with_empty_rows_works_correctly)
{
  std::vector<std::vector<int>> dense1 = {
    {1, 0, 2},
    {0, 0, 0},
    {3, 4, 0}
  };
  TCRSMatrix<int> m1(dense1);

  std::vector<std::vector<int>> dense2 = {
    {1, 0, 2},
    {0, 4, 0},
    {0, 0, 0}
  };
  TCRSMatrix<int> m2(dense2);

   TCRSMatrix<int> mm = m1 * m2;

  EXPECT_EQ(mm.getRows(), 3);
  EXPECT_EQ(mm.getCols(), 3);

  EXPECT_EQ(mm.at(0, 0), 1);
  EXPECT_EQ(mm.at(0, 1), 0);
  EXPECT_EQ(mm.at(0, 2), 2);

  EXPECT_EQ(mm.at(1, 0), 0);
  EXPECT_EQ(mm.at(1, 1), 0);
  EXPECT_EQ(mm.at(1, 2), 0);

  EXPECT_EQ(mm.at(2, 0), 3);
  EXPECT_EQ(mm.at(2, 1), 16);
  EXPECT_EQ(mm.at(2, 2), 6);
}
TEST(TCRSMatrix, can_multiply_rectangular_matrix_by_vector)
{
  std::vector<std::vector<int>> dense = {
    {1, 2, 0},
    {0, 0, 3}
  };
  TCRSMatrix<int> m(dense);
  std::vector<int> v = {1, 1, 1};
  std::vector<int> mv = m * v;

  ASSERT_EQ(mv.size(), 2);

  EXPECT_EQ(mv[0], 3);
  EXPECT_EQ(mv[1], 3);
}
TEST(TCRSMatrix, works_correctly_with_floating_point_numbers)
{
  std::vector<std::vector<double>> dense = {
    {1.5, 0.0, 1e-15}, 
    {0.0, 2.5, 0.0},
    {3.0, 0.0, 4.5}
  };
  
  TCRSMatrix<double> m(dense);

  EXPECT_EQ(m.getNNZ(), 4);
  
  EXPECT_EQ(m.at(0, 2), 0.0);
  
  EXPECT_DOUBLE_EQ(m.at(0, 0), 1.5);
  EXPECT_DOUBLE_EQ(m.at(1, 1), 2.5);
  EXPECT_DOUBLE_EQ(m.at(2, 0), 3.0);
  EXPECT_DOUBLE_EQ(m.at(2, 2), 4.5);
}
TEST(TCRSMatrix, can_create_matrix_from_raw_crs_arrays)
{
  std::vector<int> vals = {1, 2, 3, 4};
  std::vector<size_t> c_idx = {0, 2, 0, 1};
  std::vector<size_t> r_ptr = {0, 2, 2, 4};
  
  ASSERT_NO_THROW(TCRSMatrix<int> m(3, 3, vals, c_idx, r_ptr));
  
  TCRSMatrix<int> m(3, 3, vals, c_idx, r_ptr);
  
  EXPECT_EQ(m.getRows(), 3);
  EXPECT_EQ(m.getCols(), 3);
  EXPECT_EQ(m.getNNZ(), 4);
  EXPECT_EQ(m.at(0, 0), 1);
  EXPECT_EQ(m.at(0, 2), 2);
  EXPECT_EQ(m.at(2, 0), 3);
  EXPECT_EQ(m.at(2, 1), 4);
}
