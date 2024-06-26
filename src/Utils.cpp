/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-06-12 14:46:58
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-06-20 13:48:53
 * @FilePath: /cplusplus/submodule/algorithmBase/src/Utils.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "Utils.h"

namespace algorithmBase {
std::vector<Eigen::Vector2f> calculateCornerPointsByCenterPoint(
    const Eigen::Vector2f& centerPoint, const ObjectAttribute& objectAttribute) {
    std::vector<Eigen::Vector2f> res;
    float c = std::cos(objectAttribute.heading);
    float s = std::sin(objectAttribute.heading);

    res.emplace_back(
        centerPoint.x() + objectAttribute.length / 2.0f * c - objectAttribute.width / 2.0f * s,
        centerPoint.y() - objectAttribute.length / 2.0f * s - objectAttribute.width / 2.0f * c);

    res.emplace_back(
        centerPoint.x() - objectAttribute.length / 2.0f * c - objectAttribute.width / 2.0f * s,
        centerPoint.y() + objectAttribute.length / 2.0f * s - objectAttribute.width / 2.0f * c);

    res.emplace_back(
        centerPoint.x() - objectAttribute.length / 2.0f * c + objectAttribute.width / 2.0f * s,
        centerPoint.y() + objectAttribute.length / 2.0f * s + objectAttribute.width / 2.0f * c);

    res.emplace_back(
        centerPoint.x() + objectAttribute.length / 2.0f * c + objectAttribute.width / 2.0f * s,
        centerPoint.y() - objectAttribute.length / 2.0f * s + objectAttribute.width / 2.0f * c);
    return res;
}

std::vector<Eigen::Vector2f> nearestSideToNearestPoint(double x, double y, double half_length,
                                                       double half_width, double angle,
                                                       uint8_t nearest_side) {
    double c = cos(angle);
    double s = sin(angle);
    if (nearest_side == 0) {
    } else if (nearest_side == 1) {
        x = x + half_length * c;
        y = y + half_length * s;
    } else if (nearest_side == 2) {
        x = x - half_length * c;
        y = y - half_length * s;
    } else if (nearest_side == 3) {
        x = x - half_width * s;
        y = y + half_width * c;
    } else if (nearest_side == 4) {
        x = x + half_width * s;
        y = y - half_width * c;
    }
    double r1x = -half_length * c - half_width * s;
    double r1y = -half_length * s + half_width * c;
    double r2x = half_length * c - half_width * s;
    double r2y = half_length * s + half_width * c;
    return {Eigen::Vector2f(x + r1x, y + r1y), Eigen::Vector2f(x + r2x, y + r2y),
            Eigen::Vector2f(x - r1x, y - r1y), Eigen::Vector2f(x - r2x, y - r2y),
            Eigen::Vector2f(x + r1x, y + r1y)};
}

Radian calculateAngleBetweenVectors(Eigen::Vector2d& v1, Eigen::Vector2d& v2) {
    return std::acos(v1.dot(v2) / (v1.norm() * v2.norm()));
}

Radian calculateAngleBetweenVectors2(const Eigen::Vector2d& v1, const Eigen::Vector2d& v2) {
    double res = std::acos(v1.dot(v2) / (v1.norm() * v2.norm()));
    double d = std::asin(v1.dot(v2) / (v1.norm() * v2.norm()));
    if (d < 0) {
        res = -res;
    }
    return res;
}

}  // namespace algorithmBase
