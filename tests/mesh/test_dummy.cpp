#include <gtest/gtest.h>    // Toujours inclu
#include <gmock/gmock.h>    // Toujours inclu


float Division(int a, int b){
    return a/b;
}

TEST(Test_Dummy, Test_Dummy_1) {
    int a = 2;
    int b = 4;
    double c = Division(a,b);
    EXPECT_EQ(c, 0.5);
}

TEST(Test_Dummy, Test_Dummy_1) {
    EXPECT_EQ(Division(1,0), 0); 
}