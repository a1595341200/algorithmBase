/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-22 15:58:59
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-07-07 23:47:16
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
            ImPlot::SetNextLineStyle(mColors[plotName], 2.0);
            for (auto& [subPlotName, data] : mData[plotName]) {
                switch (mPlotType) {
                    case PlotType::LINE:
                        ImPlot::PlotLine(subPlotName.c_str(), &data[0].x, &data[0].y,
                                         static_cast<int>(data.size()), 0, 0, 2 * sizeof(float));
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
}  // namespace algorithmBase
