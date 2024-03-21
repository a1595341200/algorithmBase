/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-20 21:53:49
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-21 10:36:17
 * @FilePath: /cplusplus/submodule/algorithmBase/include/DataGenerator.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once
#include <cmath>
#include <iomanip>
#include <random>
#include <vector>
#include <algorithm>

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
}  // namespace algorithmBase
