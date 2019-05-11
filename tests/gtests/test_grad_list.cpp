#include <gtest/gtest.h>
#include <fvar.hpp>

extern "C"
{
  void test_ad_exit(const int exit_code);
}

class test_grad_list: public ::testing::Test {};

TEST_F(test_grad_list, gradient_structure_instance)
{
  ASSERT_TRUE(gradient_structure::get() == NULL);
  {
    gradient_structure gs;
    ASSERT_TRUE(gradient_structure::get() == &gs);
  }
  ASSERT_TRUE(gradient_structure::get() == NULL);
}
TEST_F(test_grad_list, grad_list)
{
  gradient_structure gs;
  ASSERT_EQ(gs.GRAD_LIST.size(), 1750);
  ASSERT_TRUE(gs.GRAD_LIST.get_unused() == NULL);
  for (int i = 0; i < 1750; ++i)
  {
    gs.GRAD_LIST.get(i)->x = i;
  }
  gs.GRAD_LIST.save_variables();
  gs.GRAD_LIST.initialize();
  for (int i = 0; i < 1750; ++i)
  {
    double_and_int* ptr = gs.GRAD_LIST.get(i);
    ASSERT_EQ(ptr->x, 0);
  }
  gs.GRAD_LIST.restore_variables();
  for (int i = 0; i < 1750; ++i)
  {
    double_and_int* ptr = gs.GRAD_LIST.get(i);
    ASSERT_EQ(ptr->x, i);
  }
  double_and_int* n = gs.GRAD_LIST.create();
  ASSERT_EQ(gs.GRAD_LIST.size(), 1751);
  double_and_int* ret = gs.GRAD_LIST.add_unused(n);
  ASSERT_EQ(gs.GRAD_LIST.size(), 1751);
  ASSERT_TRUE(ret == n);
  ASSERT_EQ(gs.GRAD_LIST.size(), 1751);
  double_and_int* n2 = gs.GRAD_LIST.create();
  double_and_int* ret2 = gs.GRAD_LIST.add_unused(n2);
  ASSERT_TRUE(ret2 == n2);
  ASSERT_EQ(gs.GRAD_LIST.size(), 1751);
  double_and_int* ret3 = gs.GRAD_LIST.get_unused();
  ASSERT_TRUE(ret3 == n2);
  ASSERT_EQ(gs.GRAD_LIST.size(), 1751);
  double_and_int* ret4 = gs.GRAD_LIST.get_unused();
  ASSERT_TRUE(ret4 == NULL);
  ASSERT_EQ(gs.GRAD_LIST.size(), 1751);
}
TEST_F(test_grad_list, recycle)
{
  gradient_structure gs;
  ASSERT_EQ(gs.GRAD_LIST.size(), 1750);

  double_and_int* v = NULL;
  {
    dvariable d;
    v = d.v;
  }
  ASSERT_EQ(gs.GRAD_LIST.size(), 1751);
  ASSERT_TRUE(v == gs.GRAD_LIST.get(1750));

  double_and_int* v2 = NULL;
  {
    dvariable d;
    v2 = d.v;
    ASSERT_TRUE(v == v2);
  }

  ASSERT_EQ(gs.GRAD_LIST.size(), 1751);
  ASSERT_TRUE(v == gs.GRAD_LIST.get(1750));

  double_and_int* ret = gs.GRAD_LIST.get_unused();
  ASSERT_TRUE(v == ret);
}
