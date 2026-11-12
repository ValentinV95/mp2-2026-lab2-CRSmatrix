#include "tmatrix.h"

#include <gtest.h>

TEST(TCRSMatrix, can_create_matrix_with_positive_length)
{
    ASSERT_NO_THROW(TCRSMatrix<int> m(5));
    ASSERT_NO_THROW(TCRSMatrix<int> m(1));
    ASSERT_NO_THROW(TCRSMatrix<int> m(1000));
    ASSERT_NO_THROW(TCRSMatrix<int> m(MAX_MATRIX_SIZE));
}

TEST(TCRSMatrix, can_create_rectangular_matrix_with_positive_dimensions)
{
    ASSERT_NO_THROW(TCRSMatrix<int> m(4, 6));
    ASSERT_NO_THROW(TCRSMatrix<int> m(1, 100));
    ASSERT_NO_THROW(TCRSMatrix<int> m(100, 1));
    ASSERT_NO_THROW(TCRSMatrix<double> m(7, 3));
    ASSERT_NO_THROW(TCRSMatrix<float> m(MAX_MATRIX_SIZE, MAX_MATRIX_SIZE));
}

TEST(TCRSMatrix, cant_create_too_large_matrix)
{
    ASSERT_ANY_THROW(TCRSMatrix<int> m(MAX_MATRIX_SIZE + 1));
    ASSERT_ANY_THROW(TCRSMatrix<int> m(MAX_MATRIX_SIZE + 1, 5));
    ASSERT_ANY_THROW(TCRSMatrix<int> m(5, MAX_MATRIX_SIZE + 1));
    ASSERT_ANY_THROW(TCRSMatrix<double> m(MAX_MATRIX_SIZE * 2));
}

TEST(TCRSMatrix, throws_when_create_matrix_with_zero_or_negative_length)
{
    ASSERT_ANY_THROW(TCRSMatrix<int> m(0));
    ASSERT_ANY_THROW(TCRSMatrix<int> m(-5));
    ASSERT_ANY_THROW(TCRSMatrix<int> m(0, 5));
    ASSERT_ANY_THROW(TCRSMatrix<int> m(0, 0));
    ASSERT_ANY_THROW(TCRSMatrix<long> m(-100, -100));
}

TEST(TCRSMatrix, can_create_crs_matrix_from_dense_matrix)
{
    vector<vector<int>> dense1 = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    ASSERT_NO_THROW(TCRSMatrix<int> m(dense1));

    vector<vector<double>> dense2 = {
      {1.5, 0.0, 0.0, 2.5},
      {0.0, 3.7, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0}
    };
    ASSERT_NO_THROW(TCRSMatrix<double> m(dense2));

    vector<vector<int>> dense3 = { {42} };
    ASSERT_NO_THROW(TCRSMatrix<int> m(dense3));

    vector<vector<long long>> dense4 = {
      {1000000, 2000000, 3000000},
      {4000000, 5000000, 6000000}
    };
    ASSERT_NO_THROW(TCRSMatrix<long long> m(dense4));
}

TEST(TCRSMatrix, throws_when_create_from_empty_or_inconsistent_dense_matrix)
{
    vector<vector<int>> empty;
    ASSERT_ANY_THROW(TCRSMatrix<int> m(empty));

    vector<vector<double>> empty_d;
    ASSERT_ANY_THROW(TCRSMatrix<double> m(empty_d));

    vector<vector<int>> inconsistent = {
        {1, 2, 3},
        {4, 5},
        {6, 7, 8}
    };
    ASSERT_ANY_THROW(TCRSMatrix<int> m(inconsistent));

    vector<vector<int>> inconsistent2 = {
        {1},
        {2, 3},
        {4, 5, 6}
    };
    ASSERT_ANY_THROW(TCRSMatrix<int> m(inconsistent2));

    vector<vector<int>> too_big(MAX_MATRIX_SIZE + 1,
        vector<int>(MAX_MATRIX_SIZE + 1, 1));
    ASSERT_ANY_THROW(TCRSMatrix<int> m(too_big));

    vector<vector<int>> too_wide(2, vector<int>(MAX_MATRIX_SIZE + 1, 1));
    ASSERT_ANY_THROW(TCRSMatrix<int> m(too_wide));
}

