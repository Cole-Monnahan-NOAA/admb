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
  gradient_structure* p1 = gs.get();
  ASSERT_TRUE(p1 == gradient_structure::get());
  std::vector<std::future<void>> futures;
  for(int i = 0; i < size; ++i)
  {
    futures.push_back(std::async([&f, p1]()
    {
      gradient_structure copy(*p1);
      gradient_structure* p2 = gradient_structure::get();
      ASSERT_TRUE(p2 == &copy);
      ASSERT_TRUE(p1 != p2);
      ASSERT_TRUE(p2->get_ARRAY_MEMBLOCK_BASE() != NULL);
      ASSERT_TRUE(p1->get_ARRAY_MEMBLOCK_BASE() != NULL);
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
TEST_F(test_async_gradient_structure, copy_amb)
{
  gradient_structure gs;
  gradient_structure* p1 = gradient_structure::get();
  ASSERT_TRUE(p1 == &gs);
  double* dptr1 = p1->get_ARRAY_MEMBLOCK_BASE();
  ASSERT_TRUE(dptr1 != NULL);

  gradient_structure copy(gs);
  gradient_structure* p2 = gradient_structure::get();
  ASSERT_TRUE(p2 == &copy);
  ASSERT_TRUE(p2->get_ARRAY_MEMBLOCK_BASE() != NULL);
  double* dptr2 = p2->get_ARRAY_MEMBLOCK_BASE();
  ASSERT_TRUE(dptr2 != NULL);

  ASSERT_TRUE(p1 != p2);
  ASSERT_TRUE(dptr1 != dptr2);
}
TEST_F(test_async_gradient_structure, copy_amb_lambda)
{
  gradient_structure gs;
  gradient_structure* p1 = gradient_structure::get();
  ASSERT_TRUE(p1 == &gs);
  double* dptr1 = p1->get_ARRAY_MEMBLOCK_BASE();
  ASSERT_TRUE(dptr1 != NULL);

  std::function<void(void)> f = [p1, dptr1]()
  {
    gradient_structure copy(*p1);
    gradient_structure* p2 = gradient_structure::get();
    ASSERT_TRUE(p2 == &copy);
    ASSERT_TRUE(p2->get_ARRAY_MEMBLOCK_BASE() != NULL);
    double* dptr2 = p2->get_ARRAY_MEMBLOCK_BASE();
    ASSERT_TRUE(dptr2 != NULL);

    ASSERT_TRUE(p1 != p2);
    ASSERT_TRUE(dptr1 != dptr2);
  };

  f();
}
TEST_F(test_async_gradient_structure, copy_amb_lambda_async)
{
  gradient_structure gs;
  gradient_structure* p1 = gradient_structure::get();
  ASSERT_TRUE(p1 == &gs);
  double* dptr1 = p1->get_ARRAY_MEMBLOCK_BASE();
  ASSERT_TRUE(dptr1 != NULL);

  std::function<void(void)> f = [p1, dptr1]()
  {
    gradient_structure copy(*p1);
    gradient_structure* p2 = gradient_structure::get();
    ASSERT_TRUE(p2 == &copy);
    ASSERT_TRUE(p2->get_ARRAY_MEMBLOCK_BASE() != NULL);
    double* dptr2 = p2->get_ARRAY_MEMBLOCK_BASE();
    ASSERT_TRUE(dptr2 != NULL);

    ASSERT_TRUE(p1 != p2);
    ASSERT_TRUE(dptr1 != dptr2);
  };

  std::vector<std::future<void>> futures;
  for(int i = 0; i < 10; ++i)
  {
    futures.push_back(std::async(f));
  }
  for(auto& element: futures)
  {
    element.get();
  }
}
TEST_F(test_async_gradient_structure, copy_arr_list1_lambda_async)
{
  gradient_structure gs;
  gradient_structure* p1 = gradient_structure::get();
  ASSERT_TRUE(p1 == &gs);
  arr_list* ptr1 = &(gs.ARR_LIST1);

  std::function<void(void)> f = [p1, ptr1]()
  {
    gradient_structure copy(*p1);
    arr_list* ptr2 = &(copy.ARR_LIST1);

    ASSERT_TRUE(ptr1 != ptr2);
  };

  std::vector<std::future<void>> futures;
  for(int i = 0; i < 10; ++i)
  {
    futures.push_back(std::async(f));
  }
  for(auto& element: futures)
  {
    element.get();
  }
}
TEST_F(test_async_gradient_structure, copy_arr_new_lambda_async)
{
  gradient_structure gs;
  gradient_structure* p1 = gradient_structure::get();
  ASSERT_TRUE(p1 == &gs);
  arr_list* ptr1 = &(gs.ARR_LIST1);

  std::function<void(void)> f = [p1, ptr1]()
  {
    gradient_structure copy(*p1);
    arr_list* ptr2 = &(copy.ARR_LIST1);

    ASSERT_TRUE(ptr1 != ptr2);
    ASSERT_TRUE(copy.ARR_LIST1.get_free_last() == NULL);
    ASSERT_TRUE(copy.ARR_LIST1.get_number_arr_links() == 0);
    ASSERT_TRUE(copy.ARR_LIST1.get_last() == NULL);
    ASSERT_TRUE(copy.ARR_LIST1.get_last_offset() == 0);

    double_and_int* arr = arr_new(1);
    arr_link* ptr_arr_link = *(arr_link **)arr;

    ASSERT_TRUE(copy.ARR_LIST1.get_number_arr_links() == 1);
    ASSERT_TRUE(ptr_arr_link->get_prev() == NULL);
    ASSERT_TRUE(copy.ARR_LIST1.get_last() == ptr_arr_link);
    ASSERT_TRUE(ptr_arr_link->get_next() == NULL);
    ASSERT_TRUE(ptr_arr_link->get_status() == 1);
    ASSERT_TRUE(ptr_arr_link->get_offset() == 0);
    ASSERT_TRUE(ptr_arr_link->get_size() == sizeof(double_and_int));
    ASSERT_TRUE(static_cast<void*>(arr) == static_cast<void*>(copy.get_ARRAY_MEMBLOCK_BASE()));
  };

  std::vector<std::future<void>> futures;
  for(int i = 0; i < 10; ++i)
  {
    futures.push_back(std::async(f));
  }
  for(auto& element: futures)
  {
    element.get();
  }
}
TEST_F(test_async_gradient_structure, copy_amb_lambda_async2)
{
  gradient_structure gs;
  gradient_structure* p1 = gradient_structure::get();
  ASSERT_TRUE(p1 == &gs);
  double* dptr1 = p1->get_ARRAY_MEMBLOCK_BASE();
  ASSERT_TRUE(dptr1 != NULL);

  std::function<void(void)> f = [gs, dptr1]()
  {
    gradient_structure copy(gs);
    gradient_structure* p2 = gradient_structure::get();
    ASSERT_TRUE(p2 == &copy);
    ASSERT_TRUE(p2->get_ARRAY_MEMBLOCK_BASE() != NULL);
    double* dptr2 = p2->get_ARRAY_MEMBLOCK_BASE();
    ASSERT_TRUE(dptr2 != NULL);

    ASSERT_TRUE(&gs != p2);
    ASSERT_TRUE(dptr1 != dptr2);
    ASSERT_TRUE(gs.get_ARRAY_MEMBLOCK_BASE() != dptr2);
  };

  std::vector<std::future<void>> futures;
  for(int i = 0; i < 10; ++i)
  {
    futures.push_back(std::async(f));
  }
  for(auto& element: futures)
  {
    element.get();
  }
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
TEST_F(test_async_gradient_structure, ARR_LIST1)
{
  ASSERT_EQ(gradient_structure::get_instances(), 0);
  gradient_structure gs;
  ASSERT_TRUE(gradient_structure::get() == &gs);
  ASSERT_TRUE(gs.ARR_LIST1.get_last() == NULL);
  ASSERT_TRUE(gs.ARR_LIST1.get_last_offset() == 0);
  ASSERT_TRUE(gs.ARR_LIST1.get_number_arr_links() == 0);
  ASSERT_TRUE(gs.ARR_LIST1.get_max_last_offset() == 0);
  asyncs3(10, gs, []()
  {
    gradient_structure* gsptr = gradient_structure::get();
    ASSERT_TRUE(gsptr->ARR_LIST1.get_last() == NULL);
    ASSERT_TRUE(gsptr->ARR_LIST1.get_last_offset() == 0);
    ASSERT_TRUE(gsptr->ARR_LIST1.get_number_arr_links() == 0);
    ASSERT_TRUE(gsptr->ARR_LIST1.get_max_last_offset() == 0);
  });
  ASSERT_EQ(gradient_structure::get_instances(), 1);
  ASSERT_TRUE(gradient_structure::get() == &gs);
  ASSERT_TRUE(gs.ARR_LIST1.get_last() == NULL);
  ASSERT_TRUE(gs.ARR_LIST1.get_last_offset() == 0);
  ASSERT_TRUE(gs.ARR_LIST1.get_number_arr_links() == 0);
  ASSERT_TRUE(gs.ARR_LIST1.get_max_last_offset() == 0);
}
TEST_F(test_async_gradient_structure, arr_new_0)
{
  ASSERT_EQ(gradient_structure::get_instances(), 0);
  gradient_structure gs;
  ASSERT_TRUE(gradient_structure::get() == &gs);
  ASSERT_TRUE(gs.ARR_LIST1.get_last() == NULL);
  ASSERT_TRUE(gs.ARR_LIST1.get_last_offset() == 0);
  ASSERT_TRUE(gs.ARR_LIST1.get_number_arr_links() == 0);
  ASSERT_TRUE(gs.ARR_LIST1.get_max_last_offset() == 0);
  double* ptr_amb = gs.get_ARRAY_MEMBLOCK_BASE();
  asyncs3(10, gs, []()
  {
    gradient_structure* gsptr = gradient_structure::get();
    double* ptr_amb = gsptr->get_ARRAY_MEMBLOCK_BASE();
    ASSERT_TRUE(gsptr->ARR_LIST1.get_last() == NULL);
    ASSERT_TRUE(gsptr->ARR_LIST1.get_last_offset() == 0);
    ASSERT_TRUE(gsptr->ARR_LIST1.get_number_arr_links() == 0);
    ASSERT_TRUE(gsptr->ARR_LIST1.get_max_last_offset() == 0);

    double_and_int* arr = arr_new(0);
    arr_link* ptr = *(arr_link**)arr;

    ASSERT_TRUE(static_cast<void*>(arr) == static_cast<void*>(ptr_amb));

    ASSERT_TRUE(gsptr->ARR_LIST1.get_last() == ptr);
    ASSERT_TRUE(gsptr->ARR_LIST1.get_last_offset() == 0);
    ASSERT_TRUE(gsptr->ARR_LIST1.get_number_arr_links() == 1);
    ASSERT_TRUE(gsptr->ARR_LIST1.get_max_last_offset() == 0);

    ASSERT_TRUE(ptr->get_prev() == NULL);
    ASSERT_TRUE(ptr->get_next() == NULL);
    ASSERT_TRUE(ptr->get_size() == 0);
    ASSERT_TRUE(ptr->get_offset() == 0);
    ASSERT_TRUE(ptr->get_status() == 1);

    arr_free(arr);

    ASSERT_TRUE(gsptr->ARR_LIST1.get_last() == NULL);
    ASSERT_TRUE(gsptr->ARR_LIST1.get_last_offset() == 0);
    ASSERT_TRUE(gsptr->ARR_LIST1.get_number_arr_links() == 0);
    ASSERT_TRUE(gsptr->ARR_LIST1.get_max_last_offset() == 0);
  });
  ASSERT_EQ(gradient_structure::get_instances(), 1);
  ASSERT_TRUE(gradient_structure::get() == &gs);
  ASSERT_TRUE(gs.ARR_LIST1.get_last() == NULL);
  ASSERT_TRUE(gs.ARR_LIST1.get_last_offset() == 0);
  ASSERT_TRUE(gs.ARR_LIST1.get_number_arr_links() == 0);
  ASSERT_TRUE(gs.ARR_LIST1.get_max_last_offset() == 0);
  ASSERT_TRUE(gs.get_ARRAY_MEMBLOCK_BASE() == ptr_amb);
}
TEST_F(test_async_gradient_structure, arr_new_1)
{
  ASSERT_EQ(gradient_structure::get_instances(), 0);
  gradient_structure gs;
  ASSERT_TRUE(gradient_structure::get() == &gs);
  ASSERT_TRUE(gs.ARR_LIST1.get_last() == NULL);
  ASSERT_TRUE(gs.ARR_LIST1.get_last_offset() == 0);
  ASSERT_TRUE(gs.ARR_LIST1.get_number_arr_links() == 0);
  ASSERT_TRUE(gs.ARR_LIST1.get_max_last_offset() == 0);
  double* ptr_amb = gs.get_ARRAY_MEMBLOCK_BASE();
  asyncs3(10, gs, []()
  {
    gradient_structure* gsptr = gradient_structure::get();
    double* ptr_amb = gsptr->get_ARRAY_MEMBLOCK_BASE();
    ASSERT_TRUE(gsptr->ARR_LIST1.get_last() == NULL);
    ASSERT_TRUE(gsptr->ARR_LIST1.get_last_offset() == 0);
    ASSERT_TRUE(gsptr->ARR_LIST1.get_number_arr_links() == 0);
    ASSERT_TRUE(gsptr->ARR_LIST1.get_max_last_offset() == 0);

    double_and_int* arr = arr_new(1);
    ASSERT_TRUE(static_cast<void*>(arr) == static_cast<void*>(ptr_amb));

    arr_link* ptr = *(arr_link**)arr;
    ASSERT_TRUE(ptr->get_prev() == NULL);
    ASSERT_TRUE(ptr->get_next() == NULL);
    ASSERT_TRUE(ptr->get_size() == sizeof(double_and_int));
    ASSERT_TRUE(ptr->get_offset() == 0);
    ASSERT_TRUE(ptr->get_status() == 1);
  });
  ASSERT_EQ(gradient_structure::get_instances(), 1);
  ASSERT_TRUE(gradient_structure::get() == &gs);
  ASSERT_TRUE(gs.ARR_LIST1.get_last() == NULL);
  ASSERT_TRUE(gs.ARR_LIST1.get_last_offset() == 0);
  ASSERT_TRUE(gs.ARR_LIST1.get_number_arr_links() == 0);
  ASSERT_TRUE(gs.ARR_LIST1.get_max_last_offset() == 0);
  ASSERT_TRUE(gs.get_ARRAY_MEMBLOCK_BASE() == ptr_amb);
}
