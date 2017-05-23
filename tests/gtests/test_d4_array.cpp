#include <gtest/gtest.h>
#include <cmath>
#include "fvar.hpp"

class test_d4_array: public ::testing::Test {};

TEST_F(test_d4_array, sqrt)
{
  d4_array a(1, 2, 1, 2, 1, 2, 1, 2);

  double d = 2.0;
  for (int i = 1; i <= 2; ++i)
  {
    for (int j = 1; j <= 2; ++j)
    {
      for (int k = 1; k <= 2; ++k)
      {
        for (int l = 1; l <= 2; ++l)
        {
          a(i, j, k, l) = d;
          d += 2.0;
        }
      }
    }
  }

  d4_array ret = sqrt(a);

  d = 2.0;
  for (int i = 1; i <= 2; ++i)
  {
    for (int j = 1; j <= 2; ++j)
    {
      for (int k = 1; k <= 2; ++k)
      {
        for (int l = 1; l <= 2; ++l)
        {
          ASSERT_DOUBLE_EQ(std::sqrt(d), ret(i, j, k, l));
          d += 2.0;
        }
      }
    }
  }
}
TEST_F(test_d4_array, exp)
{
  d4_array a(1, 2, 1, 2, 1, 2, 1, 2);

  double d = 2.0;
  for (int i = 1; i <= 2; ++i)
  {
    for (int j = 1; j <= 2; ++j)
    {
      for (int k = 1; k <= 2; ++k)
      {
        for (int l = 1; l <= 2; ++l)
        {
          a(i, j, k, l) = d;
          d += 2.0;
        }
      }
    }
  }

  d4_array ret = exp(a);

  d = 2.0;
  for (int i = 1; i <= 2; ++i)
  {
    for (int j = 1; j <= 2; ++j)
    {
      for (int k = 1; k <= 2; ++k)
      {
        for (int l = 1; l <= 2; ++l)
        {
          ASSERT_DOUBLE_EQ(std::exp(d), ret(i, j, k, l));
          d += 2.0;
        }
      }
    }
  }
}
TEST_F(test_d4_array, mfexp)
{
  d4_array a(1, 2, 1, 2, 1, 2, 1, 2);

  double d = 2.0;
  for (int i = 1; i <= 2; ++i)
  {
    for (int j = 1; j <= 2; ++j)
    {
      for (int k = 1; k <= 2; ++k)
      {
        for (int l = 1; l <= 2; ++l)
        {
          a(i, j, k, l) = d;
          d += 2.0;
        }
      }
    }
  }

  d4_array ret = mfexp(a);

  d = 2.0;
  for (int i = 1; i <= 2; ++i)
  {
    for (int j = 1; j <= 2; ++j)
    {
      for (int k = 1; k <= 2; ++k)
      {
        for (int l = 1; l <= 2; ++l)
        {
          ASSERT_DOUBLE_EQ(mfexp(d), ret(i, j, k, l));
          d += 2.0;
        }
      }
    }
  }
}
TEST_F(test_d4_array, division_equal_operator)
{
  d4_array a(1, 2, 1, 2, 1, 2, 1, 2);

  double d = 2.0;
  for (int i = 1; i <= 2; ++i)
  {
    for (int j = 1; j <= 2; ++j)
    {
      for (int k = 1; k <= 2; ++k)
      {
        for (int l = 1; l <= 2; ++l)
        {
          a(i, j, k, l) = d;
          d += 2.0;
        }
      }
    }
  }

  a /= 0.5;

  d = 2.0;
  for (int i = 1; i <= 2; ++i)
  {
    for (int j = 1; j <= 2; ++j)
    {
      for (int k = 1; k <= 2; ++k)
      {
        for (int l = 1; l <= 2; ++l)
        {
          ASSERT_DOUBLE_EQ(d / 0.5, a(i, j, k, l));
          d += 2.0;
        }
      }
    }
  }
}
TEST_F(test_d4_array, division_equal_operator2)
{
  d4_array a(1, 2, 1, 2, 1, 2, 1, 2);

  double d = 2.0;
  for (int i = 1; i <= 2; ++i)
  {
    for (int j = 1; j <= 2; ++j)
    {
      for (int k = 1; k <= 2; ++k)
      {
        for (int l = 1; l <= 2; ++l)
        {
          a(i, j, k, l) = d;
          d += 2.0;
        }
      }
    }
  }

  d4_array result = a / 0.5;

  d = 2.0;
  for (int i = 1; i <= 2; ++i)
  {
    for (int j = 1; j <= 2; ++j)
    {
      for (int k = 1; k <= 2; ++k)
      {
        for (int l = 1; l <= 2; ++l)
        {
          ASSERT_DOUBLE_EQ(d / 0.5, result(i, j, k, l));
          d += 2.0;
        }
      }
    }
  }
}
TEST_F(test_d4_array, division_equal_operator3)
{
  d4_array a(1, 2, 1, 2, 1, 2, 1, 2);

  double d = 2.0;
  for (int i = 1; i <= 2; ++i)
  {
    for (int j = 1; j <= 2; ++j)
    {
      for (int k = 1; k <= 2; ++k)
      {
        for (int l = 1; l <= 2; ++l)
        {
          a(i, j, k, l) = d;
          d += 2.0;
        }
      }
    }
  }

  d4_array result = 0.5 / a;

  d = 2.0;
  for (int i = 1; i <= 2; ++i)
  {
    for (int j = 1; j <= 2; ++j)
    {
      for (int k = 1; k <= 2; ++k)
      {
        for (int l = 1; l <= 2; ++l)
        {
          ASSERT_DOUBLE_EQ(0.5 / d , result(i, j, k, l));
          d += 2.0;
        }
      }
    }
  }
}