TEST(TCRSMatrix, crs_arrays_are_formed_correctly_from_dense)
{
    vector<vector<int>> dense = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    TCRSMatrix<int> m(dense);

    EXPECT_EQ(m.getRows(), 3u);
    EXPECT_EQ(m.getCols(), 3u);
    EXPECT_EQ(m.getNNZ(), 4u);
    EXPECT_EQ(m.getValues(), vector<int>({ 1, 2, 3, 4 }));
    EXPECT_EQ(m.getColIndices(), vector<size_t>({ 0, 2, 0, 1 }));
    EXPECT_EQ(m.getRowPtr(), vector<size_t>({ 0, 2, 2, 4 }));

    vector<vector<int>> dense2 = {
      {0, 0, 0, 0},
      {5, 0, 6, 0},
      {0, 0, 0, 0},
      {7, 8, 9, 0}
    };
    TCRSMatrix<int> m2(dense2);

    EXPECT_EQ(m2.getRows(), 4u);
    EXPECT_EQ(m2.getCols(), 4u);
    EXPECT_EQ(m2.getNNZ(), 5u);
    EXPECT_EQ(m2.getValues(), vector<int>({ 5, 6, 7, 8, 9 }));
    EXPECT_EQ(m2.getColIndices(), vector<size_t>({ 0, 2, 0, 1, 2 }));
    EXPECT_EQ(m2.getRowPtr(), vector<size_t>({ 0, 0, 2, 2, 5 }));

    vector<vector<int>> dense3 = {
      {0, 0, 0},
      {0, 0, 0}
    };
    TCRSMatrix<int> m3(dense3);

    EXPECT_EQ(m3.getRows(), 2u);
    EXPECT_EQ(m3.getCols(), 3u);
    EXPECT_EQ(m3.getNNZ(), 0u);
    EXPECT_EQ(m3.getValues(), vector<int>({}));
    EXPECT_EQ(m3.getColIndices(), vector<size_t>({}));
    EXPECT_EQ(m3.getRowPtr(), vector<size_t>({ 0, 0, 0 }));
}

TEST(TCRSMatrix, can_convert_crs_matrix_back_to_dense)
{
    vector<vector<int>> dense1 = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    TCRSMatrix<int> m1(dense1);
    EXPECT_EQ(m1.toDense(), dense1);

    vector<vector<double>> dense2 = {
      {1.5, 0.0, 2.5},
      {0.0, 3.5, 0.0}
    };
    TCRSMatrix<double> m2(dense2);
    EXPECT_EQ(m2.toDense(), dense2);

    vector<vector<int>> dense3 = {
      {10, 20, 30, 40},
      {50, 60, 70, 80}
    };
    TCRSMatrix<int> m3(dense3);
    EXPECT_EQ(m3.toDense(), dense3);
}

TEST(TCRSMatrix, can_create_copied_matrix)
{
    TCRSMatrix<int> m1(5);
    ASSERT_NO_THROW(TCRSMatrix<int> m2(m1));

    vector<vector<int>> dense = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    TCRSMatrix<int> m3(dense);
    ASSERT_NO_THROW(TCRSMatrix<int> m4(m3));

    TCRSMatrix<double> m5(3, 7);
    ASSERT_NO_THROW(TCRSMatrix<double> m6(m5));
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
    EXPECT_EQ(m1.toDense(), dense);

    vector<vector<double>> dense2 = {
      {1.5, 0.0, 2.5, 0.0},
      {0.0, 3.5, 0.0, 4.5}
    };
    TCRSMatrix<double> dm(dense2);
    TCRSMatrix<double> dm1(dm);

    EXPECT_EQ(dm, dm1);
    EXPECT_EQ(dm1.toDense(), dense2);
}

