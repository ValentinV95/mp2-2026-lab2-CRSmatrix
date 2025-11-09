#include "utmatrix.h"

#include <gtest.h>

TEST(TVector, can_create_vector_with_positive_length) 
{
	ASSERT_NO_THROW(TVector<int> v(5)); 
}

TEST(TVector, cant_create_too_large_vector) 
{
	ASSERT_ANY_THROW(TVector<int> v(MAX_VECTOR_SIZE + 1));
}

TEST(TVector, throws_when_create_vector_with_negative_length) 
{
	ASSERT_ANY_THROW(TVector<int> v(-5));
}

TEST(TVector, throws_when_create_vector_with_negative_startindex)
{
	ASSERT_ANY_THROW(TVector<int> v(5, -2));
}

TEST(TVector, can_create_copied_vector)
{
	TVector<int> v(10);

	ASSERT_NO_THROW(TVector<int> v1(v));
}

TEST(TVector, copied_vector_is_equal_to_source_one)
{
	TVector<int> v(10);
	TVector<int> v1(v);
	ASSERT_TRUE(v == v1);
}

TEST(TVector, copied_vector_has_its_own_memory) 
{
	TVector<int> v(10);
	TVector<int> v1(v);

	EXPECT_NE(&v, &v1);
}

TEST(TVector, can_get_size)
{
	TVector<int> v(4);
	int s = v.GetSize();
	EXPECT_EQ(4, s);
}

TEST(TVector, can_get_start_index)
{
	TVector<int> v(4, 2);

	EXPECT_EQ(2, v.GetStartIndex());
}

TEST(TVector, can_set_and_get_element)
{
	TVector<int> v(4);
	v[0] = 4;

	EXPECT_EQ(4, v[0]);

	int a = v[0];
	EXPECT_EQ(a, 4);
}

TEST(TVector, throws_when_set_element_with_negative_index)
{
	TVector<int> v(4);
	ASSERT_ANY_THROW(v[-1] = 4);
}

TEST(TVector, throws_when_set_element_with_too_large_index)
{
	TVector<int> v(4);
	ASSERT_ANY_THROW(v[100000001] = 4);
}

TEST(TVector, can_assign_vector_to_itself) 
{
	TVector<int> v(5);
	for (int i = 0; i < 5; i++) {
		v[i] = i;
	}
	TVector<int> v1(v);
	//v = v;
	v = v1;
	for (int i = 0; i < 5; i++) {
		EXPECT_EQ(i, v[i]);
	}
}

TEST(TVector, can_assign_vectors_of_equal_size)
{
	TVector<int> v(5);
	TVector<int> v1(5);
	for (int i = 0; i < 5; i++) {
		v[i] = i;
	}
	v1 = v;
	for (int i = 0; i < 5; i++) {
		EXPECT_EQ(i, v1[i]);
	}

}

TEST(TVector, assign_operator_change_vector_size)
{
	TVector<int> v(5);
	TVector<int> v1(7);
	v = v1;
	int l = v.GetSize();
	EXPECT_EQ(l, 7);
	TVector<int> v2(5);
	v1 = v2;
	l = v1.GetSize();
	EXPECT_EQ(5, l);
}

TEST(TVector, can_assign_vectors_of_different_size)
{
	TVector<int> v(5);
	TVector<int> v1(6);
	for (int i = 0; i < 6; i++) {
		v1[i] = i;
	}
	v = v1;
	for (int i = 0; i < 6; i++) {
		EXPECT_EQ(i, v[i]);
	}
	TVector<int> v2(5);
	v1 = v2;
	for (int i = 0; i < 5; i++) {
		EXPECT_EQ(0, v1[i]);
	}
}

TEST(TVector, compare_equal_vectors_return_true)
{
	TVector<int> v(10);
	TVector<int> v1(10);
	ASSERT_TRUE(v == v1);
}

TEST(TVector, compare_vector_with_itself_return_true)
{
	TVector<int> v(10);
	ASSERT_TRUE(v == v);
}

TEST(TVector, vectors_with_different_size_are_not_equal)
{
	TVector<int> v(10);
	TVector<int> v1(9);
	ASSERT_FALSE(v == v1);

}

TEST(TVector, can_add_scalar_to_vector)
{
	TVector<int> v(10);
	v = v + 1;
	int* arr = new int[10];
	for (int i = 0; i < 10; i++) {
		arr[i] = 1;
		EXPECT_EQ(arr[i], v[i]);
	}
}

TEST(TVector, can_subtract_scalar_from_vector)
{
	TVector<int> v(10);
	v = v - 1;
	int* arr = new int[10];
	for (int i = 0; i < 10; i++) {
		arr[i] = -1;
		EXPECT_EQ(arr[i], v[i]);
	}
}

TEST(TVector, can_multiply_scalar_by_vector)
{
	TVector<int> v(10);
	v = v + 1;
	v = v * 3;
	int* arr = new int[10];
	for (int i = 0; i < 10; i++) {
		arr[i] = 3;
		EXPECT_EQ(arr[i], v[i]);
	}
}

TEST(TVector, can_add_vectors_with_equal_size)
{
	TVector<int> v(10);
	TVector<int> v1(10);
	v = v + 1;
	v1 = v1 + 2;
	TVector<int> v2(10);
	v2 = v + v1;
	int arr[10];
	for (int i = 0; i < 10; i++) {
		arr[i] = 3;
	}
	for (int i = 0; i < 10; i++)
	{
		EXPECT_EQ(arr[i], v2[i]);
	}
}

TEST(TVector, cant_add_vectors_with_not_equal_size)
{
	TVector<int> v(10);
	TVector<int> v1(9);
	ASSERT_ANY_THROW(v + v1);
}

TEST(TVector, can_subtract_vectors_with_equal_size)
{
	TVector<int> v(10);
	TVector<int> v1(10);
	v = v + 4;
	v1 = v1 + 2;
	TVector<int> v2(10);
	v2 = v - v1;
	int arr[10];
	for (int i = 0; i < 10; i++) {
		arr[i] = 2;
	}
	for (int i = 0; i < 10; i++)
	{
		EXPECT_EQ(arr[i], v2[i]);
	}
}

TEST(TVector, cant_subtract_vectors_with_not_equal_size)
{
	TVector<int> v(10);
	TVector<int> v1(9);
	ASSERT_ANY_THROW(v - v1);
}

TEST(TVector, can_multiply_vectors_with_equal_size)
{
	TVector<int> v(3);
	TVector<int> v1(3);
	v = v + 4;
	v1 = v1 + 2;

	int a = v * v1;

	EXPECT_EQ(a, 24);

}


TEST(TVector, cant_multiply_vectors_with_not_equal_size)
{
	TVector<int> v(10);
	TVector<int> v1(9);
	ASSERT_ANY_THROW(v * v1);
}