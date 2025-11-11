#include "tmatrix.h"
#include <gtest.h>

const size_t rows = 5;
const size_t cols = 10;

TEST(TMatrix, can_create_matrix_with_positive_length)
{
    ASSERT_NO_THROW(TMatrix<int> m(rows, cols));
}

TEST(TMatrix, cant_create_too_large_matrix)
{
    ASSERT_ANY_THROW(TMatrix<int> m(MAX_MATRIX_SIZE + 1, MAX_MATRIX_SIZE + 1));
    ASSERT_ANY_THROW(TMatrix<int> m(rows, MAX_MATRIX_SIZE + 1));
    ASSERT_ANY_THROW(TMatrix<int> m(MAX_MATRIX_SIZE + 1, cols));
}

TEST(TMatrix, throws_when_create_matrix_with_zero_size)
{
    ASSERT_ANY_THROW(TMatrix<int> m(0, 0));
    ASSERT_ANY_THROW(TMatrix<int> m(0, cols));
    ASSERT_ANY_THROW(TMatrix<int> m(rows, 0));
}

TEST(TMatrix, can_create_copied_matrix)
{
    TMatrix<int> m(rows, cols);
    ASSERT_NO_THROW(TMatrix<int> m1(m));
}

TEST(TMatrix, copied_matrix_is_equal_to_source_one)
{
    TMatrix<int> m(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            m[i][j] = i + j + 1;
        }
    }
    TMatrix<int> m1(m);
    EXPECT_EQ(m, m1);
}

TEST(TMatrix, copied_matrix_has_its_own_memory)
{
    TMatrix<int> m(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            m[i][j] = i + j + 1;
        }
    }
    TMatrix<int> m1(m);
    EXPECT_NE(&m[0], &m1[0]);
    m1[1][1] = 10;
    EXPECT_EQ(m[1][1], 3);
    EXPECT_EQ(m1[1][1], 10);
    m[1][1] = 100;
    EXPECT_EQ(m[1][1], 100);
    EXPECT_EQ(m1[1][1], 10);
}

TEST(TMatrix, can_move_assign_matrix)
{
    TMatrix<int> m(rows, cols), m1(rows, cols);
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            m[i][j] = i + j + 1;

    m1 = std::move(m);

    EXPECT_EQ(m1.GetRows(), rows);
    EXPECT_EQ(m1.GetCols(), cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            EXPECT_EQ(m1[i][j], i + j + 1);
        }
    }
    EXPECT_EQ(m.GetRows(), 0);
}

TEST(TMatrix, can_move_construct_matrix)
{
    TMatrix<int> m(rows, cols);
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            m[i][j] = i + j + 1;

    TMatrix<int> m1(std::move(m));

    EXPECT_EQ(m1.GetRows(), rows);
    EXPECT_EQ(m1.GetCols(), cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            EXPECT_EQ(m1[i][j], i + j + 1);
        }
    }
}

TEST(TMatrix, can_get_size)
{
    TMatrix<int> m(rows, cols);
    EXPECT_EQ(rows, m.GetRows());
    EXPECT_EQ(cols, m.GetCols());
}

TEST(TMatrix, can_set_and_get_element)
{
    TMatrix<int> m(rows, cols);
    m[0][0] = 42;
    EXPECT_EQ(42, m[0][0]);
}

TEST(TMatrix, throws_when_set_element_with_negative_index)
{
    TMatrix<int> m(rows, cols);
    ASSERT_ANY_THROW(m.at(-1).at(-1) = 42);
}

TEST(TMatrix, throws_when_set_element_with_too_large_index)
{
    TMatrix<int> m(rows, cols);
    ASSERT_ANY_THROW(m.at(rows).at(cols) = 42);
}

TEST(TMatrix, can_assign_matrix_to_itself)
{
    TMatrix<int> m(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            m[i][j] = i + j + 1;
        }
    }
    m = m;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            EXPECT_EQ(m[i][j], i + j + 1);
        }
    }
}

TEST(TMatrix, can_assign_matrices_of_equal_size)
{
    TMatrix<int> m(rows, cols), m1(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            m[i][j] = i + j + 1;
        }
    }
    m1 = m;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            EXPECT_EQ(m1[i][j], i + j + 1);
        }
    }
}