TEST(TCRSMatrix, copied_matrix_has_its_own_memory)
{
    vector<vector<int>> dense = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    TCRSMatrix<int> m(dense);
    TCRSMatrix<int> copy(m);

    vector<vector<int>> new_dense = {
      {9, 9, 9},
      {9, 9, 9},
      {9, 9, 9}
    };
    m = TCRSMatrix<int>(new_dense);

    EXPECT_EQ(copy.toDense(), dense);

    vector<vector<double>> dense2 = {
      {1.1, 0.0},
      {0.0, 2.2}
    };
    TCRSMatrix<double> dm(dense2);
    TCRSMatrix<double> dcopy(dm);

    dm = TCRSMatrix<double>(vector<vector<double>>({ {5.5, 5.5}, {5.5, 5.5} }));

    EXPECT_EQ(dcopy.toDense(), dense2);
}

TEST(TCRSMatrix, can_assign_matrix_to_itself)
{
    vector<vector<int>> dense = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    TCRSMatrix<int> m(dense);
    TCRSMatrix<int>& ref = m;
    m = ref;

    EXPECT_EQ(m.toDense(), dense);

    vector<vector<double>> dense2 = { {1.5, 2.5}, {3.5, 4.5} };
    TCRSMatrix<double> dm(dense2);
    TCRSMatrix<double>& dref = dm;
    dm = dref;

    EXPECT_EQ(dm.toDense(), dense2);
}

TEST(TCRSMatrix, can_assign_matrices_of_equal_size)
{
    vector<vector<int>> d1 = { {1, 0}, {0, 2} };
    vector<vector<int>> d2 = { {3, 4}, {5, 6} };

    TCRSMatrix<int> m1(d1);
    TCRSMatrix<int> m2(d2);

    m1 = m2;

    EXPECT_EQ(m1, m2);
    EXPECT_EQ(m1.toDense(), d2);

    vector<vector<double>> d3 = { {1.1, 0.0, 0.0}, {0.0, 2.2, 0.0}, {0.0, 0.0, 3.3} };
    vector<vector<double>> d4 = { {4.4, 0.0, 0.0}, {0.0, 5.5, 0.0}, {0.0, 0.0, 6.6} };

    TCRSMatrix<double> m3(d3);
    TCRSMatrix<double> m4(d4);

    m3 = m4;

    EXPECT_EQ(m3, m4);
    EXPECT_EQ(m3.toDense(), d4);
}

TEST(TCRSMatrix, assign_operator_change_matrix_size)
{
    vector<vector<int>> d1 = { {1, 0, 2}, {0, 0, 0}, {3, 4, 0} };
    vector<vector<int>> d2 = { {1, 2}, {3, 4} };

    TCRSMatrix<int> m1(d1);
    TCRSMatrix<int> m2(d2);

    m1 = m2;

    EXPECT_EQ(m1.getRows(), 2u);
    EXPECT_EQ(m1.getCols(), 2u);
    EXPECT_EQ(m1.toDense(), d2);

    vector<vector<double>> d3 = { {1.0, 2.0} };
    vector<vector<double>> d4 = { {7.7}, {8.8}, {9.9}, {10.10} };

    TCRSMatrix<double> m3(d3);
    TCRSMatrix<double> m4(d4);

    m3 = m4;

    EXPECT_EQ(m3.getRows(), 4u);
    EXPECT_EQ(m3.getCols(), 1u);
    EXPECT_EQ(m3.toDense(), d4);
}

