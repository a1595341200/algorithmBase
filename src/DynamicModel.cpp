/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-11 15:07:34
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-07-01 21:16:31
 * @FilePath: /cplusplus/submodule/algorithmBase/src/DynamicModel.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "DynamicModel.h"

#include <Log.h>

#include <cmath>

void CTRA::prediction(Eigen::VectorXd& X, Eigen::MatrixXd& P, float dt) {
    (void)P;
    assert(X.size() != 6);
    float px_k(0.0), py_k(0.0), velo_k(0.0), yaw_k(0.0), yawd_k(0.0), a_k(0.0);
    px_k = static_cast<float>(X(0));
    py_k = static_cast<float>(X(1));
    velo_k = static_cast<float>(X(2));
    yaw_k = static_cast<float>(X(3));
    yawd_k = static_cast<float>(X(4));
    a_k = static_cast<float>(X(5));

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
        px_k += (velo_k * dt + 0.5f * a_k * dt * dt) * static_cast<float>(cos(yaw_k));
        py_k += (velo_k * dt + 0.5f * a_k * dt * dt) * static_cast<float>(sin(yaw_k));
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

CA::CA() {
    Pacc.resize(6, 6);
    double fx = 0.01;
    double fy = 0.01;
    // clang-format off
    Pacc << fx, 0, 0, 0, 0, 0,
            0, fy, 0, 0, 0, 0,
            0, 0, fx, 0, 0, 0,
            0, 0, 0, fy, 0, 0,
            0, 0, 0, 0, fx, 0,
            0, 0, 0, 0, 0, fy;

    A << 1, 0, mDt, 0, 0.5*std::pow(mDt,2),  0,
         0, 1, 0, mDt, 0,                   0.5*std::pow(mDt,2),
         0, 0, 1, 0,  mDt,                  0,
         0, 0, 0, 1,  0,                   mDt,
         0, 0, 0, 0,  1,                   0,
         0, 0, 0, 0,  0,                   1;

    Q << 0.05*std::pow(mDt,5), 0,                   std::pow(mDt,4)/8, 0,                std::pow(mDt,3)/6, 0,
         0,                   0.05*std::pow(mDt,5), 0,                std::pow(mDt,4)/8, 0,                std::pow(mDt,3)/6,
         std::pow(mDt,4)/8,    0,                   std::pow(mDt,3)/3, 0,                std::pow(mDt,2)/2, 0,
         0,                   std::pow(mDt,4)/8,    0,                std::pow(mDt,3)/3, 0,                std::pow(mDt,2)/2,
         std::pow(mDt,3)/6,    0,                   std::pow(mDt,2)/2, 0,                mDt,               0,
         0,                   std::pow(mDt,3)/6,    0,                std::pow(mDt,2)/2, 0,                mDt;
    // clang-format on
    H = Eigen::MatrixXd::Identity(6, 6);
}

void CA::prediction(Eigen::VectorXd& X, Eigen::MatrixXd& P, float dt) {
    computationalA(dt);
    computationalQ(dt);
    X = A * X;
    P = A * P * A.transpose() + Pacc * Q;
}

void CA::update(Eigen::VectorXd& X, Eigen::MatrixXd& P, const Eigen::VectorXd& Z,
                const Eigen::MatrixXd& R) {
    auto K = P * H.transpose() * (H * P * H.transpose() + R).inverse();
    X = X + K * (Z - H * X);
    P = (Eigen::MatrixXd::Identity(6, 6) - K * H) * P;
}

Eigen::MatrixXd CA::computationalA(float dt) {
    A(0, 2) = dt;
    A(0, 4) = 0.5 * std::pow(dt, 2);
    A(1, 3) = dt;
    A(1, 5) = 0.5 * std::pow(dt, 2);
    A(2, 4) = dt;
    A(3, 5) = dt;
    return A;
}

Eigen::MatrixXd CA::computationalQ(float dt) {
    Q(0, 0) = 0.05 * std::pow(dt, 5);
    Q(0, 2) = std::pow(dt, 4) / 8;
    Q(0, 4) = std::pow(dt, 3) / 6;

    Q(1, 1) = 0.05 * std::pow(dt, 5);
    Q(1, 3) = std::pow(dt, 4) / 8;
    Q(1, 5) = std::pow(dt, 3) / 6;

    Q(2, 0) = std::pow(dt, 4) / 8;
    Q(2, 2) = std::pow(dt, 3) / 3;
    Q(2, 4) = std::pow(dt, 2) / 2;

    Q(3, 1) = std::pow(dt, 4) / 8;
    Q(3, 3) = std::pow(dt, 3) / 3;
    Q(3, 5) = std::pow(dt, 2) / 2;

    Q(4, 0) = std::pow(dt, 3) / 6;
    Q(4, 2) = std::pow(dt, 2) / 2;
    Q(4, 4) = dt;

    Q(5, 1) = 0;
    Q(5, 3) = std::pow(dt, 2) / 2;
    Q(5, 5) = dt;
    return Q;
}

CT::CT() {
    A = Eigen::MatrixXd::Zero(4, 4);
    Q = Eigen::MatrixXd::Zero(4, 4);
    // clang-format off
    A << 1, std::sin(w*mDt)/w,        0, -(1 - std::cos(w*mDt))/w,
         0, std::cos(w*mDt),          0, -sin(w*mDt), 
         0, (1 - std::cos(w*mDt))/w, 1, std::sin(w*mDt)/w, 
         0, std::sin(w*mDt),          0, cos(w*mDt);

    Q << std::pow(mDt,3)/3, std::pow(mDt,2)/2, 0,                     0,
         std::pow(mDt,2)/2, mDt,                    0,                     0,
         0,                     0,                     std::pow(mDt,3)/3, std::pow(mDt,2)/2,
         0,                     0,                     std::pow(mDt,2)/2, mDt;
    // clang-format on
}

void CT::prediction(Eigen::VectorXd& X, Eigen::MatrixXd& P, float dt) {
    w = static_cast<float>(X(4));
    computationalA(dt);
    computationalQ(dt);
    X.head(4) = A * X.head(4);
    P = A * P * A.transpose() + Pw * Q;
}

Eigen::MatrixXd CT::computationalA(float dt) {
    A(0, 1) = std::sin(w * dt) / w;
    A(0, 3) = -(1 - std::cos(w * dt)) / w;

    A(1, 1) = std::cos(w * dt);
    A(1, 3) = -sin(w * dt);

    A(2, 1) = (1 - std::cos(w * dt)) / w;
    A(2, 3) = std::sin(w * dt) / w;

    A(3, 1) = std::sin(w * dt);
    A(3, 3) = cos(w * dt);
    LOG_INFO("A\n", A);
    return A;
}

Eigen::MatrixXd CT::computationalQ(float dt) {
    Q(0, 0) = std::pow(dt, 3) / 3;
    Q(0, 1) = std::pow(dt, 2) / 2;

    Q(1, 0) = std::pow(dt, 2) / 2;
    Q(1, 1) = dt;

    Q(2, 2) = std::pow(dt, 3) / 3;
    Q(2, 3) = std::pow(dt, 2) / 2;

    Q(3, 2) = std::pow(dt, 2) / 2;
    Q(3, 3) = dt;
    LOG_INFO("Q\n", Q);
    return Q;
}

CV::CV() {
    A = Eigen::MatrixXd::Zero(4, 4);
    Q = Eigen::MatrixXd::Zero(4, 4);
    // clang-format off
    A << 1, 0, mDt, 0, 
         0, 1, 0,  mDt, 
         0, 0, 1,  0,  
         0, 0, 0,  1;

    Q << std::pow(mDt,3)/3, std::pow(mDt,2)/2,  0, 0,
         std::pow(mDt,2)/2, mDt,                0, 0,
         0,                0,                std::pow(mDt,3)/3, std::pow(mDt,2)/2,
         0,                0,                std::pow(mDt,2)/2,                mDt;
    // clang-format on
}

void CV::prediction(Eigen::VectorXd& X, Eigen::MatrixXd& P, float dt) {
    computationalA(dt);
    computationalQ(dt);
    X = A * X;
    P = A * P * A.transpose() + Pq * Q;
}

Eigen::MatrixXd CV::computationalA(float dt) {
    A(0, 2) = dt;
    A(1, 3) = dt;
    return A;
}

Eigen::MatrixXd CV::computationalQ(float dt) {
    Q(0, 0) = std::pow(dt, 3) / 3;
    Q(0, 1) = std::pow(dt, 2) / 2;

    Q(1, 0) = std::pow(dt, 2) / 2;
    Q(1, 1) = dt;

    Q(2, 2) = std::pow(dt, 3) / 3;
    Q(2, 3) = std::pow(dt, 2) / 2;

    Q(3, 2) = std::pow(dt, 2) / 2;
    Q(3, 3) = dt;
    return Q;
}