TEST(TMatrix, assign_operator_change_matrix_size)
{
    TMatrix<int> m(rows, cols), m1(rows - 1, cols - 1);
    m1 = m;
    EXPECT_EQ(m1.GetRows(), m.GetRows());
    EXPECT_EQ(m1.GetCols(), m.GetCols());
}

TEST(TMatrix, can_assign_matrices_of_different_size)
{

    TMatrix<int> m(rows, cols), m1(rows - 1, cols - 1);
    m1 = m;
    EXPECT_EQ(m1.GetRows(), m.GetRows());
    EXPECT_EQ(m1.GetCols(), m.GetCols());
    EXPECT_EQ(m1, m);


    TMatrix<int> m2(rows - 1, cols - 1), m3(rows, cols);
    m3 = m2;
    EXPECT_EQ(m3.GetRows(), m2.GetRows());
    EXPECT_EQ(m3.GetCols(), m2.GetCols());
    EXPECT_EQ(m3, m2);
}

TEST(TMatrix, compare_equal_matrices_return_true)
{
    TMatrix<int> m(rows, cols), m1(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            m[i][j] = i + j + 1;
        }
    }
    m1 = m;
    EXPECT_TRUE(m == m1);
}

TEST(TMatrix, compare_matrix_with_itself_return_true)
{
    TMatrix<int> m(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            m[i][j] = i + j + 1;
        }
    }
    EXPECT_TRUE(m == m);
}

TEST(TMatrix, matrices_with_different_size_are_not_equal)
{
    TMatrix<int> m(rows, cols), m1(rows - 1, cols - 1);
    EXPECT_FALSE(m == m1);
}

TEST(TMatrix, can_add_matrices_with_equal_size)
{
    TMatrix<int> m(rows, cols), m1(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            m[i][j] = i + j + 1;
            m1[i][j] = i + j + 2;
        }
    }
    TMatrix<int> res = m + m1;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            EXPECT_EQ(res[i][j], m[i][j] + m1[i][j]);
        }
    }
}

TEST(TMatrix, cant_add_matrices_with_not_equal_size)
{
    TMatrix<int> m(rows, cols), m1(rows - 1, cols - 1);
    ASSERT_ANY_THROW(m + m1);
}

TEST(TMatrix, can_sub_matrices_with_equal_size)
{
    TMatrix<int> m(rows, cols), m1(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            m[i][j] = i + j + 1;
            m1[i][j] = i + j + 2;
        }
    }
    TMatrix<int> res = m - m1;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            EXPECT_EQ(res[i][j], m[i][j] - m1[i][j]);
        }
    }
}

TEST(TMatrix, cant_sub_matrices_with_not_equal_size)
{
    TMatrix<int> m(rows, cols), m1(rows - 1, cols - 1);
    ASSERT_ANY_THROW(m - m1);
}

TEST(TMatrix, can_mul_matrices_with_compatible_size)
{
    TMatrix<int> m(rows, cols), m1(cols, rows);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            m[i][j] = i + j + 1;
        }
    }
    for (size_t i = 0; i < cols; ++i) {
        for (size_t j = 0; j < rows; ++j) {
            m1[i][j] = i + j + 2;
        }
    }
    TMatrix<int> res = m * m1;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < rows; ++j) {
            int sum = 0;
            for (size_t k = 0; k < cols; ++k) {
                sum += m[i][k] * m1[k][j];
            }
            EXPECT_EQ(res[i][j], sum);
        }
    }
}

TEST(TMatrix, cant_mul_matrices_with_incompatible_size)
{
    TMatrix<int> m(rows, cols), m1(cols + 1, rows + 1);
    ASSERT_ANY_THROW(m * m1);
}

TEST(TMatrix, can_mul_matrix_and_vector_with_compatible_size)
{
    TMatrix<int> m(rows, cols);
    TVector<int> v(cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            m[i][j] = i + j + 1;
        }
    }
    for (size_t k = 0; k < cols; ++k) {
        v[k] = k + 1;
    }
    TVector<int> res = m * v;
    for (size_t i = 0; i < rows; ++i) {
        int sum = 0;
        for (size_t j = 0; j < cols; ++j) {
            sum += m[i][j] * v[j];
        }
        EXPECT_EQ(res[i], sum);
    }
}

TEST(TMatrix, can_mul_matrix_with_scalar)
{
    TMatrix<int> m(rows, cols);
    const int scalar = 42;
    TMatrix<int> res = m * scalar;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            EXPECT_EQ(res[i][j], m[i][j] * scalar);
        }
    }
}

