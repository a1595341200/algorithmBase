/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-06-12 14:33:29
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-06-12 14:53:10
 * @FilePath: /cplusplus/submodule/algorithmBase/include/Utils.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once
#include <vector>

#include <eigen3/Eigen/Dense>

namespace algorithmBase {
struct ObjectAttribute {
    float width{};
    float height{};
    float heading{};
};

std::vector<Eigen::Vector2f> calculateCornerPointsByCenterPoint(
    const Eigen::Vector2f& centerPoint, const ObjectAttribute& objectAttribute);

std::vector<Eigen::Vector2f> nearestSideToNearestPoint(double x, double y, double half_length,
                                                       double half_width, double angle,
                                                       uint8_t nearest_side);

}  // namespace algorithmBase
