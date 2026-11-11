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
    vector<vector<int>> empty_shveps;
    ASSERT_ANY_THROW(TCRSMatrix<int> m(empty_shveps));
    vector<vector<int>> empty_rows = {{},{}};
    ASSERT_ANY_THROW(TCRSMatrix<int> m(empty_rows));
}

TEST(TCRSMatrix, crs_arrays_are_formed_correctly_from_dense)
{
  vector<vector<int>> dense= {
    { 1, 0, 2 },
    { 0, 0, 0 },
    { 3, 4, 0 }
  };

  TCRSMatrix<int> m(dense);
  EXPECT_EQ(m.getNNZ(), 4);

  vector<int> want_values = { 1,2,3,4 };
  vector<size_t> want_col_ind = { 0,2,0,1 };
  vector<size_t> want_row_ptr = { 0,2,2,4 };

  EXPECT_EQ(m.getValues(), want_values);
  EXPECT_EQ(m.getColIndices(), want_col_ind);
  EXPECT_EQ(m.getRowPtr(), want_row_ptr);
}

TEST(TCRSMatrix, can_convert_crs_matrix_back_to_dense)
{
  vector<vector<int>> dense = {
    { 1, 0, 2 },
    { 0, 0, 0 },
    { 3, 4, 0 }
  };

  TCRSMatrix<int> m(dense);
  vector<vector<int>> convert = m.toDense();

  ASSERT_EQ(convert.size(), dense.size());
  for (size_t i = 0; i < dense.size(); i++) {
      ASSERT_EQ(convert[i].size(), dense[i].size());
  }
  for (size_t i = 0; i < dense.size(); i++) {
      for (size_t j = 0; j < dense[i].size(); j++) {
          EXPECT_EQ(convert[i][j], dense[i][j]);
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
    { 1, 0, 2 },
    { 0, 0, 0 },
    { 3, 4, 0 }
  };

  TCRSMatrix<int> onepac(dense);
  TCRSMatrix<int> twopac(dense);

  EXPECT_TRUE(onepac == twopac);
  EXPECT_FALSE(onepac != twopac);

  EXPECT_EQ(onepac.getRows(), twopac.getRows());
  EXPECT_EQ(onepac.getCols(), twopac.getCols());
  EXPECT_EQ(onepac.getNNZ(), twopac.getNNZ());
}

TEST(TCRSMatrix, copied_matrix_has_its_own_memory)
{
  vector<vector<int>> dense = {
    { 1, 0, 2 },
    { 0, 0, 0 },
    { 3, 4, 0 }
  };

  TCRSMatrix<int> brat1(dense);
  TCRSMatrix<int> brat2(dense);

  ASSERT_GT(brat1.getNNZ(), 0);
  EXPECT_NE(brat1.getValues().data(), brat2.getValues().data());
  EXPECT_NE(brat1.getColIndices().data(), brat2.getColIndices().data());
  EXPECT_NE(brat1.getRowPtr().data(), brat2.getRowPtr().data());
}

TEST(TCRSMatrix, can_assign_matrix_to_itself)
{
  vector<vector<int>> dense = {
    { 1, 0, 2 },
    { 0, 0, 0 },
    { 3, 4, 0 }
  };
  
  TCRSMatrix<int> default(dense);

  ASSERT_NO_THROW(default = default);
  EXPECT_TRUE(default == default);
}

TEST(TCRSMatrix, can_assign_matrices_of_equal_size)
{
  vector<vector<int>> s1 = {
    { 1, 0, 2 },
    { 0, 0, 0 },
    { 3, 4, 0 }
  };
  
  vector<vector<int>> s2 = {
    { 6, 7, 6 },
    { 7, 6, 7 },
    { 6, 7, 6 }
  };

  TCRSMatrix<int> sh1(s1);
  TCRSMatrix<int> sh2(s2);

  sh1 = sh2;

  EXPECT_TRUE(sh1 == sh2);
  EXPECT_EQ(sh1.getRows(), sh2.getRows());
  EXPECT_EQ(sh1.getCols(), sh2.getCols());
  EXPECT_EQ(sh1.getNNZ(), sh2.getNNZ());
}

TEST(TCRSMatrix, assign_operator_change_matrix_size)
{
    TCRSMatrix<int> mat1(3, 4);
    vector<vector<int>> dense = {
        { 6,7 },
        { 6,7 },
        { 6,7 },
        { 6,7 },
        { 6,7 }
    };

    TCRSMatrix<int> mat2(dense);

    mat1 = mat2;

    EXPECT_EQ(mat1.getRows(), 5);
    EXPECT_EQ(mat1.getCols(), 2);
    EXPECT_TRUE(mat1 == mat2);

}

TEST(TCRSMatrix, can_assign_matrices_of_different_size)
{
    vector<vector<int>> mat1 = { { 2,2,8 } };
    vector<vector<int>> mat2 = { { 1,2 },{ 5,6 },{ 6,5 } };
    TCRSMatrix<int> m1(mat1);
    TCRSMatrix<int> m2(mat2);

    m1 = m2;
    
    EXPECT_EQ(m1.getRows(), 3);
    EXPECT_EQ(m2.getCols(), 2);
    EXPECT_TRUE(m1 == m2);
    EXPECT_TRUE(m1.toDense() == m2.toDense());
}

TEST(TCRSMatrix, can_get_rows_cols_and_nnz)
{
  vector<vector<int>> dense = {
    { 1, 0, 2 },
    { 0, 0, 0 },
    { 3, 4, 0 }
  };

  TCRSMatrix<int> mat(dense);

  EXPECT_EQ(mat.getRows(), 3);
  EXPECT_EQ(mat.getCols(), 3);
  EXPECT_EQ(mat.getNNZ(), 4);

  TCRSMatrix<int> emp(4, 2);
  EXPECT_EQ(emp.getRows(), 4);
  EXPECT_EQ(emp.getCols(), 2);
  EXPECT_EQ(emp.getNNZ(), 0);
}

TEST(TCRSMatrix, can_get_nonzero_and_zero_elements_by_at)
{
  vector<vector<int>> dense = {
    { 1, 0, 2 },
    { 0, 0, 0 },
    { 3, 4, 0 }
  };

  TCRSMatrix<int> mat(dense);

  EXPECT_EQ(mat.at(0, 0), 1);
  EXPECT_EQ(mat.at(0, 2), 2);

  EXPECT_EQ(mat.at(0, 1), 0);
  EXPECT_EQ(mat.at(1, 0), 0);
}

TEST(TCRSMatrix, throws_when_get_element_with_negative_or_too_large_index)
{
    vector<vector<int>> dense = {
        { 1, 0, 2 },
        { 0, 0, 0 },
        { 3, 4, 0 }
    };

    TCRSMatrix<int> mat(dense);

    EXPECT_ANY_THROW(mat.at(3, 0));
    EXPECT_ANY_THROW(mat.at(42, 1));

    EXPECT_ANY_THROW(mat.at(0, 3));
    EXPECT_ANY_THROW(mat.at(1, 42));

    EXPECT_ANY_THROW(mat.at(static_cast<size_t>(-1), 0));
    EXPECT_ANY_THROW(mat.at(0, static_cast<size_t>(-1)));
}

TEST(TCRSMatrix, compare_equal_matrices_return_true)
{
    vector<vector<int>> dense = {
          { 1, 0, 2 },
          { 0, 0, 0 },
          { 3, 4, 0 }
    };

    TCRSMatrix<int> mat1(dense);
    TCRSMatrix<int> mat2(dense);
    EXPECT_TRUE(mat1 == mat2);
    EXPECT_FALSE(mat1 != mat2);
}

TEST(TCRSMatrix, compare_matrix_with_itself_return_true)
{
    vector<vector<int>> dense = {
          { 1, 0, 2 },
          { 0, 0, 0 },
          { 3, 4, 0 }
    };

    TCRSMatrix<int> mat(dense);

    EXPECT_TRUE(mat == mat);
    EXPECT_FALSE(mat != mat);
}

TEST(TCRSMatrix, matrices_with_different_size_are_not_equal)
{
    TCRSMatrix<int> mat1(4, 2);
    TCRSMatrix<int> mat2(2, 4);
    TCRSMatrix<int> mat3(2, 2);

    EXPECT_TRUE(mat1 != mat2);
    EXPECT_TRUE(mat1 != mat3);
    EXPECT_TRUE(mat2 != mat3);
    EXPECT_FALSE(mat1 == mat2);
    EXPECT_FALSE(mat1 == mat3);
    EXPECT_FALSE(mat2 == mat3);
}

TEST(TCRSMatrix, matrices_with_same_size_but_different_elements_are_not_equal)
{
  vector<vector<int>> dense1 = {
          { 1, 0, 2 },
          { 0, 0, 0 },
          { 3, 4, 0 }
  };
  vector<vector<int>> dense2 = {
          { 4, 0, 2 },
          { 0, 0, 0 },
          { 6, 7, 0 }
  };

  TCRSMatrix<int> mat1(dense1);
  TCRSMatrix<int> mat2(dense2);
  
  EXPECT_TRUE(mat1 != mat2);
  EXPECT_FALSE(mat1 == mat2);
}

TEST(TCRSMatrix, can_transpose_square_matrix)
{
    vector<vector<int>> dense1 = {
          { 1, 0, 2 },
          { 0, 0, 0 },
          { 3, 4, 0 }
    };
    vector<vector<int>> want = {
          { 1, 0, 3 },
          { 0, 0, 4 },
          { 2, 0, 0 }
    };

    TCRSMatrix<int> mat1(dense1);
    TCRSMatrix<int> tra1 = mat1.transpose();

    EXPECT_EQ(tra1.getRows(), 3);
    EXPECT_EQ(tra1.getCols(), 3);
    EXPECT_EQ(tra1.toDense(), want);
}

TEST(TCRSMatrix, can_transpose_rectangular_matrix)
{
    vector<vector<int>> dense = {
        { 1, 0, 3, 0 },
        { 0, 0, 4, 0 },
        { 2, 0, 0, 6 }
    };

    vector<vector<int>> want = {
        { 1, 0, 2 },
        { 0, 0, 0 },
        { 3, 4, 0 },
        { 0, 0, 6 }
    };
    
    TCRSMatrix<int> mat1(dense);
    TCRSMatrix<int> tra = mat1.transpose();

    EXPECT_EQ(tra.getRows(), 4);
    EXPECT_EQ(tra.getCols(), 3);
    EXPECT_EQ(tra.toDense(), want);
}

TEST(TCRSMatrix, double_transpose_returns_original_matrix)
{
    vector<vector<int>> dense = {
        { 1, 0, 3, 0 },
        { 0, 0, 4, 0 },
        { 2, 0, 0, 6 }
    };

    TCRSMatrix<int> mat(dense);
    TCRSMatrix<int> tratra = mat.transpose().transpose();

    EXPECT_EQ(mat.getRows(), tratra.getRows());
    EXPECT_EQ(mat.getCols(), tratra.getCols());
    EXPECT_EQ(mat.toDense(), tratra.toDense());
}

TEST(TCRSMatrix, can_multiply_matrix_by_vector_of_compatible_size)
{
    vector<vector<int>> dense = {
        { 1, 0, 2 },
        { 0, 0, 0 },
        { 3, 4, 0 }
    };

    TCRSMatrix<int> mat(dense);
    vector<int> vec = { 1,2,3 };
    vector<int> some = mat * vec;

    vector<int> want = { 7,0,11 };
    EXPECT_EQ(some, want);
}

TEST(TCRSMatrix, cant_multiply_matrix_by_vector_of_incompatible_size)
{
    vector<vector<int>> dense = {
        { 1, 0, 2 },
        { 0, 0, 0 },
        { 3, 4, 0 }
    };

    TCRSMatrix<int> mat(dense);

    vector<int> sh = { 1,2 };
    vector<int> lg = { 1,2,3,4 };

    EXPECT_ANY_THROW(mat * sh);
    EXPECT_ANY_THROW(mat * lg);
}

TEST(TCRSMatrix, multiply_zero_matrix_by_vector_returns_zero_vector)
{
    TCRSMatrix<int> empt(3, 4);
    vector<int> vec = { 1,2,3,4 };
    vector<int> some = empt * vec;

    EXPECT_EQ(some.size(), 3);
    EXPECT_EQ(some, (vector<int>{0, 0, 0}));
}

TEST(TCRSMatrix, can_multiply_square_crs_matrices_of_equal_size)
{
    vector<vector<int>> dense1 = {
        { 1, 0, 2 },
        { 0, 0, 0 },
        { 3, 4, 0 }
    };
    vector<vector<int>> dense2 = {
        { 0, 5, 0 },
        { 6, 0, 0 },
        { 0, 0, 7 }
    };

    vector<vector<int>> want = {
        { 0, 5, 14 },
        { 0, 0, 0 },
        { 24, 15, 0 }
    };

    TCRSMatrix<int> mat1(dense1);
    TCRSMatrix<int> mat2(dense2);
    TCRSMatrix<int> want1 = mat1 * mat2;

    EXPECT_EQ(want1.getRows(), 3);  
    EXPECT_EQ(want1.getCols(), 3);
    EXPECT_EQ(want1.toDense(), want);
}

TEST(TCRSMatrix, can_multiply_rectangular_crs_matrices_of_compatible_size)
{
  vector<vector<int>> dense1 = {
     { 1, 2, 3 },
     { 4, 5, 6 }
  };
  vector<vector<int>> dense2 = {
      { 7,8 },
      { 9,10 },
      { 11,12 }
  };

  vector<vector<int>> want = {
      { 58,64 },
      { 139,154 }
  };

  TCRSMatrix<int> mat1(dense1);
  TCRSMatrix<int> mat2(dense2);
  TCRSMatrix<int> mat3 = mat1 * mat2;

  EXPECT_EQ(mat3.getRows(), 2);
  EXPECT_EQ(mat3.getCols(), 2);
  EXPECT_EQ(mat3.toDense(), want);
}

TEST(TCRSMatrix, cant_multiply_crs_matrices_with_incompatible_sizes)
{
    TCRSMatrix<int> mat1(2, 3);
    TCRSMatrix<int> mat2(2, 4);
    
    EXPECT_ANY_THROW(mat1 * mat2);
}

TEST(TCRSMatrix, multiply_by_identity_matrix_returns_original_matrix)
{
    vector<vector<int>> dense = {
        { 1, 0, 2 },
        { 0, 0, 0 },
        { 3, 4, 0 }
    };

    vector<vector<int>> id = {
        { 1, 0, 0 },
        { 0, 1, 0 },
        { 0, 0, 1 }
    };

    TCRSMatrix<int>mat(dense);
    TCRSMatrix<int>ide(id);
    TCRSMatrix<int> want = mat * ide;

    EXPECT_EQ(want.getRows(), mat.getRows());
    EXPECT_EQ(want.getCols(), mat.getCols());
    EXPECT_EQ(want.toDense(), mat.toDense());
}

TEST(TCRSMatrix, multiply_matrix_with_empty_rows_works_correctly)
{
    vector<vector<int>> dense = {
        { 1, 0, 2 },
        { 0, 0, 0 },
        { 3, 4, 0 }
    };
    vector<vector<int>> dense1 = {
        { 1, 0, 0 },
        { 0, 1, 0 },
        { 0, 0, 1 }
    };

    TCRSMatrix<int>mat1(dense);
    TCRSMatrix<int>mat2(dense1);
    TCRSMatrix<int>res = mat1 * mat2;

    vector<vector<int>> densres = res.toDense();
    EXPECT_EQ(densres, mat1.toDense());
    EXPECT_EQ(res.getRowPtr()[2] - res.getRowPtr()[1], 0);
}

TEST(TDynamicMatrix, cant_subtract_matrixes_with_not_equal_size)
{
  ADD_FAILURE();
}

