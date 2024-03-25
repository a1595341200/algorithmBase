/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-20 22:13:55
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-25 16:53:00
 * @FilePath: /cplusplus/submodule/algorithmBase/main.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include <iostream>

#include <fmt/color.h>
#include <fmt/ranges.h>

#include "DataGenerator.h"
#include "DataPlot.h"
#include "Log.h"
#include "dbg.h"

using namespace algorithmBase;

void appTest() {
    dbg(fmt::format(fg(fmt::color::green) | fmt::emphasis::bold, "{}\n",
                    generateNormalDistributionData(1, 1)));
    auto v = generateNormalDistributionData(1, 1, 10);
    dbg(fmt::format(fg(fmt::color::green) | fmt::emphasis::bold, "{}\n", v));
    DataPlot app("DataPlot", 1920, 1080);
    app.addPlot("E_X");
    app.Run();
}

void logTest() {
    LOG_INFO("Hello, World!", 1, 2, 3);
}

void generatingGaussianEquationTest() {
    auto res = generatingGaussianEquation(0, 2);
    DataPlot app("DataPlot", 1920, 1080);
    app.addPlot("Gaussian");
    for (size_t i = 0; i < res.first.size(); ++i) {
        app.addData("Gaussian", "stdDev=2", res.first[i], res.second[i]);
    }
    res = generatingGaussianEquation(0, 0.5);
    for (size_t i = 0; i < res.first.size(); ++i) {
        app.addData("Gaussian", "stdDev=0.5", res.first[i], res.second[i]);
    }
    app.Run();
}

int main(int argc, char const *argv[]) {
    // logTest();
    generatingGaussianEquationTest();
    return 0;
}
