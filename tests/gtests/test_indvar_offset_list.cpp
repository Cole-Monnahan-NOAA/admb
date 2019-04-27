#include <gtest/gtest.h>
#include <fvar.hpp>

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
