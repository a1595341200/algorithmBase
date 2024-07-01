/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-04-12 13:24:14
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-07-01 22:30:49
 * @FilePath: /cplusplus/submodule/algorithmBase/src/CovarianceIntersection.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "CovarianceIntersection.h"
#include <limits>
#include <iostream>

void CovarianceIntersection::update(const Eigen::VectorXf& z) {
    double min(std::numeric_limits<double>::max());
    Eigen::MatrixXf PG;
    Eigen::MatrixXf tmpP;
    for (double tmpW = 0; tmpW <= 1; tmpW += 0.1) {
        tmpP = tmpW * P.inverse() + (1.0 - tmpW) * R.inverse();
        tmpP = tmpP.inverse();
        if (tmpP.determinant() < min) {
            min = tmpP.determinant();
            PG = tmpP;
            w = tmpW;
        }
        // std::cout << tmpP.determinant() <<std::endl;
        // std::cout << w <<std::endl;
    }
    X = PG * (w * P.inverse() * X + (1 - w) * R.inverse() * z);
    std::cout << PG << std::endl;
    std::cout << "w : " << w << std::endl;
    P = PG;
}

CovarianceIntersection::CovarianceIntersection() {
    A.resize(2, 2);
    A << 1.0f, 0, 0, 1.0f;
    X.resize(2);
    X << 1.0f, 1.0f;
    Q.resize(2, 2);
    Q << 0.01f, 0.0001f, 0.0002f, 0.01f;
    R.resize(2, 2);
    R << 0.1f, -0.1f, -0.12f, 0.1f;
    P.resize(2, 2);
    P << 0.05f, 0.0001f, 0.0002f, 0.05f;
    H.resize(2);
    H << 1.0f, 1.0f;
}

void CovarianceIntersection::prediction(double dt) {
    (void)dt;
    X = A * X;
    P = A * P * A.transpose() + Q;
}
