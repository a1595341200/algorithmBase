/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-20 21:53:49
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-07-08 00:04:17
 * @FilePath: /cplusplus/submodule/algorithmBase/include/DataGenerator.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once
#include <algorithm>
#include <armadillo>
#include <cmath>
#include <iomanip>
#include <random>
#include <vector>

#include <eigen3/Eigen/Dense>

namespace algorithmBase {
template <typename T = double>
T generateNormalDistributionData(double mean, double stdDev) {
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<T> d{mean, stdDev};
    return d(gen);
}

template <typename T = double>
std::vector<T> generateNormalDistributionData(double mean, double stdDev, size_t size) {
    std::vector<T> result(size);
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<T> d{mean, stdDev};
    std::generate(result.begin(), result.end(), [&]() {
        return d(gen);
    });
    return result;
}

template <typename T = double>
std::pair<std::vector<T>, std::vector<T>> generatingGaussianEquation(double mean, double stdDev) {
    std::pair<std::vector<T>, std::vector<T>> res;
    static auto gaussian = [=](double x) {
        return std::exp(-0.5 * std::pow((x - mean) / stdDev, 2)) / (std::sqrt(2 * M_PI) * stdDev);
    };
    for (double x = mean - 5.0; x <= mean + 5.0; x += 0.1) {
        double y = gaussian(x);
        res.first.push_back(x);
        res.second.push_back(y);
    }

    return res;
}

template <typename T>
inline void create_gaussian_particles(std::vector<T>& particles, const arma::vec& mean,
                                      const arma::mat& cov, size_t n) {
    // 生成多维正态分布的随机样本
    arma::mat samples = arma::randn(n, mean.size());   // 标准正态分布
    arma::mat result = arma::chol(cov) * samples.t();  // 转换为特定均值和协方差
    particles.clear();
    particles.resize(n, T());
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < mean.size(); ++j) {
            particles[i][j] = result.unsafe_col(i)(j);
        }
    }
}

}  // namespace algorithmBase
