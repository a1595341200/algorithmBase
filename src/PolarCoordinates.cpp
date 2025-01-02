/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2025-01-02 15:40:48
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2025-01-02 15:44:23
 * @FilePath: /cplusplus/submodule/algorithmBase/src/PolarCoordinates.cpp
 * @Description: 极坐标实现文件
 */

#include "PolarCoordinates.h"
#include <cmath>

namespace algorithmBase {

PolarCoordinates::PolarCoordinates(double r, double theta)
    : r_(r), theta_(theta) {}

void PolarCoordinates::setPolar(double r, double theta) {
    r_ = r;
    theta_ = theta;
}

void PolarCoordinates::setCartesian(double x, double y) {
    r_ = std::sqrt(x * x + y * y);
    theta_ = std::atan2(y, x);
}

double PolarCoordinates::getR() const {
    return r_;
}

double PolarCoordinates::getTheta() const {
    return theta_;
}

double PolarCoordinates::getX() const {
    return r_ * std::cos(theta_);
}

double PolarCoordinates::getY() const {
    return r_ * std::sin(theta_);
}

void PolarCoordinates::toCartesian(double& x, double& y) const {
    x = getX();
    y = getY();
}

PolarCoordinates PolarCoordinates::fromCartesian(double x, double y) {
    double r = std::sqrt(x * x + y * y);
    double theta = std::atan2(y, x);
    return PolarCoordinates(r, theta);
}

void PolarCoordinates::polarToCartesian(double r, double theta, double& x, double& y) {
    x = r * std::cos(theta);
    y = r * std::sin(theta);
}

} // namespace algorithmBase
