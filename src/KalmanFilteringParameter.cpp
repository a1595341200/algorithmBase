/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-11 14:41:36
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-11 15:32:29
 * @FilePath: /cplusplus/src/algorithm/ukf/src/KalmanFilteringParameter.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */

#include "KalmanFilteringParameter.h"

void KalmanFilteringParameter::prediction(Eigen::VectorXd& X, float dt) {
    dynamicModel->prediction(X, dt);
}
