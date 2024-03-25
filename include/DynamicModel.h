/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-11 15:04:28
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-20 23:25:22
 * @FilePath: /cplusplus/src/algorithm/ukf/include/DynamicModel.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#ifndef DYNAMICMODEL_H
#define DYNAMICMODEL_H
#include <eigen3/Eigen/Dense>

class DynamicModelBase {
public:
    virtual void prediction(Eigen::VectorXd& X, float dt) = 0;
    virtual ~DynamicModelBase() = default;
};

class CTRA : public DynamicModelBase {
public:
    void prediction(Eigen::VectorXd& X, float dt) override;
};

#endif /* DYNAMICMODEL_H */