TEST(TCRSMatrix, can_assign_matrices_of_different_size)
{
    vector<vector<int>> d1 = { {1, 2}, {3, 4} };
    vector<vector<int>> d2 = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    TCRSMatrix<int> m1(d1);
    TCRSMatrix<int> m2(d2);

    m1 = m2;

    EXPECT_EQ(m1, m2);
    EXPECT_EQ(m1.getRows(), 3u);
    EXPECT_EQ(m1.getCols(), 3u);

    vector<vector<long long>> d3 = {
        {100, 200, 300, 400},
        {500, 600, 700, 800}
    };
    vector<vector<long long>> d4 = {
        {1, 2},
        {3, 4},
        {5, 6}
    };

    TCRSMatrix<long long> m3(d3);
    TCRSMatrix<long long> m4(d4);

    m3 = m4;

    EXPECT_EQ(m3, m4);
    EXPECT_EQ(m3.getRows(), 3u);
    EXPECT_EQ(m3.getCols(), 2u);
}

TEST(TCRSMatrix, can_get_rows_cols_and_nnz)
{
    vector<vector<int>> dense = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    TCRSMatrix<int> m(dense);

    EXPECT_EQ(m.getRows(), 3u);
    EXPECT_EQ(m.getCols(), 3u);
    EXPECT_EQ(m.getNNZ(), 4u);

    TCRSMatrix<int> empty(5);
    EXPECT_EQ(empty.getRows(), 5u);
    EXPECT_EQ(empty.getCols(), 5u);
    EXPECT_EQ(empty.getNNZ(), 0u);

    TCRSMatrix<int> rect(2, 7);
    EXPECT_EQ(rect.getRows(), 2u);
    EXPECT_EQ(rect.getCols(), 7u);
    EXPECT_EQ(rect.getNNZ(), 0u);

    vector<vector<double>> dd = {
      {1.1, 0.0, 2.2},
      {0.0, 0.0, 0.0},
      {3.3, 4.4, 0.0},
      {0.0, 0.0, 5.5}
    };
    TCRSMatrix<double> dm(dd);
    EXPECT_EQ(dm.getRows(), 4u);
    EXPECT_EQ(dm.getCols(), 3u);
    EXPECT_EQ(dm.getNNZ(), 5u);
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
    EXPECT_EQ(m.at(0, 2), 2);
    EXPECT_EQ(m.at(2, 0), 3);
    EXPECT_EQ(m.at(2, 1), 4);

    EXPECT_EQ(m.at(0, 1), 0);
    EXPECT_EQ(m.at(1, 0), 0);
    EXPECT_EQ(m.at(1, 1), 0);
    EXPECT_EQ(m.at(1, 2), 0);
    EXPECT_EQ(m.at(2, 2), 0);

    vector<vector<double>> dd = {
      {1.5, 0.0, 0.0, 2.5},
      {0.0, 3.5, 0.0, 0.0}
    };
    TCRSMatrix<double> dm(dd);

    EXPECT_DOUBLE_EQ(dm.at(0, 0), 1.5);
    EXPECT_DOUBLE_EQ(dm.at(0, 3), 2.5);
    EXPECT_DOUBLE_EQ(dm.at(1, 1), 3.5);
    EXPECT_DOUBLE_EQ(dm.at(0, 1), 0.0);
    EXPECT_DOUBLE_EQ(dm.at(1, 3), 0.0);
}

TEST(TCRSMatrix, throws_when_get_element_with_negative_or_too_large_index)
{
    vector<vector<int>> dense = { {1, 2}, {3, 4} };
    TCRSMatrix<int> m(dense);

    ASSERT_ANY_THROW(m.at(2, 0));
    ASSERT_ANY_THROW(m.at(0, 2));
    ASSERT_ANY_THROW(m.at(100, 100));
    ASSERT_ANY_THROW(m.at(static_cast<size_t>(-1), 0));
    ASSERT_ANY_THROW(m.at(0, static_cast<size_t>(-1)));

    vector<vector<double>> dd = {
      {1.0, 2.0, 3.0},
      {4.0, 5.0, 6.0},
      {7.0, 8.0, 9.0}
    };
    TCRSMatrix<double> dm(dd);

    ASSERT_ANY_THROW(dm.at(3, 0));
    ASSERT_ANY_THROW(dm.at(0, 3));
    ASSERT_ANY_THROW(dm.at(static_cast<size_t>(-1), static_cast<size_t>(-1)));
}

