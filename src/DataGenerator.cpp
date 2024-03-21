/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-21 09:32:26
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-21 09:36:02
 * @FilePath: /cplusplus/submodule/algorithmBase/src/DataGenerator.cpp
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include "DataGenerator.h"
#include <algorithm>
namespace algorithmBase {

double generateNormalDistributionData(double mean, double stdDev) {
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<double> d{mean, stdDev};
    return d(gen);
}

std::vector<double> generateNormalDistributionData(double mean, double stdDev, size_t size) {
    std::vector<double> result(size);
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<double> d{mean, stdDev};
    std::generate(result.begin(), result.end(), [&]() {
        return d(gen);
    });
    return result;
}
}  // namespace algorithmBase
