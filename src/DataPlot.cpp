/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-22 15:58:59
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2025-01-15 16:24:49
 * @FilePath: /cplusplus/submodule/algorithmBase/src/DataPlot.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "DataPlot.h"

#include "Helpers.h"
namespace algorithmBase {
void DataPlot::Update() {
    for (auto& plotName : mPlotNames) {
        ImGui::SameLine();
        ImGui::Checkbox(plotName.c_str(), &mCheckBoxes[plotName]);
    }

    float window_height = ImGui::GetContentRegionAvail().y;
    size_t count{};
    for (auto& plotName : mPlotNames) {
        if (!mCheckBoxes[plotName]) {
            continue;
        }
        count++;
    }

    for (auto& plotName : mPlotNames) {
        if (!mCheckBoxes[plotName]) {
            continue;
        }
        if (ImPlot::BeginPlot(plotName.c_str(),
                              ImVec2(-1, window_height / static_cast<float>(count)))) {
            ImPlot::SetupAxes("x", "y");

            float thickness = mLineThicknesses.count(plotName) ? mLineThicknesses[plotName] : 2.0f;

            ImPlot::SetNextLineStyle(mColors[plotName], thickness);

            for (auto& [subPlotName, data] : mData[plotName]) {
                switch (mPlotType) {
                    case PlotType::LINE:
                        ImPlot::SetNextMarkerStyle(ImPlotMarker_Asterisk);
                        ImPlot::PlotLine(subPlotName.c_str(), &data[0].x, &data[0].y,
                                         static_cast<int>(data.size()), mLineTypes[plotName], 0,
                                         2 * sizeof(float));
                        break;
                    case PlotType::SCATTER:
                        ImPlot::PlotScatter(subPlotName.c_str(), &data[0].x, &data[0].y,
                                            static_cast<int>(data.size()), 0, 0, 2 * sizeof(float));
                        break;
                    default:
                        break;
                };
            }
            ImPlot::EndPlot();
        }
    }
}

void DataPlot::addPlot(const std::string& plotName) {
    addPlot(plotName, plotName);
}

void DataPlot::addPlot(const std::string& plotName, const std::string& subPlotName) {
    (void)subPlotName;
    auto it = std::find(mPlotNames.begin(), mPlotNames.end(), plotName);
    if (it == mPlotNames.end()) {
        mPlotNames.push_back(plotName);
    }
    mCheckBoxes[plotName] = false;
    mColors[plotName] = RandomColor();
    mLineTypes[plotName] = ImPlotLineFlags_::ImPlotLineFlags_None;
    mLineThicknesses[plotName] = 2.0f;
}

void DataPlot::addData(const std::string& plotName, const std::string& subPlotName, float x,
                       float y) {
    auto it = std::find(mPlotNames.begin(), mPlotNames.end(), plotName);
    if (it == mPlotNames.end()) {
        mPlotNames.push_back(plotName);
    }
    if (mCheckBoxes.count(plotName) == 0) {
        mCheckBoxes[plotName] = false;
    }
    if (mColors.count(plotName) == 0) {
        mColors[plotName] = RandomColor();
    }
    mData[plotName][subPlotName].push_back(ImVec2(x, y));
}

void DataPlot::clear(const std::string& plotName) {
    if (mData.count(plotName)) {
        mData.erase(plotName);
    }
}

}  // namespace algorithmBase