TEST(TCRSMatrix, compare_equal_matrices_return_true)
{
    vector<vector<int>> dense = { {1, 0}, {0, 2} };

    TCRSMatrix<int> m1(dense);
    TCRSMatrix<int> m2(dense);

    EXPECT_TRUE(m1 == m2);
    EXPECT_FALSE(m1 != m2);

    vector<vector<double>> dd = { {1.5, 0.0, 2.5}, {0.0, 3.5, 0.0} };
    TCRSMatrix<double> dm1(dd);
    TCRSMatrix<double> dm2(dd);

    EXPECT_TRUE(dm1 == dm2);
    EXPECT_FALSE(dm1 != dm2);

    vector<vector<long long>> dd2 = { {100, 0, 200}, {0, 300, 0} };
    TCRSMatrix<long long> lm1(dd2);
    TCRSMatrix<long long> lm2(dd2);

    EXPECT_TRUE(lm1 == lm2);
    EXPECT_FALSE(lm1 != lm2);
}

TEST(TCRSMatrix, compare_matrix_with_itself_return_true)
{
    vector<vector<int>> dense = { {1, 0}, {0, 2} };
    TCRSMatrix<int> m(dense);

    EXPECT_TRUE(m == m);
    EXPECT_FALSE(m != m);

    vector<vector<float>> df = { {1.5f, 0.0f, 0.0f}, {0.0f, 2.5f, 0.0f}, {0.0f, 0.0f, 3.5f} };
    TCRSMatrix<float> fm(df);

    EXPECT_TRUE(fm == fm);
    EXPECT_FALSE(fm != fm);
}

TEST(TCRSMatrix, matrices_with_different_size_are_not_equal)
{
    TCRSMatrix<int> m1(3);
    TCRSMatrix<int> m2(4);
    EXPECT_FALSE(m1 == m2);
    EXPECT_TRUE(m1 != m2);

    vector<vector<int>> d1 = { {1, 2}, {3, 4} };
    vector<vector<int>> d2 = { {1, 2, 3}, {4, 5, 6} };
    TCRSMatrix<int> m3(d1);
    TCRSMatrix<int> m4(d2);
    EXPECT_FALSE(m3 == m4);
    EXPECT_TRUE(m3 != m4);

    TCRSMatrix<double> m5(2, 5);
    TCRSMatrix<double> m6(5, 2);
    EXPECT_FALSE(m5 == m6);
    EXPECT_TRUE(m5 != m6);

    TCRSMatrix<long long> m7(1);
    TCRSMatrix<long long> m8(1, 2);
    EXPECT_FALSE(m7 == m8);
    EXPECT_TRUE(m7 != m8);
}

