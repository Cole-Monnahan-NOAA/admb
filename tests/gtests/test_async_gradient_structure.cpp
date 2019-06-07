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
};

TEST_F(test_async_gradient_structure, gs)
{
  asyncs(100, []()
  {
    gradient_structure gs;
  });
}
TEST_F(test_async_gradient_structure, empty_gradcalc)
{
  asyncs(100, []()
  {
    gradient_structure gs;
    dvector gradients(1, 2);
    ASSERT_ANY_THROW({
      gradcalc(gradients.size(), gradients);
    });
  });
}
TEST_F(test_async_gradient_structure, withvariables_gradcalc)
{
  asyncs(100, []()
  {
    independent_variables independents(1, 2);
    gradient_structure gs;
    dvar_vector variables(independents);
    dvector gradients(independents.indexmin(), independents.indexmax());
    gradcalc(gradients.size(), gradients);
  });
}
TEST_F(test_async_gradient_structure, simple)
{
  asyncs(100, []()
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

    objective_function_value f;
    f = regression(y, yhat);

    dvector gradients(independents.indexmin(), independents.indexmax());
    gradcalc(gradients.size(), gradients);

    ASSERT_DOUBLE_EQ(independents(1), 0);
    ASSERT_DOUBLE_EQ(independents(2), 0);
    ASSERT_DOUBLE_EQ(gradients(1), -0.7278138528138528);
    ASSERT_DOUBLE_EQ(gradients(2), -3.6126893939393945);
  });
}
