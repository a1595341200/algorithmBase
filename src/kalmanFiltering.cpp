/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-11 17:25:32
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-22 00:12:34
 * @FilePath: /cplusplus/submodule/algorithmBase/src/kalmanFiltering.cpp
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include "kalmanFiltering.h"

#include <iostream>

#include <eigen3/Eigen/Dense>

KalmanFilter::KalmanFilter() {  // 动力学模型转移矩阵
  //    A << 1, 0, dt, 0,
  //        0, 1, 0, dt,
  //        0, 0, 1, 0,
  //        0, 0, 0, 1;
  // 动力学模型转移矩阵
  A.resize(4, 4);
  Q.resize(4, 4);
  R.resize(4, 4);
  P.resize(4, 4);
  H.resize(4, 4);
  x.resize(4);
  //clang-format off
  A << 1, 0, 0, 0,
       0, 1, 0, 0,
       0, 0, 0, 0,
       0, 0, 0, 0;

  x << 0,
       0,
       0,
       0;

  Q << 0.01, 0, 0, 0,
       0, 0.01, 0, 0,
       0, 0, 0.01, 0,
       0, 0, 0, 0.01;

  R << 0.01, 0, 0, 0,
       0, 0.01, 0, 0,
       0, 0, 0.01, 0,
       0, 0, 0, 0.01;

  P << 1, 0, 0, 0,
       0, 1, 0, 0,
       0, 0, 1, 0,
       0, 0, 0, 1;

  H << 1, 0, 0, 0,
       0, 1, 0, 0,
       0, 0, 1, 0,
       0, 0, 0, 1;
  //clang-format on
}

void KalmanFilter::predict(double dt) {
  // 预测步骤
  A(0, 2) = dt;
  A(1, 3) = dt;
  x_hat = A * x;
  P_hat = A * P * A.transpose() + Q;
}

void KalmanFilter::update(Eigen::VectorXf measurement) {
  // 更新步骤
  // 测量残差协方差
  auto S = (H * P_hat * H.transpose() + R).inverse();
//   std::cout << "S = \n" << S << std::endl;
  K = P_hat * H.transpose() * S;
  std::cout << "K = \n" << K << std::endl;
  // 测量残差
  auto z = measurement - H * x_hat;
  x = x_hat + K * z;
  P = (Eigen::MatrixXf::Identity(A.rows(), A.cols()) - K * H) * P_hat;
  std::cout << "P \n" << P << std::endl;
}

Eigen::VectorXf KalmanFilter::getState() const {
  return x;
}
