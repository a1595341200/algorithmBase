/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-11 14:41:36
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-25 16:19:38
 * @FilePath: /cplusplus/submodule/algorithmBase/include/KalmanFilteringParameter.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#ifndef KALMANFILTERINGPARAMETER_H
#define KALMANFILTERINGPARAMETER_H
#include <memory>

#include <eigen3/Eigen/Dense>
#include "DynamicModel.h"

class KalmanFilteringParameter {
public:
    int measurementDimension{};    //量测维度
    int stateDimension{};          //状态维度
    std::string kinematicModel{};  //动力学模型名称
    std::unique_ptr<DynamicModelBase> dynamicModel;
    Eigen::VectorXd X;  // 状态向量
    Eigen::MatrixXd P;  //状态协方差
    Eigen::MatrixXd Q;  //状态白噪声
    Eigen::MatrixXd R;  //量测白噪声
    Eigen::MatrixXd K;  //卡尔曼滤波增益
    void prediction(Eigen::VectorXd& X, float dt);
};

class UKFKalmanFilteringParameter {
public:
    UKFKalmanFilteringParameter() {
        kfp.dynamicModel = std::make_unique<CTRA>();
    }
    KalmanFilteringParameter kfp;
    Eigen::MatrixXd sigmaPoints;     // sigma点
    Eigen::MatrixXd sigmaPointsPre;  //预测sigma
    Eigen::MatrixXd zSigma;          //先验在量测空间sigma点
    Eigen::VectorXd zPre;            //先验在量测空间均值
    Eigen::MatrixXd S;               //先验在量测空间协方差
    Eigen::VectorXd zminus;          //实际量测与预测量测差值
    Eigen::VectorXd wm;              //状态权重
    Eigen::VectorXd wc;              //方差权重
    float alpha{1};                   //权重参数影响的是Sigma 点远离均值的程度
    float beta{2};                    //权重参数影响的是Sigma 点远离均值的程度
    float lamda{};                   //超参数,sigma点越小越集中
};

#endif /* KALMANFILTERINGPARAMETER_H */
