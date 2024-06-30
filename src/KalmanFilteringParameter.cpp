/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-11 14:41:36
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-06-30 21:36:43
 * @FilePath: /cplusplus/submodule/algorithmBase/src/KalmanFilteringParameter.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */

#include "KalmanFilteringParameter.h"

void KalmanFilteringParameter::prediction(Eigen::VectorXd& _X, Eigen::MatrixXd& _P, float dt) {
    dynamicModel->prediction(_X, _P, dt);
}
