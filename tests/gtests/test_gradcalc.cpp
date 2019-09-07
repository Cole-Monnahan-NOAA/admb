#include <gtest/gtest.h>
#include <fvar.hpp>
#include <admodel.h>

class test_gradcalc: public ::testing::Test {};

TEST_F(test_gradcalc, issue50)
{
  independent_variables independents(1, 3);
  independents(1) = 60;
  independents(2) = 61;
  independents(3) = 100;

  gradient_structure gs;
  dvar_vector values(independents);
  dvar_vector results = mfexp(values);
  dvector fd1 = value(results);
  fd1 /= std::exp(60.0);
  double dx = 1.0e-7;
  dvar_vector values2(1, 3);
  values2(1) = 60 + dx;
  values2(2) = 61 + dx;
  values2(3) = 100 + dx;
  dvector fd2 = value(mfexp(values2));
  fd2 /= std::exp(60.0);

  objective_function_value total;
  total = sum(results);
  double f = value(total);

  dvector gradients(1, 3);
  gradcalc(3, gradients);

  cout << independents << endl;

  gradients /= std::exp(60);

  ASSERT_DOUBLE_EQ(gradients(1), std::pow(1.0 + independents(1) - 60.0, -2));
  ASSERT_DOUBLE_EQ(gradients(2), std::pow(1.0 + independents(2) - 60.0, -2));
  ASSERT_DOUBLE_EQ(gradients(3), std::pow(1.0 + independents(3) - 60.0, -2));

  dvector fd(1, 3);
  fd(1) = (fd2(1) - fd1(1)) / dx;
  fd(2) = (fd2(2) - fd1(2)) / dx;
  fd(3) = (fd2(3) - fd1(3)) / dx;

  ASSERT_NEAR(fd(1), gradients(1), 0.000001);
  ASSERT_NEAR(fd(2), gradients(2), 0.000001);
  ASSERT_NEAR(fd(3), gradients(3), 0.000001);
}

#include <functional>
#include <utility>

std::pair<double,dvector> gradcalc(const independent_variables& independents, std::function<dvariable(dvar_vector)> f)
{
  gradient_structure gs;

  objective_function_value objective;
  objective = f(independents);

  double v = value(objective);

  dvector gradients(1, 3);
  gradcalc(3, gradients);

  return std::make_pair(v, gradients);
}

TEST_F(test_gradcalc, independent_variables_to_dvar_vector)
{
  independent_variables independents(1, 3);
  independents(1) = 60;
  independents(2) = 61;
  independents(3) = 100;

  std::pair<double,dvector> results = gradcalc(independents, [](dvar_vector values)
  {
    dvariable total;
    dvar_vector results = mfexp(values);
    total = sum(results);
    return total;
  });

  cout << results.first << ' ' << results.second << endl;
}

std::pair<double,dvector> gradcalc(const std::vector<double>& independent_values, std::function<dvariable(dvar_vector)> f)
{
  size_t size = independent_values.size();
  independent_variables independents(1, size);
  for (int i = 1; i <= size; ++i)
  {
    independents(i) = independent_values[i - 1];
  }
  return gradcalc(independents, f);
}

TEST_F(test_gradcalc, independent_values_to_dvar_vector)
{
  std::pair<double,dvector> results = gradcalc({60, 61, 100}, [](dvar_vector values)
  {
    dvariable total;
    dvar_vector results = mfexp(values);
    total = sum(results);
    return total;
  });

  cout << results.first << ' ' << results.second << endl;
}

template <class... Args>
std::vector<double> make_vector(double value, Args&&...args);
template <class... Args>
std::vector<double> make_vector(dvector& values, Args&&...args);
template <class... Args>
std::vector<double> make_vector(std::vector<double>& values, Args&&...args);

