/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-20 21:53:49
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-20 21:58:53
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

namespace algorithmBase {
double generateNormalDistributionData(double mean, double stdDev);

std::vector<double> generateNormalDistributionData(double mean, double stdDev, size_t size);
}  // namespace algorithmBase
