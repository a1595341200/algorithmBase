/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-11 10:56:53
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-25 16:09:55
 * @FilePath: /cplusplus/submodule/algorithmBase/include/ukf.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#ifndef MY_UKF_H
#define MY_UKF_H
#include <stdlib.h>

#include <iostream>
#include <string>
#include <unordered_map>

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>

#include "KalmanFilteringParameter.h"

// https://zhuanlan.zhihu.com/p/399370551
// https://zhuanlan.zhihu.com/p/482392082
// https://zhuanlan.zhihu.com/p/359811364
//原文参考S. J. Julier, J. K. Uhlmann and H. F. Durrant-Whyte, "A new approach for filtering
//nonlinear systems," Proceedings of 1995 American Control Conference - ACC'95, Seattle, WA, USA,
//1995, pp. 1628-1632 vol.3, doi: 10.1109/ACC.1995.529783.
//关键：UT变换无迹变换是计算一系列点（关键点，Sigma
// Point），然后通过非线性函数进行变换，通过变换结果和对应的权重来计算高斯分布

class UKF {
public:
    UKF(std::string smodel, int state_n, int mea_n, Eigen::MatrixXd Q, Eigen::MatrixXd R,
        Eigen::MatrixXd P);
    void initialization(Eigen::VectorXd& X, Eigen::MatrixXd& P, float time);
    bool isinitalized();
    void predictionZ(Eigen::VectorXd& X, Eigen::MatrixXd& P, float ntime);
    void update(Eigen::VectorXd& Z);
    Eigen::VectorXd getState();
    Eigen::MatrixXd getCovariance();
    Eigen::VectorXd getZminus();
    Eigen::MatrixXd getS();
    Eigen::VectorXd getPredictionZ();

private:
    void makeSigmaPoints();
    void prediction(float ntime);

    bool is_initalized_ = false;

    float pretime{};

    std::unordered_map<std::string, int> model_hash_ = {{"CV", 1}, {"CTRV", 2}, {"CTRA", 3}};
    int model_{};
    UKFKalmanFilteringParameter parameter;
};
#endif
