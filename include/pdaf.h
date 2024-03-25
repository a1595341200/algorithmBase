/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-23 21:35:41
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-25 11:08:37
 * @FilePath: /cplusplus/submodule/algorithmBase/include/pdaf.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once
#include <vector>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <eigen3/Eigen/Dense>

namespace algorithmBase {
class pdaf {
public:
    pdaf();
    void prediction(float dt);
    void update(const std::vector<Eigen::Vector2f>& mess);

public:
    // 1 预测状态
    void statePrediction(float dt);
    // 2 预测协方差矩阵
    void errorCovPrediction();
    // 3 预测的观测值
    void measurementPrediction();
    // 4 预测新息向量
    void innovationPrediction(const std::vector<Eigen::Vector2f>& mess);
    // 5 预测新息的协方差矩阵
    void innovationErrorCovPrediction();
    // 6 卡尔曼滤波增益矩阵
    void kalmanGain();
    // 7 等效新息向量
    void equivalentInnovation(const std::vector<Eigen::Vector2f>& mess);
    // 8 更新状态
    void stateUpdate();
    // 9 更新协方差矩阵
    void errorCovUpdate(const std::vector<Eigen::Vector2f>& mess);

    double calculateBi(const std::vector<Eigen::Vector2f>& mess, size_t index);
    double calculateLi(const std::vector<Eigen::Vector2f>& mess, size_t index);
    double sumLi(const std::vector<Eigen::Vector2f>& mess);
    float validationGateVolume();

    Eigen::Vector2f X;               // 状态
    Eigen::Matrix2f A;               // 状态方程
    Eigen::Matrix2f Q;               // 过程噪声协方差矩阵
    Eigen::Matrix2f R;               // 测量噪声的方差矩阵
    Eigen::Matrix2f K;               // 卡尔曼滤波增益
    Eigen::Matrix2f P;               // 预测的状态估计误差的协方差
    Eigen::Matrix2f H;               // 观测矩阵
    Eigen::Vector2f Z;               // 预测的观测值
    std::vector<Eigen::Vector2f> V;  // 预测的新息向量
    Eigen::Matrix2f S;               // 预测的新息向量协方差矩阵
    Eigen::Vector2f EV;              // 等效新息向量
    // 表示在已知所有的有效观测值后，当前K时刻第i个观测值与第i个观测值来自这个目标的概率。将
    // 这个概率当做权重,求新息向量的加权和作为等效新息向量
    std::vector<float> Bi;
    Eigen::Matrix2f Pc;  // cov matrix of the stated update with the correct measurement
    Eigen::Matrix2f Ps;  // spread of innovations
    float Pd{0.35};  // 目标的检测概率, 有传感器性能决定, 是一个已知值
    float Pg{0.9999};  // 目标的检测概率, 有传感器性能决定, 是一个已知值
};

}  // namespace algorithmBase
