/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-21 21:51:52
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2025-01-15 16:17:17
 * @FilePath: /cplusplus/submodule/algorithmBase/include/DataPlot.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once
#include <App.h>

#include <unordered_map>
#include <vector>

namespace algorithmBase {
enum class PlotType {
    LINE,
    SCATTER,
};

class DataPlot : public App {
public:
    using App::App;
    void Update() override;
    void setPlotType(PlotType plotType) {
        mPlotType = plotType;
    }
    void addPlot(const std::string& plotName);
    void addPlot(const std::string& plotName, const std::string& subPlotName);
    
    // 添加设置线段类型和粗细的方法
    void setLineType(const std::string& plotName, ImPlotLineFlags type) {
        mLineTypes[plotName] = type;
    }
    void setLineThickness(const std::string& plotName, float thickness) {
        mLineThicknesses[plotName] = thickness;
    }

    template <typename T, typename U>
    void addData(const std::string& plotName, T x, U y) {
        addData(plotName, plotName, static_cast<double>(x), static_cast<double>(y));
    }
    void addData(const std::string& plotName, const std::string& subPlotName, float x, float y);

    void clear(const std::string& plotName);

private:
    std::vector<std::string> mPlotNames{};
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<ImVec2>>> mData{};
    std::unordered_map<std::string, ImVec4> mColors{};
    std::unordered_map<std::string, bool> mCheckBoxes{};
    
    // 添加存储线段类型和粗细的成员变量
    std::unordered_map<std::string, ImPlotLineFlags> mLineTypes{};
    std::unordered_map<std::string, float> mLineThicknesses{};
    
    PlotType mPlotType{PlotType::LINE};
};
}  // namespace algorithmBase
