#include "tmatrix.h"

#include <gtest.h>

namespace
{
  typedef vector<vector<int>> Dense;
  Dense sampleDense()
  {
    return {
      {1,0,2},
      {0,0,0},
      {3,4,0}
      };
  }
Dense denseMul(const Dense& a, const Dense& b)
{
  Dense res(a.size(), vector<int>(b[0].size(), 0));
  for (size_t i = 0; i < a.size(); i++)
    for (size_t k = 0; k < b.size(); k++)
      for (size_t j = 0; j < b[0].size(); j++)
        res[i][j] += a[i][k] * b[k][j];
  return resl
}
}


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
    {1,0,2},
    {0,0,0},
    {3,4,0}
  };
  ASSERT_NO_THROW(TCRSMatrix<int> m(dense));
}

TEST(TCRSMatrix, throws_when_create_from_empty_or_inconsistent_dense_matrix)
{
  Dense empty;
  Dense empty_rows = { {}, {} };
  Dense inconsistent = { {1,2}, {3} };

  ASSERT_ANY_THROW(TCRSMatrix<int> m(empty));
  ASSERT_ANY_THROW(TCRSMatrix<int> m(empty_rows));
  ASSERT_ANY_THROW(TCRSMatrix<int> m(inconsistent));
}

TEST(TCRSMatrix, crs_arrays_are_formed_correctly_from_dense)
{
  TCRSMatrix<int> m(sampleDense());
  EXPECT_EQ(vector<int>({1,2,3,4}), m.getValues());
  EXPECT_EQ(vector<size_t>({0,2,0, }), m.getColIndices());
  EXPECT_EQ(vector<size_t>({0,2,2,4}), m.getRowPtr());
}

TEST(TCRSMatrix, can_convert_crs_matrix_back_to_dense)
{
  Dense dense = sampleDense();
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
  TCRSMatrix<int> m(sampleDense());
  TCRSMatrix<int> m1(m);

  EXPECT_TRUE(m1 == m);
}

TEST(TCRSMatrix, copied_matrix_has_its_own_memory)
{
  TCRSMatrix<int> m(sampleDense());
  TCRSMatrix<int> m1(m);

  EXPECT_NE(m.getValues().data(), m1.getValues().data());
  EXPECT_NE(m.getColIndices().data(), m1.getColIndices().data());
  EXPECT_NE(m.getRowPtr().data(), m1.getRowPtr().data());
}

TEST(TCRSMatrix, can_assign_matrix_to_itself)
{
  TCRSMatrix<int> m(sampleDense());
  TCRSMatrix<int> expected(m);
  TCRSMatrix<int>& ref = m;

  ASSERT_NO_THROW(m = ref);
  EXPECT_TRUE(m == expected);
}

TEST(TCRSMatrix, can_assign_matrices_of_equal_size)
{
  TCRSMatrix<int> m1(sampleDense());
  TCRSMatrix<int> m2(3);

  m2 = m1;

  EXPECT_TRUE(m2 == m1);
}

TEST(TCRSMatrix, assign_operator_change_matrix_size)
{
  TCRSMatrix<int> m1(Dense({ {1,0,0,2}, {0,3,0,0} }));
  TCRSMatrix<int> m2(5);

  m2 = m1;
  EXPECT_EQ(2u, m2.getRows());
  EXPECT_EQ(4u, m2.getCols());
}

TEST(TCRSMatrix, can_assign_matrices_of_different_size)
{
  TCRSMatrix<int> m1(Dense({ {1,0,0,2}, {0,3,0,0} }));
  TCRSMatrix<int> m2(5);
  
  ASSERT_NO_THROW(m2 = m1);
  EXPECT_TRUE(m2 == m1);
}

