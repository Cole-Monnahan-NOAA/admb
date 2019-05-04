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
  ASSERT_EQ(gs.GRAD_LIST.total_addresses(), 1750);
  ASSERT_TRUE(gs.GRAD_LIST.last_remove() == NULL);
  for (int i = 0; i < 1750; ++i)
  {
    double* ptr = gs.GRAD_LIST.get(i);
    *ptr = i;
  }
  gs.GRAD_LIST.save_variables();
  gs.GRAD_LIST.initialize();
  for (int i = 0; i < 1750; ++i)
  {
    double* ptr = gs.GRAD_LIST.get(i);
    ASSERT_EQ(*ptr, 0);
  }
  gs.GRAD_LIST.restore_variables();
  for (int i = 0; i < 1750; ++i)
  {
    double* ptr = gs.GRAD_LIST.get(i);
    ASSERT_EQ(*ptr, i);
  }
  dlink* n = gs.GRAD_LIST.create();
  ASSERT_EQ(gs.GRAD_LIST.total_addresses(), 1751);
  dlink* ret = gs.GRAD_LIST.append(n);
  ASSERT_TRUE(ret == n);
  ASSERT_EQ(gs.GRAD_LIST.total_addresses(), 1751);
  ASSERT_TRUE(ret->prev == NULL);
  dlink* n2 = gs.GRAD_LIST.create();
  dlink* ret2 = gs.GRAD_LIST.append(n2);
  ASSERT_TRUE(ret2 == n2);
  ASSERT_EQ(gs.GRAD_LIST.total_addresses(), 1752);
  ASSERT_TRUE(n2->prev == n);
  ASSERT_TRUE(n->prev == NULL);
  dlink* ret3 = gs.GRAD_LIST.last_remove();
  ASSERT_TRUE(ret3 == n2);
  ASSERT_TRUE(ret3->prev == NULL);
  dlink* ret4 = gs.GRAD_LIST.last_remove();
  ASSERT_TRUE(ret4 == n);
  ASSERT_TRUE(ret4->prev == NULL);
  dlink* ret5 = gs.GRAD_LIST.last_remove();
  ASSERT_TRUE(ret5 == NULL);
}
TEST_F(test_grad_list, recycle)
{
  gradient_structure gs;
  ASSERT_EQ(gs.GRAD_LIST.total_addresses(), 1750);

  double_and_int* v = NULL;
  {
    dvariable d;
    v = d.v;
  }
  ASSERT_EQ(gs.GRAD_LIST.total_addresses(), 1751);
  ASSERT_TRUE(&v->x == gs.GRAD_LIST.get(1750));

  double_and_int* v2 = NULL;
  {
    dvariable d;
    v2 = d.v;
    ASSERT_TRUE(v == v2);
  }

  ASSERT_EQ(gs.GRAD_LIST.total_addresses(), 1751);
  ASSERT_TRUE(&v->x == gs.GRAD_LIST.get(1750));

  dlink* ret = gs.GRAD_LIST.last_remove();
  ASSERT_TRUE(v == &(ret->di));
}