TEST(TCRSMatrix, matrices_with_same_size_but_different_elements_are_not_equal)
{
    vector<vector<int>> d1 = { {1, 2}, {3, 4} };
    vector<vector<int>> d2 = { {1, 2}, {3, 5} };

    TCRSMatrix<int> m1(d1);
    TCRSMatrix<int> m2(d2);

    EXPECT_FALSE(m1 == m2);
    EXPECT_TRUE(m1 != m2);

    vector<vector<double>> dd1 = { {1.1, 0.0, 2.2}, {0.0, 3.3, 0.0} };
    vector<vector<double>> dd2 = { {1.1, 0.0, 2.2}, {0.0, 3.4, 0.0} };

    TCRSMatrix<double> dm1(dd1);
    TCRSMatrix<double> dm2(dd2);

    EXPECT_FALSE(dm1 == dm2);
    EXPECT_TRUE(dm1 != dm2);

    vector<vector<int>> de1 = { {0, 0}, {0, 0} };
    vector<vector<int>> de2 = { {0, 1}, {0, 0} };
    TCRSMatrix<int> em1(de1);
    TCRSMatrix<int> em2(de2);

    EXPECT_FALSE(em1 == em2);
    EXPECT_TRUE(em1 != em2);
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
    TCRSMatrix<int> t = m.transpose();

    EXPECT_EQ(t.getRows(), 3u);
    EXPECT_EQ(t.getCols(), 3u);
    EXPECT_EQ(t.toDense(), expected);

    vector<vector<double>> dd = {
      {1.5, 0.0, 2.5, 0.0},
      {0.0, 3.5, 0.0, 0.0},
      {4.5, 0.0, 0.0, 5.5},
      {0.0, 0.0, 6.5, 0.0}
    };
    vector<vector<double>> dexp = {
      {1.5, 0.0, 4.5, 0.0},
      {0.0, 3.5, 0.0, 0.0},
      {2.5, 0.0, 0.0, 6.5},
      {0.0, 0.0, 5.5, 0.0}
    };

    TCRSMatrix<double> dm(dd);
    TCRSMatrix<double> dt = dm.transpose();

    EXPECT_EQ(dt.getRows(), 4u);
    EXPECT_EQ(dt.getCols(), 4u);
    EXPECT_EQ(dt.toDense(), dexp);
}

TEST(TCRSMatrix, can_transpose_rectangular_matrix)
{
    vector<vector<int>> dense = {
      {1, 2, 3},
      {4, 5, 6}
    };
    vector<vector<int>> expected = {
      {1, 4},
      {2, 5},
      {3, 6}
    };

    TCRSMatrix<int> m(dense);
    TCRSMatrix<int> t = m.transpose();

    EXPECT_EQ(t.getRows(), 3u);
    EXPECT_EQ(t.getCols(), 2u);
    EXPECT_EQ(t.toDense(), expected);

    vector<vector<double>> dd = {
      {1.1, 0.0, 0.0, 2.2},
      {0.0, 3.3, 0.0, 0.0},
      {0.0, 0.0, 4.4, 5.5}
    };
    vector<vector<double>> dexp = {
      {1.1, 0.0, 0.0},
      {0.0, 3.3, 0.0},
      {0.0, 0.0, 4.4},
      {2.2, 0.0, 5.5}
    };

    TCRSMatrix<double> dm(dd);
    TCRSMatrix<double> dt = dm.transpose();

    EXPECT_EQ(dt.getRows(), 4u);
    EXPECT_EQ(dt.getCols(), 3u);
    EXPECT_EQ(dt.toDense(), dexp);
}

TEST(TCRSMatrix, double_transpose_returns_original_matrix)
{
    vector<vector<int>> dense = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    TCRSMatrix<int> m(dense);

    EXPECT_EQ(m.transpose().transpose(), m);

    vector<vector<double>> dd = {
      {1.5, 0.0, 2.5, 0.0},
      {0.0, 3.5, 0.0, 0.0}
    };
    TCRSMatrix<double> dm(dd);

    EXPECT_EQ(dm.transpose().transpose(), dm);

    vector<vector<long long>> dd2 = {
      {100, 200, 0},
      {0, 300, 400},
      {0, 0, 500}
    };
    TCRSMatrix<long long> lm(dd2);

    EXPECT_EQ(lm.transpose().transpose(), lm);
}

