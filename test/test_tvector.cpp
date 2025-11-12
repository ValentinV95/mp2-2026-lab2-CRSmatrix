#include "tmatrix.h"

#include <gtest.h>

TEST(TDynamicVector, can_create_vector_with_positive_length)
{
  ASSERT_NO_THROW(TDynamicVector<int> v(5));
}

TEST(TDynamicVector, cant_create_too_large_vector)
{
  ASSERT_ANY_THROW(TDynamicVector<int> v(MAX_VECTOR_SIZE + 1));
}

TEST(TDynamicVector, throws_when_create_vector_with_negative_length)
{
  ASSERT_ANY_THROW(TDynamicVector<int> v(-5));
}

TEST(TDynamicVector, can_create_copied_vector)
{
  TDynamicVector<int> v(10);

  ASSERT_NO_THROW(TDynamicVector<int> v1(v));
}

TEST(TDynamicVector, copied_vector_is_equal_to_source_one)
{
	TDynamicVector<int> m1(4);
	for (int i = 0; i < 4; i++) {
			m1[i] = i ;
		
	}
	TDynamicVector<int> m2(m1);
	EXPECT_EQ(m2[1], m1[1]);
}

TEST(TDynamicVector, copied_vector_has_its_own_memory)
{
	TDynamicVector<int> m1(4);
	TDynamicVector<int> m2(m1);
	for (int i = 0; i < 4; i++) {
			m1[i] = i +1;
		
	}
	EXPECT_NE(m2[0], m1[0]);
}

TEST(TDynamicVector, can_get_size)
{
  TDynamicVector<int> v(4);

  EXPECT_EQ(4, v.size());
}

//TEST(TDynamicVector, can_set_and_get_element)
//{
//  TDynamicVector<int> v(4);
//  v[0] = 4;
//
//  EXPECT_EQ(4, v[0]);
//}

TEST(TDynamicVector, throws_when_set_element_with_negative_index)
{
	TDynamicVector<int> m(5);
	ASSERT_ANY_THROW(m[-1] = 1);
}

TEST(TDynamicVector, throws_when_set_element_with_too_large_index)
{
	TDynamicVector<int> m(MAX_VECTOR_SIZE - 1);
	ASSERT_ANY_THROW(m[MAX_VECTOR_SIZE + 1] = 1);
}

TEST(TDynamicVector, can_assign_vector_to_itself)
{
	TDynamicVector<int> m(MAX_VECTOR_SIZE - 1);
	ASSERT_NO_THROW(m = m);
}

TEST(TDynamicVector, can_assign_vectors_of_equal_size)
{
	TDynamicVector<int> m(MAX_VECTOR_SIZE - 1);
	TDynamicVector<int> m2(MAX_VECTOR_SIZE - 1);
	ASSERT_NO_THROW(m = m2);
}

TEST(TDynamicVector, assign_operator_change_vector_size)
{
	TDynamicVector<int> m(1);
	TDynamicVector<int> m2(MAX_VECTOR_SIZE - 1);
	m2 = m;
	EXPECT_EQ(1, m2.size());
}

TEST(TDynamicVector, can_assign_vectors_of_different_size)
{
	TDynamicVector<int> m(1);
	TDynamicVector<int> m2(MAX_VECTOR_SIZE - 1);
	m2 = m;
	EXPECT_EQ(1, m2.size());
}

TEST(TDynamicVector, compare_equal_vectors_return_true)
{
	TDynamicVector<int> m(1);
	TDynamicVector<int> m2(MAX_VECTOR_SIZE - 1);
	m2 = m;
	EXPECT_EQ(m == m2, 1);
}

TEST(TDynamicVector, compare_vector_with_itself_return_true)
{
	TDynamicVector<int> m(1);
	EXPECT_EQ(m == m, 1);

}

TEST(TDynamicVector, vectors_with_different_size_are_not_equal)
{
	TDynamicVector<int> m(1);
	TDynamicVector<int> m2(MAX_VECTOR_SIZE - 1);
	EXPECT_EQ(m == m2, 0);
}

TEST(TDynamicVector, can_add_scalar_to_vector)
{
	TDynamicVector<int> m(3);
	ASSERT_NO_THROW(m +3 );
}

TEST(TDynamicVector, can_subtract_scalar_from_vector)
{
	TDynamicVector<int> m(3);
	ASSERT_NO_THROW(m - 3);
}

TEST(TDynamicVector, can_multiply_scalar_by_vector)
{
	TDynamicVector<int> m(3);
	ASSERT_NO_THROW(m * 3);
}

TEST(TDynamicVector, can_add_vectors_with_equal_size)
{
	TDynamicVector<int> m(3);
	TDynamicVector<int> m2(3);
	for (size_t i = 0; i < 3; i++)
	{
			m[i] = i + 1;
			m2[i] = i +3;
		
	}

	ASSERT_NO_THROW(m + m2);
}

TEST(TDynamicVector, cant_add_vectors_with_not_equal_size)
{
	TDynamicVector<int> m(3);
	TDynamicVector<int> m2(5);
	for (size_t i = 0; i < 3; i++)
	{
			m[i] = i ;
			m2[i] = i ;
		}
	

	ASSERT_ANY_THROW(m + m2);
}


TEST(TDynamicVector, can_subtract_vectors_with_equal_size)
{
	TDynamicVector<int> m(3);
	TDynamicVector<int> m2(3);
	for (size_t i = 0; i < 3; i++)
	{
			m[i] = i ;
			m2[i] = i ;
		
	}
	ASSERT_NO_THROW(m - m2);

}

TEST(TDynamicVector, cant_subtract_vectors_with_not_equal_size)
{
	TDynamicVector<int> m(3);
	TDynamicVector<int> m2(5);
	for (size_t i = 0; i < 3; i++)
	{
			m[i] = i;
			m2[i]= i ;
		
	}

	ASSERT_ANY_THROW(m - m2);
}

TEST(TDynamicVector, can_multiply_vectors_with_equal_size)
{
	TDynamicVector<int> m(3);
	TDynamicVector<int> m2(3);
	for (size_t i = 0; i < 3; i++)
	{
		m[i] = i;
		m2[i] = i;

	}

	ASSERT_NO_THROW(m * m2);
}

TEST(TDynamicVector, cant_multiply_vectors_with_not_equal_size)
{
	TDynamicVector<int> m(3);
	TDynamicVector<int> m2(5);
	for (size_t i = 0; i < 3; i++)
	{
		m[i] = i;
		m2[i] = i;

	}

	ASSERT_ANY_THROW(m * m2);
}

