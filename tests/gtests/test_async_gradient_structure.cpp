#include <future>
#include <gtest/gtest.h>
#include <fvar.hpp>
#include <admodel.h>
#include <unistd.h>

extern "C"
{
  void test_ad_exit(const int exit_code);
}

class test_async_gradient_structure: public ::testing::Test
{
protected:
void asyncs(const int size, std::function<void(void)> f)
{
  std::vector<std::future<void>> futures;
  for(int i = 0; i < size; ++i)
  {
    futures.push_back(std::async(f));
  }
  for(auto& element: futures)
  {
    element.get();
  }
}
void asyncs2(const int size, std::function<void(double)> f)
{
  std::vector<std::future<void>> futures;
  for(int i = 0; i < size; ++i)
  {
    futures.push_back(std::async(f, i));
  }
  for(auto& element: futures)
  {
    element.get();
  }
}
void asyncs3(const int size, gradient_structure& gs, std::function<void(void)> f)
{
  std::vector<std::future<void>> futures;
  for(int i = 0; i < size; ++i)
  {
    futures.push_back(std::async([&f, &gs]()
    {
      gradient_structure copy(gs);
      f();
    }));
  }
  for(auto& element: futures)
  {
    element.get();
  }
}
};

TEST_F(test_async_gradient_structure, gs)
{
  ASSERT_EQ(gradient_structure::get_instances(), 0);
  asyncs(100, []()
  {
    gradient_structure gs;
  });
  ASSERT_EQ(gradient_structure::get_instances(), 0);
}
TEST_F(test_async_gradient_structure, empty_gradcalc)
{
  ASSERT_EQ(gradient_structure::get_instances(), 0);
  asyncs(100, []()
  {
    gradient_structure gs;
    dvector gradients(1, 2);
    ASSERT_ANY_THROW({
      gradcalc(gradients.size(), gradients);
    });
  });
  ASSERT_EQ(gradient_structure::get_instances(), 0);
}
TEST_F(test_async_gradient_structure, withvariables_gradcalc)
{
  ASSERT_EQ(gradient_structure::get_instances(), 0);
  asyncs(100, []()
  {
    independent_variables independents(1, 2);
    gradient_structure gs;
    dvar_vector variables(independents);
    dvector gradients(independents.indexmin(), independents.indexmax());
    gradcalc(gradients.size(), gradients);
  });
  ASSERT_EQ(gradient_structure::get_instances(), 0);
}
TEST_F(test_async_gradient_structure, simple)
{
  ASSERT_EQ(gradient_structure::get_instances(), 0);
  asyncs(2, []()
  {
    independent_variables independents(1, 2);

    gradient_structure gs;

    dvar_vector variables(independents);

    dvector x(1, 10);
    x(1) = -1.0;
    x(2) = 0.0;
    x(3) = 1.0;
    x(4) = 2.0;
    x(5) = 3.0;
    x(6) = 4.0;
    x(7) = 5.0;
    x(8) = 6.0;
    x(9) = 7.0;
    x(10) = 8.0;

    dvar_vector yhat(1, 10);
    yhat = variables(1) + variables(2) * x;

    dvector y(1, 10);
    y(1) = 1.4;
    y(2) = 4.7;
    y(3) = 5.1;
    y(4) = 8.3;
    y(5) = 9.0;
    y(6) = 14.5;
    y(7) = 14.0;
    y(8) = 13.4;
    y(9) = 19.2;
    y(10) = 18.0;

    dvariable f;
    f = regression(y, yhat);

    dvector gradients(independents.indexmin(), independents.indexmax());
    gradcalc(gradients.size(), gradients);

    ASSERT_DOUBLE_EQ(independents(1), 0);
    ASSERT_DOUBLE_EQ(independents(2), 0);
    ASSERT_DOUBLE_EQ(gradients(1), -0.7278138528138528);
    ASSERT_DOUBLE_EQ(gradients(2), -3.6126893939393945);
  });
  ASSERT_EQ(gradient_structure::get_instances(), 0);
}
TEST_F(test_async_gradient_structure, simple2)
{
  ASSERT_EQ(gradient_structure::get_instances(), 0);
  asyncs2(100, [](const double input)
  {
    independent_variables independents(1, 2);
    independents(1) = input;
    independents(2) = -input;

    gradient_structure gs;

    dvar_vector variables(independents);

    dvariable f;
    f = input * variables(1) - input * variables(2);

    dvector gradients(independents.indexmin(), independents.indexmax());
    gradcalc(gradients.size(), gradients);

    ASSERT_DOUBLE_EQ(independents(1), input);
    ASSERT_DOUBLE_EQ(independents(2), -input);
    ASSERT_DOUBLE_EQ(gradients(1), input);
    ASSERT_DOUBLE_EQ(gradients(2), -input);
  });
  ASSERT_EQ(gradient_structure::get_instances(), 0);
}
TEST_F(test_async_gradient_structure, simple3)
{
  ASSERT_EQ(gradient_structure::get_instances(), 0);
  asyncs2(100, [](const double input)
  {
    independent_variables independents(1, 2);
    independents(1) = input;
    independents(2) = -input;

    gradient_structure gs;

    dvar_vector variables(independents);

    objective_function_value f;
    f = input * variables(1) - input * variables(2);

    dvector gradients(independents.indexmin(), independents.indexmax());
    gradcalc(gradients.size(), gradients);

    ASSERT_DOUBLE_EQ(independents(1), input);
    ASSERT_DOUBLE_EQ(independents(2), -input);
    ASSERT_DOUBLE_EQ(gradients(1), input);
    ASSERT_DOUBLE_EQ(gradients(2), -input);
  });
  ASSERT_EQ(gradient_structure::get_instances(), 0);
}
/*
TEST_F(test_async_gradient_structure, simple4)
{
  ASSERT_EQ(gradient_structure::get_instances(), 0);
  asyncs2(100, [](const double input)
  {
    independent_variables independents(1, 2);
    independents(1) = input;
    independents(2) = -input;

    gradient_structure gs;

    dvar_vector variables(independents);

    objective_function_value obj;
    obj = input * variables(1) - input * variables(2);

    dvariable f;
    ASSERT_TRUE(objective_function_value::pobjfun == &obj);
    f += *objective_function_value::pobjfun;

    dvector gradients(independents.indexmin(), independents.indexmax());
    gradcalc(gradients.size(), gradients);

    ASSERT_DOUBLE_EQ(independents(1), input);
    ASSERT_DOUBLE_EQ(independents(2), -input);
    ASSERT_DOUBLE_EQ(gradients(1), input);
    ASSERT_DOUBLE_EQ(gradients(2), -input);
  });
  ASSERT_EQ(gradient_structure::get_instances(), 0);
}
*/
TEST_F(test_async_gradient_structure, copyconstructor)
{
  ASSERT_EQ(gradient_structure::get_instances(), 0);
  {
    gradient_structure gs;
    gradient_structure copy(gs);
    ASSERT_EQ(gradient_structure::get_instances(), 2);
  }
  ASSERT_EQ(gradient_structure::get_instances(), 0);
}
TEST_F(test_async_gradient_structure, copyconstructor2)
{
  ASSERT_EQ(gradient_structure::get_instances(), 0);
  asyncs2(100, [](const double input)
  {
    gradient_structure gs;
    ASSERT_TRUE(gradient_structure::get() == &gs);
    gradient_structure copy(gs);
    ASSERT_TRUE(gradient_structure::get() == &copy);
  });
  ASSERT_EQ(gradient_structure::get_instances(), 0);
}
TEST_F(test_async_gradient_structure, copyconstructor_dvariable)
{
  ASSERT_EQ(gradient_structure::get_instances(), 0);
  {
    gradient_structure gs;
    gradient_structure copy(gs);
    dvariable v;
  }
  ASSERT_EQ(gradient_structure::get_instances(), 0);
}
TEST_F(test_async_gradient_structure, copyconstructor_dvar_vector)
{
  ASSERT_EQ(gradient_structure::get_instances(), 0);
  {
    gradient_structure gs;
    gradient_structure copy(gs);
    dvar_vector v(1, 4);
  }
  ASSERT_EQ(gradient_structure::get_instances(), 0);
}
TEST_F(test_async_gradient_structure, dffile)
{
  ASSERT_EQ(gradient_structure::get_instances(), 0);
  {
    gradient_structure gs;
    gradient_structure copy(gs);
    dvar_vector v(1, 4);
    v.initialize();
  }
  ASSERT_EQ(gradient_structure::get_instances(), 0);
}
TEST_F(test_async_gradient_structure, dffile2)
{
  ASSERT_EQ(gradient_structure::get_instances(), 0);
  gradient_structure gs;
  {
    gradient_structure copy(gs);
    dvar_vector v(1, 4);
    v.initialize();
  }
  dvar_vector v(1, 4);
  v.initialize();
  ASSERT_EQ(gradient_structure::get_instances(), 1);
}
TEST_F(test_async_gradient_structure, copyconstructor_gradstack)
{
  ASSERT_EQ(gradient_structure::get_instances(), 0);
  {
    gradient_structure gs;
    gradient_structure copy(gs);
    dvariable v;
    v = 5.0;
  }
  ASSERT_EQ(gradient_structure::get_instances(), 0);
}
TEST_F(test_async_gradient_structure, copyconstructor_gradstack2)
{
  ASSERT_EQ(gradient_structure::get_instances(), 0);
  gradient_structure gs;
  {
    gradient_structure copy(gs);
    dvariable v;
    v = 5.0;
  }
  dvariable v;
  v = 5.0;
  ASSERT_EQ(gradient_structure::get_instances(), 1);
}
TEST_F(test_async_gradient_structure, copy)
{
  ASSERT_EQ(gradient_structure::get_instances(), 0);
  gradient_structure gs;
  ASSERT_EQ(gs.GRAD_LIST.size(), 1750);
  ASSERT_EQ(gs.GRAD_STACK1->total(), 0);
  ASSERT_EQ(gs.GRAD_STACK1->totalempty(), gradient_structure::get_GRADSTACK_BUFFER_SIZE());
  asyncs3(10, gs, []()
  {
    ASSERT_EQ(gradient_structure::get()->GRAD_LIST.size(), 0);
    ASSERT_EQ(gradient_structure::get()->GRAD_STACK1->total(), 0);
    dvariable v;
    ASSERT_EQ(gradient_structure::get()->GRAD_LIST.size(), 1);
#ifdef SAFE_INITIALIZE
    ASSERT_EQ(gradient_structure::get()->GRAD_STACK1->total(), 1);
#else
    ASSERT_EQ(gradient_structure::get()->GRAD_STACK1->total(), 0);
#endif
    v = 5.0;
    ASSERT_EQ(gradient_structure::get()->GRAD_LIST.size(), 1);
#ifdef SAFE_INITIALIZE
    ASSERT_EQ(gradient_structure::get()->GRAD_STACK1->total(), 2);
#else
    ASSERT_EQ(gradient_structure::get()->GRAD_STACK1->total(), 1);
#endif
    ASSERT_EQ(gradient_structure::get()->GRAD_STACK1->total(), 1);
    ASSERT_EQ(gradient_structure::get()->GRAD_STACK1->totalempty(), gradient_structure::get_GRADSTACK_BUFFER_SIZE() - 1);
  });
  ASSERT_EQ(gradient_structure::get_instances(), 1);
  ASSERT_EQ(gs.GRAD_LIST.size(), 1750);
  ASSERT_EQ(gs.GRAD_STACK1->total(), 0);
  ASSERT_EQ(gs.GRAD_STACK1->totalempty(), gradient_structure::get_GRADSTACK_BUFFER_SIZE());
}
