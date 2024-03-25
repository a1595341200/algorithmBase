/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-11 15:07:34
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-11 15:47:29
 * @FilePath: /cplusplus/src/algorithm/ukf/src/DynamicModel.cpp
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include "DynamicModel.h"

void CTRA::prediction(Eigen::VectorXd& X, float dt) {
    assert(X.size() != 6);
    float px_k(0.0), py_k(0.0), velo_k(0.0), yaw_k(0.0), yawd_k(0.0), a_k(0.0);
    px_k = X(0);
    py_k = X(1);
    velo_k = X(2);
    yaw_k = X(3);
    yawd_k = X(4);
    a_k = X(5);

    if (fabs(yawd_k) > 0.001) {
        px_k += ((velo_k * yawd_k + a_k * yawd_k * dt) * std::sin(yaw_k + yawd_k * dt) +
                 a_k * std::cos(yaw_k + yawd_k * dt) - velo_k * yawd_k * std::sin(yaw_k) -
                 a_k * std::cos(yaw_k)) /
                (yawd_k * yawd_k);
        py_k += ((-velo_k * yawd_k - a_k * yawd_k * dt) * std::cos(yaw_k + yawd_k * dt) +
                 a_k * std::sin(yaw_k + yawd_k * dt) + velo_k * yawd_k * std::cos(yaw_k) -
                 a_k * std::sin(yaw_k)) /
                (yawd_k * yawd_k);
    } else {
        px_k += (velo_k * dt + 0.5 * a_k * dt * dt) * cos(yaw_k);
        py_k += (velo_k * dt + 0.5 * a_k * dt * dt) * sin(yaw_k);
    }

    velo_k += a_k * dt;
    yaw_k += yawd_k * dt;

    X(0) = px_k;
    X(1) = py_k;
    X(2) = velo_k;
    X(3) = yaw_k;
    X(4) = yawd_k;
    X(5) = a_k;
}
