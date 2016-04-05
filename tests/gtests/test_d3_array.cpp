#include <gtest/gtest.h>
#include "fvar.hpp"

class test_d3_array: public ::testing::Test {};

TEST_F(test_d3_array, pow)
{
  d3_array a(1, 2, 1, 2, 1, 2);
  a(1) = 1;
  a(2) = 2;

  d3_array ret = pow(a, 3);
  ASSERT_EQ(1, ret(1, 1, 1));
  ASSERT_EQ(1, ret(1, 1, 2));
  ASSERT_EQ(1, ret(1, 2, 1));
  ASSERT_EQ(1, ret(1, 2, 2));
  ASSERT_EQ(8, ret(2, 1, 1));
  ASSERT_EQ(8, ret(2, 1, 2));
  ASSERT_EQ(8, ret(2, 2, 1));
  ASSERT_EQ(8, ret(2, 2, 2));
}
TEST_F(test_d3_array, cube)
{
  d3_array a(1, 2, 1, 2, 1, 2);
  a(1) = 1;
  a(2) = 2;

  d3_array ret = cube(a);
  ASSERT_EQ(1, ret(1, 1, 1));
  ASSERT_EQ(1, ret(1, 1, 2));
  ASSERT_EQ(1, ret(1, 2, 1));
  ASSERT_EQ(1, ret(1, 2, 2));
  ASSERT_EQ(8, ret(2, 1, 1));
  ASSERT_EQ(8, ret(2, 1, 2));
  ASSERT_EQ(8, ret(2, 2, 1));
  ASSERT_EQ(8, ret(2, 2, 2));
}
TEST_F(test_d3_array, division)
{
  d3_array a(1, 2, 1, 2, 1, 2);

  double d = 2.0;
  for (int i = 1; i <= 2; ++i)
  {
    for (int j = 1; j <= 2; ++j)
    {
      for (int k = 1; k <= 2; ++k)
      {
        a(i, j, k) = d;
        d += 2.0;
      }
    }
  }

  d3_array ret = a / 2.0;

  d = 2.0;
  for (int i = 1; i <= 2; ++i)
  {
    for (int j = 1; j <= 2; ++j)
    {
      for (int k = 1; k <= 2; ++k)
      {
        ASSERT_DOUBLE_EQ(d / 2.0, ret(i, j, k));
        d += 2.0;
      }
    }
  }
}
TEST_F(test_d3_array, division_member)
{
  d3_array a(1, 2, 1, 2, 1, 2);

  double d = 2.0;
  for (int i = 1; i <= 2; ++i)
  {
    for (int j = 1; j <= 2; ++j)
    {
      for (int k = 1; k <= 2; ++k)
      {
        a(i, j, k) = d;
        d += 2.0;
      }
    }
  }

   a /= 2.0;

  d = 2.0;
  for (int i = 1; i <= 2; ++i)
  {
    for (int j = 1; j <= 2; ++j)
    {
      for (int k = 1; k <= 2; ++k)
      {
        ASSERT_DOUBLE_EQ(d / 2.0, a(i, j, k));
        d += 2.0;
      }
    }
  }
}