TEST(TCRSMatrix, can_multiply_matrix_by_vector_of_compatible_size)
{
    vector<vector<int>> dense = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    TCRSMatrix<int> m(dense);

    vector<int> v = { 1, 2, 3 };
    vector<int> expected = { 7, 0, 11 };

    EXPECT_EQ(m * v, expected);

    vector<vector<double>> dd = {
      {1.5, 0.0, 2.5},
      {0.0, 3.5, 0.0},
      {0.0, 0.0, 4.5}
    };
    TCRSMatrix<double> dm(dd);

    vector<double> dv = { 1.0, 2.0, 3.0 };
    vector<double> dexpected = { 9.0, 7.0, 13.5 };
    vector<double> dres = dm * dv;

    ASSERT_EQ(dres.size(), 3u);
    EXPECT_DOUBLE_EQ(dres[0], dexpected[0]);
    EXPECT_DOUBLE_EQ(dres[1], dexpected[1]);
    EXPECT_DOUBLE_EQ(dres[2], dexpected[2]);
}

TEST(TCRSMatrix, cant_multiply_matrix_by_vector_of_incompatible_size)
{
    vector<vector<int>> dense = { {1, 0}, {0, 2} };
    TCRSMatrix<int> m(dense);

    vector<int> v1 = { 1, 2, 3 };
    ASSERT_ANY_THROW(m * v1);

    vector<int> v2 = { 1 };
    ASSERT_ANY_THROW(m * v2);

    vector<int> v3;
    ASSERT_ANY_THROW(m * v3);

    TCRSMatrix<double> dm(3, 4);
    vector<double> dv = { 1.0, 2.0, 3.0 };
    ASSERT_ANY_THROW(dm * dv);
}

TEST(TCRSMatrix, multiply_zero_matrix_by_vector_returns_zero_vector)
{
    TCRSMatrix<int> m(3);
    vector<int> v = { 1, 2, 3 };
    vector<int> expected = { 0, 0, 0 };

    EXPECT_EQ(m * v, expected);

    TCRSMatrix<double> dm(2, 4);
    vector<double> dv = { 1.0, 2.0, 3.0, 4.0 };
    vector<double> dexp = { 0.0, 0.0 };

    EXPECT_EQ(dm * dv, dexp);

    TCRSMatrix<long long> lm(1);
    vector<long long> lv = { 999 };
    vector<long long> lexp = { 0 };

    EXPECT_EQ(lm * lv, lexp);
}

TEST(TCRSMatrix, can_multiply_square_crs_matrices_of_equal_size)
{
    vector<vector<int>> d1 = {
      {1, 0, 2},
      {0, 0, 0},
      {3, 4, 0}
    };
    vector<vector<int>> d2 = {
      {1, 2, 3},
      {4, 5, 6},
      {7, 8, 9}
    };
    vector<vector<int>> expected = {
      {15, 18, 21},
      { 0,  0,  0},
      {19, 26, 33}
    };

    TCRSMatrix<int> m1(d1);
    TCRSMatrix<int> m2(d2);
    TCRSMatrix<int> res = m1 * m2;

    EXPECT_EQ(res.getRows(), 3u);
    EXPECT_EQ(res.getCols(), 3u);
    EXPECT_EQ(res.toDense(), expected);

    vector<vector<double>> dd1 = {
      {1.0, 0.0},
      {0.0, 2.0}
    };
    vector<vector<double>> dd2 = {
      {3.0, 0.0},
      {0.0, 4.0}
    };
    vector<vector<double>> dexp = {
      {3.0, 0.0},
      {0.0, 8.0}
    };

    TCRSMatrix<double> dm1(dd1);
    TCRSMatrix<double> dm2(dd2);
    TCRSMatrix<double> dres = dm1 * dm2;

    EXPECT_EQ(dres.getRows(), 2u);
    EXPECT_EQ(dres.getCols(), 2u);
    EXPECT_EQ(dres.toDense(), dexp);
}

