/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-21 21:51:52
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-21 22:33:27
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
class DataPlot : public App {
public:
    using App::App;
    void Update() override;
    void addPlot(const std::string& plotName);
    void addPlot(const std::string& plotName, const std::string& subPlotName);
    void addData(const std::string& plotName, float x, float y);
    void addData(const std::string& plotName, const std::string& subPlotName, float x, float y);

private:
    std::vector<std::string> mPlotNames;
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<ImVec2>>> mData;
    std::unordered_map<std::string, ImVec4> mColors;
    std::unordered_map<std::string, bool> mCheckBoxes;
};
}  // namespace algorithmBase
