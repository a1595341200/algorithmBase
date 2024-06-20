/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-06-12 14:33:29
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-06-21 11:49:00
 * @FilePath: /cplusplus/submodule/algorithmBase/include/Utils.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once
#include <vector>

#include <eigen3/Eigen/Dense>

/**
 * @namespace algorithmBase
 * @brief 包含算法基础功能的命名空间
 */
namespace algorithmBase {
/**
 * @brief 结构体 ObjectAttribute 表示物体的属性。
 * 
 * 该结构体包含物体的宽度、长度和朝向。
 */
struct ObjectAttribute {
    float width{};
    float length{};
    float heading{};
};

using Radian = double;

/**
 * @brief 计算两个向量之间的夹角（以弧度表示）
 * @param v1 第一个向量
 * @param v2 第二个向量
 * @return 夹角的弧度值
 */
Radian calculateAngleBetweenVectors(Eigen::Vector2d& v1, Eigen::Vector2d& v2);

/**
 * @brief 计算两个向量之间的夹角（以弧度表示）带方向逆时针为负, 顺时针为负。
 * @param v1 第一个向量
 * @param v2 第二个向量
 * @return 夹角的弧度值
 */
Radian calculateAngleBetweenVectors2(const Eigen::Vector2d& v1, const Eigen::Vector2d& v2);

/**
 * @brief 根据中心点和物体属性计算角点坐标
 * @param centerPoint 中心点坐标
 * @param objectAttribute 物体属性
 * @return 角点坐标的向量
 */
std::vector<Eigen::Vector2f> calculateCornerPointsByCenterPoint(
    const Eigen::Vector2f& centerPoint, const ObjectAttribute& objectAttribute);

/**
 * @brief 计算最近点到最近边的距离
 * @param x 最近点的x坐标
 * @param y 最近点的y坐标
 * @param half_length 物体长度的一半
 * @param half_width 物体宽度的一半
 * @param angle 物体的朝向角度
 * @param nearest_side 最近边的标识
 * @return 最近点到最近边的距离
 */
std::vector<Eigen::Vector2f> nearestSideToNearestPoint(double x, double y, double half_length,
                                                       double half_width, double angle,
                                                       uint8_t nearest_side);

}  // namespace algorithmBase
