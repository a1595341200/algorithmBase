/*
 * @Author: yao.xie 
 * @Date: 2025-01-02
 * @Description: PolarCoordinates 测试用例
 */

#include <gtest/gtest.h>
#include "PolarCoordinates.h"
#include <cmath>

using namespace algorithmBase;

class PolarCoordinatesTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 设置测试精度
        epsilon = 1e-10;
    }
    
    double epsilon{};
};

// 测试构造函数和基本getter
TEST_F(PolarCoordinatesTest, ConstructorAndGetters) {
    PolarCoordinates point(5.0, M_PI / 4);  // 45度角，半径5
    
    EXPECT_DOUBLE_EQ(point.getR(), 5.0);
    EXPECT_DOUBLE_EQ(point.getTheta(), M_PI / 4);
}

// 测试极坐标到笛卡尔坐标的转换
TEST_F(PolarCoordinatesTest, PolarToCartesian) {
    PolarCoordinates point(2.0, M_PI / 6);  // 30度角，半径2
    
    double x, y;
    point.toCartesian(x, y);
    
    // 预期值：x = 2 * cos(π/6), y = 2 * sin(π/6)
    EXPECT_NEAR(x, 2.0 * std::cos(M_PI / 6), epsilon);
    EXPECT_NEAR(y, 2.0 * std::sin(M_PI / 6), epsilon);
}

// 测试笛卡尔坐标到极坐标的转换
TEST_F(PolarCoordinatesTest, CartesianToPolar) {
    double x = 3.0, y = 4.0;  // 3-4-5三角形
    auto point = PolarCoordinates::fromCartesian(x, y);
    
    EXPECT_NEAR(point.getR(), 5.0, epsilon);  // 半径应该是5
    EXPECT_NEAR(point.getTheta(), std::atan2(4.0, 3.0), epsilon);  // 角度应该是arctan(4/3)
}

// 测试静态转换方法
TEST_F(PolarCoordinatesTest, StaticConversion) {
    double x_out, y_out;
    PolarCoordinates::polarToCartesian(2.0, M_PI / 2, x_out, y_out);  // 90度角，半径2
    
    EXPECT_NEAR(x_out, 0.0, epsilon);  // cos(90°) = 0
    EXPECT_NEAR(y_out, 2.0, epsilon);  // sin(90°) = 1, r = 2
}

// 测试特殊情况
TEST_F(PolarCoordinatesTest, SpecialCases) {
    // 原点
    PolarCoordinates origin(0.0, 0.0);
    double x, y;
    origin.toCartesian(x, y);
    EXPECT_NEAR(x, 0.0, epsilon);
    EXPECT_NEAR(y, 0.0, epsilon);
    
    // 负半径的处理
    PolarCoordinates negative_r(-2.0, M_PI / 4);
    negative_r.toCartesian(x, y);
    EXPECT_NEAR(x, -2.0 * std::cos(M_PI / 4), epsilon);
    EXPECT_NEAR(y, -2.0 * std::sin(M_PI / 4), epsilon);
}

// 测试设置方法
TEST_F(PolarCoordinatesTest, SetMethods) {
    PolarCoordinates point;
    
    // 测试setPolar
    point.setPolar(3.0, M_PI / 3);
    EXPECT_NEAR(point.getR(), 3.0, epsilon);
    EXPECT_NEAR(point.getTheta(), M_PI / 3, epsilon);
    
    // 测试setCartesian
    point.setCartesian(1.0, 1.0);
    EXPECT_NEAR(point.getR(), std::sqrt(2.0), epsilon);
    EXPECT_NEAR(point.getTheta(), M_PI / 4, epsilon);
}


// 添加 main 函数
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
