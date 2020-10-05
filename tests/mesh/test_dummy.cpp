#include <gmock/gmock.h>// Toujours inclu
#include <gtest/gtest.h>// Toujours inclu


double Division(double a, double b) {
  if (b == 0) {
	return 0;
  } else
	return a / b;
}

TEST(Test_Dummy, Dummy_1) {
  int a = 2;
  int b = 4;
  double c = Division(a, b);
  EXPECT_EQ(c, 0.5);
}

TEST(Test_Dummy, Dummy_2) {
  EXPECT_EQ(Division(1, 0), 0);
}