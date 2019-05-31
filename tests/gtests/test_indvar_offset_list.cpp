#include <gtest/gtest.h>
#include <fvar.hpp>

extern "C"
{
  void test_ad_exit(const int exit_code);
}

class test_indvar_offset_list: public ::testing::Test {};

TEST_F(test_indvar_offset_list, gradient_structure_instance)
{
  ASSERT_TRUE(gradient_structure::get() == NULL);
  {
    gradient_structure gs;
    ASSERT_TRUE(gradient_structure::get() == &gs);
  }
  ASSERT_TRUE(gradient_structure::get() == NULL);
}
TEST_F(test_indvar_offset_list, make_indvar_list)
{
  {
    gradient_structure gs;
    ASSERT_TRUE(gradient_structure::get() == &gs);

    independent_variables independents(1, 4);

    dvar_vector dvars(independents);
  }
}
/*
TEST_F(test_indvar_offset_list, constructor)
{
  indvar_offset_list list;

  ASSERT_TRUE(list.get_address(0) == NULL);

  unsigned int index = 0;
  double d0;
  list.put_address(index, &d0);
  ASSERT_TRUE(list.get_address(0) == &d0);

  double d1;
  list.put_address(index, &d1);
  ASSERT_TRUE(list.get_address(0) == &d1);
}
*/
TEST_F(test_indvar_offset_list, option_mno)
{
  ad_exit=&test_ad_exit;

  gradient_structure::set_MAX_NVAR_OFFSET(0);

  ad_comm::argc = 1;
  ad_comm::argv = new char*[1];
  ad_comm::argv[0] = { "-mno" };
  ASSERT_ANY_THROW({
    gradient_structure gs;
  });
  ASSERT_TRUE(gradient_structure::get() == NULL);
}
