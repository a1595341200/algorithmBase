/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-04-12 13:23:54
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-04-12 14:07:49
 * @FilePath: /cplusplus/submodule/algorithmBase/include/CovarianceIntersection.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once

#include <eigen3/Eigen/Dense>

class CovarianceIntersection {
public:
    CovarianceIntersection();
    void update(const Eigen::VectorXf& z);
    void prediction(double dt);

// private:
    Eigen::MatrixXf A;
    Eigen::MatrixXf Q;
    Eigen::MatrixXf P;
    Eigen::VectorXf X;
    Eigen::MatrixXf R;
    Eigen::VectorXf H;
};