std::vector<double> make_vector()
{
  std::vector<double> lst;
  return std::move(lst);
}
template <class... Args>
std::vector<double> make_vector(std::vector<double>& values, Args&&...args)
{
  std::vector<double> lst = make_vector(args...);
  for (double value: values)
  {
    lst.push_back(value);
  }
  return std::move(lst);
}
template <class... Args>
std::vector<double> make_vector(dvector& values, Args&&...args)
{
  std::vector<double> lst = make_vector(args...);
  for (int i = values.indexmin(); i <= values.indexmax(); ++i)
  {
    lst.push_back(values(i));
  }
  return std::move(lst);
}
template <class... Args>
std::vector<double> make_vector(double value, Args&&...args)
{
  std::vector<double> lst = make_vector(args...);
  lst.push_back(value);
  return std::move(lst);
}
template <class Fn, class... Args>
std::pair<double,dvector> compute_gradients(Fn&& fn, Args&&... args)
{
  /*
  std::vector<double> lst = make_vector(args...);

  int index = lst.size();
  independent_variables independents(1, index);
  for (double value: lst)
  {
    independents(index--) = value;
  }
  */

  gradient_structure gs;
  //dvar_vector values(independents);

  objective_function_value fn_result;
  fn_result = fn(std::forward<Args>(args)...);

  double value_fn_result = value(fn_result);

  dvector gradients(1, 3);
  gradcalc(3, gradients);

  return std::make_pair(value_fn_result, gradients);
}
/*
void make_indvar_list(const dvar_vector& t)
{
  gradient_structure* gs = gradient_structure::get();
  if (gs)
  {
    gs->INDVAR_LIST.set_addresses(t);
  }
}
void indvar_offset_list::set_addresses(const dvar_vector& t)
{
  int min = t.indexmin();
  int max = t.indexmax();
  address.clear();
  for (int i = min; i <= max; ++i)
  {
    address.push_back(&(t.va[i].x));
  }
}
*/
TEST_F(test_gradcalc, compute_gradients_template)
{
  auto fn = [](dvar_vector values, double d, std::vector<double> stdvector)
  {
    dvariable total;
    dvar_vector results = mfexp(values);
    total = sum(results);
    return total;
  };

  dvector dv(1, 3);
  dv(1) = 60;
  dv(2) = 61;
  dv(3) = 100;
  double d = 61;
  std::vector<double> stdvector{100};
  std::pair<double,dvector> results = compute_gradients(fn, independent_variables(dv), d, stdvector);

  cout << results.first << ' ' << results.second << endl;
}
TEST_F(test_gradcalc, inline_compute_gradients_template)
{
  dvector dv(1, 3);
  dv(1) = 60;
  dv(2) = 61;
  dv(3) = 100;
  double d = 61;
  std::vector<double> stdvector{100};
  std::pair<double,dvector> results = compute_gradients(
    [](dvar_vector values, double d, std::vector<double> stdvector)
    {
      dvariable total;
      dvar_vector results = mfexp(values);
      total = sum(results);
      return total;
    }, independent_variables(dv), d, stdvector);

  cout << results.first << ' ' << results.second << endl;
}
template <class Fn, class... Args>
std::pair<double,dvector> compute_gradients2(Fn&& fn, Args&&... args)
{
  gradient_structure gs;

  dvariable fn_result;
  fn_result = fn(std::forward<Args>(args)...);

  double value_fn_result = value(fn_result);

  size_t nvar = gs.INDVAR_LIST.get_nvar();
  dvector gradients(1, nvar);
  gradcalc(nvar, gradients);

  return std::make_pair(value_fn_result, gradients);
}
TEST_F(test_gradcalc, inline_compute_gradients_template_double)
{
  dvector dv(1, 2);
  dv(1) = 60;
  dv(2) = 61;
  double d = 100;
  std::vector<double> stdvector{100};
  std::pair<double,dvector> results = compute_gradients2(
    [](dvar_vector values, dvariable d, std::vector<double> stdvector)
    {
      dvariable total;
      dvar_vector results = mfexp(values);
      total = sum(results) + mfexp(d);
      return total;
    }, dv, independent_variables(d), stdvector);

  cout << results.first << ' ' << results.second << endl;
}
#include <future>
TEST_F(test_gradcalc, future)
{
  double x = 5;
  double y = 3;

  std::future<double> results_first_term(std::async([](double x, double y)
    {
      return x / y;
    }, x, y));
  std::future<double> results_second_term(std::async([](double x, double y)
    {
      return x * y;
    }, x, y));

  ASSERT_DOUBLE_EQ(results_first_term.get(), 5.0 / 3.0);
  ASSERT_DOUBLE_EQ(results_second_term.get(), 5.0 * 3.0);
}
double mydiv(const double x, const double y) { return x / y; }
dvariable mydvariablediv(const double x, const double y)
{
  dvariable result;
  result = x / y;
  return result;
}
TEST_F(test_gradcalc, futurevariable)
{
  double x = 5;
  double y = 3;

  std::future<double> results_first_term(std::async(mydiv, x, y));
  std::future<double> results_second_term(std::async([](double x, double y)
    {
      return x * y;
    }, x, y));

  ASSERT_DOUBLE_EQ(results_first_term.get(), 5.0 / 3.0);
  ASSERT_DOUBLE_EQ(results_second_term.get(), 5.0 * 3.0);
}
template <class Fn, class... Args>
std::future<std::pair<double,dvector>> async_compute_gradients(Fn&& fn, Args&&... args)
{
  std::future<std::pair<double,dvector>> results(
    std::async([=]() { return compute_gradients2(fn, args...); })
  );
  return std::move(results);
}
TEST_F(test_gradcalc, async_compute_gradients)
{
  dvector dv(1, 2);
  dv(1) = 60;
  dv(2) = 61;
  double d = 100;
  std::vector<double> stdvector{100};

  std::future<std::pair<double,dvector>> future_results = async_compute_gradients(
    [](dvar_vector values, dvariable d, std::vector<double> stdvector)
    {
      dvariable total;
      dvar_vector results = mfexp(values);
      total = sum(results) + mfexp(d);
      return total;
    }, dv, independent_variables(d), stdvector);

  future_results.wait();
  std::pair<double,dvector> results = future_results.get();
  cout << results.first << ' ' << results.second << endl;
}
