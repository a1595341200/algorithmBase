/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-04-12 13:24:14
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-04-12 17:20:38
 * @FilePath: /cplusplus/submodule/algorithmBase/src/CovarianceIntersection.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "CovarianceIntersection.h"
#include <iostream>

void CovarianceIntersection::update(const Eigen::VectorXf& z) {
    double min(INT_MAX);
    double w{1};
    Eigen::MatrixXf PG;
    Eigen::MatrixXf tmpP;
    for (double tmpW = 0.1; tmpW <= 0.99; tmpW += 0.1) {
        tmpP = tmpW * P.inverse() + (1.0 - tmpW) * R.inverse();
        tmpP = tmpP.inverse();
        if (tmpP.determinant() < min) {
            min = tmpP.determinant();
            PG = tmpP;
            w = tmpW;
        }
        // std::cout << tmpP <<std::endl;
        // std::cout << w <<std::endl;
    }
    X = PG * (w * P.inverse() * X + (1 - w) * R.inverse() * z);
    std::cout << PG <<std::endl;
    std::cout << "w : " << w <<std::endl;
    P = PG;
}


CovarianceIntersection::CovarianceIntersection() {
    A.resize(4, 4);
    A << 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0;
    X.resize(4);
    X << 0, 0, 1, 1;
    Q.resize(4, 4);
    Q << 0.01, 0, 0, 0, 0, 0.01, 0, 0, 0, 0, 0.01, 0, 0, 0, 0, 0.01;
    R.resize(4, 4);
    R << 0.1, 0, 0, 0, 0, 0.1, 0, 0, 0, 0, 0.1, 0, 0, 0, 0, 0.1;
    P.resize(4, 4);
    P << 0.05, 0, 0, 0, 0, 0.05, 0, 0, 0, 0, 0.05, 0, 0, 0, 0, 0.05;
    H.resize(4);
    H << 1, 1, 1, 1;
}

void CovarianceIntersection::prediction(double dt) {
    A(0, 2) = dt;
    A(1, 3) = dt;
    X = A * X;
    P = A * P * A.transpose() + Q;
}