TEST(TCRSMatrix, can_multiply_rectangular_crs_matrices_of_compatible_size)
{
    vector<vector<int>> d1 = {
      {1, 2, 3},
      {4, 5, 6}
    };
    vector<vector<int>> d2 = {
      {1, 2},
      {3, 4},
      {5, 6}
    };
    vector<vector<int>> expected = {
      {22, 28},
      {49, 64}
    };

    TCRSMatrix<int> m1(d1);
    TCRSMatrix<int> m2(d2);
    TCRSMatrix<int> res = m1 * m2;

    EXPECT_EQ(res.getRows(), 2u);
    EXPECT_EQ(res.getCols(), 2u);
    EXPECT_EQ(res.toDense(), expected);

    vector<vector<double>> dd1 = {
      {1.0, 0.0, 2.0, 0.0},
      {0.0, 3.0, 0.0, 0.0}
    };
    vector<vector<double>> dd2 = {
      {1.0, 2.0},
      {0.0, 0.0},
      {3.0, 0.0},
      {0.0, 4.0}
    };
    vector<vector<double>> dexp = {
      {7.0, 2.0},
      {0.0, 0.0}
    };

    TCRSMatrix<double> dm1(dd1);
    TCRSMatrix<double> dm2(dd2);
    TCRSMatrix<double> dres = dm1 * dm2;

    EXPECT_EQ(dres.getRows(), 2u);
    EXPECT_EQ(dres.getCols(), 2u);
    EXPECT_EQ(dres.toDense(), dexp);
}

TEST(TCRSMatrix, cant_multiply_crs_matrices_with_incompatible_sizes)
{
    vector<vector<int>> d1 = {
      {1, 2, 3},
      {4, 5, 6}
    };
    vector<vector<int>> d2 = {
      {1, 2},
      {3, 4}
    };

    TCRSMatrix<int> m1(d1);
    TCRSMatrix<int> m2(d2);

    ASSERT_ANY_THROW(m1 * m2);

    TCRSMatrix<double> dm1(2, 5);
    TCRSMatrix<double> dm2(4, 3);
    ASSERT_ANY_THROW(dm1 * dm2);

    TCRSMatrix<long long> lm1(1, 7);
    TCRSMatrix<long long> lm2(1, 7);
    ASSERT_ANY_THROW(lm1 * lm2);
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
    TCRSMatrix<int> id(identity);

    EXPECT_EQ((m * id).toDense(), dense);
    EXPECT_EQ((id * m).toDense(), dense);

    vector<vector<double>> dd = {
      {1.5, 0.0, 2.5},
      {0.0, 3.5, 0.0}
    };
    vector<vector<double>> didentity = {
      {1.0, 0.0, 0.0},
      {0.0, 1.0, 0.0},
      {0.0, 0.0, 1.0}
    };

    TCRSMatrix<double> dm(dd);
    TCRSMatrix<double> did(didentity);

    EXPECT_EQ((dm * did).toDense(), dd);
}

TEST(TCRSMatrix, multiply_matrix_with_empty_rows_works_correctly)
{
    vector<vector<int>> d1 = {
      {0, 0, 0},
      {1, 2, 3},
      {0, 0, 0}
    };
    vector<vector<int>> d2 = {
      {1, 2, 3},
      {0, 0, 0},
      {4, 5, 6}
    };
    vector<vector<int>> expected = {
      { 0,  0,  0},
      {13, 17, 21},
      { 0,  0,  0}
    };

    TCRSMatrix<int> m1(d1);
    TCRSMatrix<int> m2(d2);

    EXPECT_EQ((m1 * m2).toDense(), expected);

    vector<vector<double>> dd1 = {
      {0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0},
      {1.5, 0.0, 2.5, 0.0}
    };
    vector<vector<double>> dd2 = {
      {1.0, 2.0},
      {3.0, 4.0},
      {5.0, 6.0},
      {7.0, 8.0}
    };
    vector<vector<double>> dexp = {
      {0.0, 0.0},
      {0.0, 0.0},
      {14.0, 18.0}
    };

    TCRSMatrix<double> dm1(dd1);
    TCRSMatrix<double> dm2(dd2);

    EXPECT_EQ((dm1 * dm2).toDense(), dexp);
}