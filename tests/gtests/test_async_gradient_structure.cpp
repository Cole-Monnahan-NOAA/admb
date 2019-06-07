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