TEST(TCRSMatrix, can_get_rows_cols_and_nnz)
{
  TCRSMatrix<int> m(Dense({ {1,0,2,0}, {0,0,0,0}, {3,4,0,0}));
  EXPECT_EQ(3u, m2.getRows());
  EXPECT_EQ(4u, m2.getCols());
  EXPECT_EQ(4u, m2.getNNZ());
}

TEST(TCRSMatrix, can_get_nonzero_and_zero_elements_by_at)
{
  Dense dense = sampleDense();
  TCRSMatrix<int> m(dense);

  for(size_t i = 0; i < 3; i++)
    for(size_t j = 0; j < 3; j++)
      EXPECT_EQ(dense[i][j], m.at(i, j));
}

TEST(TCRSMatrix, throws_when_get_element_with_negative_or_too_large_index)
{
  TCRSMatrix<int> m(sampleDense());
  ASSERT_ANY_THROW(m.at(-1,0));
  ASSERT_ANY_THROW(m.at(0,-1));
  ASSERT_ANY_THROW(m.at(3,0));
  ASSERT_ANY_THROW(m.at(0,3));
}

TEST(TCRSMatrix, compare_equal_matrices_return_true)
{
  TCRSMatrix<int> m1(sampleDense());
  TCRSMatrix<int> m2(sampleDense());

  EXPECT_TRUE(m1 == m2);
  EXPECT_FALSE(m1 != m2);
}

TEST(TCRSMatrix, compare_matrix_with_itself_return_true)
{
  TCRSMatrix<int> m(sampleDense());

  EXPECT_TRUE(m == m);
}

TEST(TCRSMatrix, matrices_with_different_size_are_not_equal)
{
  TCRSMatrix<int> m1(3);
  TCRSMatrix<int> m2(4);
  TCRSMatrix<int> m3(3,4);
  
  EXPECT_TRUE(m1 != m2);
  EXPECT_TRUE(m1 != m3);
}

TEST(TCRSMatrix, matrices_with_same_size_but_different_elements_are_not_equal)
{
  TCRSMatrix<int> m1(Dense({ {1,0}, {0,2} }));
  TCRSMatrix<int> same_pattern(Dense({ {1,0}, {0,5} }));
  TCRSMatrix<int> other_pattern(Dense({ {1,2}, {0,0} }));

  EXPECT_TRUE(m1 != same_pattern);
  EXPECT_TRUE(m1 != other_pattern);
}

TEST(TCRSMatrix, can_transpose_square_matrix)
{
  TCRSMatrix<int> m(sampleDense());
  TCRSMatrix<int> expected(Dense({
    {1,0,3},
    {0,0,4},
    {2,0,0}
  }));

  EXPECT_TRUE(m.transpose() == expected);
}

TEST(TCRSMatrix, can_transpose_rectangular_matrix)
{
  TCRSMatrix<int> m(Dense({ {1,0,2}, {0,3,0} }));
  TCRSMatrix<int> expected(Dense({ {1,0}, {0,3}, {2,0} }));
  TCRSMatrix<int> t = m.transpose();

  EXPECT_EQ(3u, t.getRows());
  EXPECT_EQ(2u, t.getCols());
  EXPECT_TRUE(t == expected);
}

TEST(TCRSMatrix, double_transpose_returns_original_matrix)
{
  TCRSMatrix<int> m(Dense({ {1,0,2,0}, {0,0,0,0}, {3,4,0,5} }));

  EXPECT_TRUE(m.transpose().transpose() == m);
}

TEST(TCRSMatrix, can_multiply_matrix_by_vector_of_compatible_size)
{
  TCRSMatrix<int> m(sampleDense());
  vector<int> v = {1,2,3};

  EXPECT_EQ(vector<int>({7,0,11}), m * v);
}

TEST(TCRSMatrix, cant_multiply_matrix_by_vector_of_incompatible_size)
{
  TCRSMatrix<int> m(sampleDense());

  ASSERT_ANY_THROW(m * vector<int>(2, 1));
  ASSERT_ANY_THROW(m * vector<int>(4, 1));
}

TEST(TCRSMatrix, multiply_zero_matrix_by_vector_returns_zero_vector)
{
  TCRSMatrix<int> z(3, 4);
  vector<int> v = {1, 2, 3, 4};

  EXPECT_EQ(vector<int>(3, 0), z * v);
}

TEST(TCRSMatrix, can_multiply_square_crs_matrices_of_equal_size)
{
  Dense a = { {1,0,2}, {0,3,0}, {4,0,5} };
  Dense b = { {0,1,0}, {2,0,3}, {0,0,4} };

  TCRSMatrix<int> c = TCRSMatrix<int>(a) * TCRSMatrix<int>(b);
  EXPECT_TRUE(c == TCRSMatrix<int>(senseMul(a, b)));
}

TEST(TCRSMatrix, can_multiply_rectangular_crs_matrices_of_compatible_size)
{
  Dense a = { {1,0,2}, {0,3,0} };
  Dense b = { {1,0}, {0,2}, {3,4} }; 
  
  TCRSMatrix<int> c = TCRSMatrix<int>(a) * TCRSMatrix<int>(b);
  
  EXPECT_EQ(2u, c.getRows());
  EXPECT_EQ(2u, c.getCols());
  EXPECT_TRUE(c == TCRSMatrix<int>(denseMul(a, b)));
}

TEST(TCRSMatrix, cant_multiply_crs_matrices_with_incompatible_sizes)
{
  TCRSMatrix<int> a(2,3);
  TCRSMatrix<int> b(2,3);

  ASSERT_ANY_THROW(a * b);
}

TEST(TCRSMatrix, multiply_by_identity_matrix_returns_original_matrix)
{
  TCRSMatrix<int> a(sampleDense());
  TCRSMatrix<int> e(Dense( {1,0,0}, {0,1,0}, {0,0,1}));

  EXPECT_TRUE(a * e == a);
  EXPECT_TRUE(e * a == a);
}

TEST(TCRSMatrix, multiply_matrix_with_empty_rows_works_correctly)
{
  Dense a = { {0,0,0}, {1,0,2}, {0,0,0} };
  Dense b = { {1,2,0}, {0,0,0}, {0,3,4} };

  TCRSMatrix<int> c = TCRSMatrix<int>(a) * TCRSMatrix<int>(b);

  EXPECT_TRUE(c == TCRSMatrix<int>(denseMul(a,b)));
  EXPECT_EQ(vector<size_t>({0,0,3,3}), c.getRowPtr());
}

TEST(TDynamicMatrix, cant_subtract_matrixes_with_not_equal_size)
{
  ADD_FAILURE();
}

TEST(TCRSMatrix, can_create_matrix_from_crs_arrays)
{
  vector<int} vals = {1,2,3,4};
  vector<size_t> cols = {0,2,0,1};
  vector<size_t> ptr = {0,2,2,4};

  ASSERT_NO_THROW(TCRSMatrix<int> m(3,3,vals,cols,ptr));

  TCRSMatrix<int> m(3,3,vals,cols,ptr);
  EXPECT_TRUE( m == TCRSMatrix<int>(sampleDense()));
}

TEST(TCRSMatrix, zeros_in_dense_matrix_are_not_stored)
{
  Dense zero = { {0,0}, {0,0} };
  TCRSMatrix<int> m(zero);
  
  EXPECT_EQ(0u, m.getNNZ());
  EXPECT_EQ(vector<size_t>({0, 0, 0}), m.getRowPtr());
}
