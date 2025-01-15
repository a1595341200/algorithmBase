/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-22 15:58:59
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2025-01-15 18:24:18
 * @FilePath: /cplusplus/submodule/algorithmBase/src/DataPlot.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "DataPlot.h"

#include <implot.h>

#include "Helpers.h"
namespace algorithmBase {

void DataPlot::plotLine() {
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

void DataPlot::plotBev() {
    if (ImPlot::BeginPlot("##CustomRend", ImVec2(-1, -1), ImPlotFlags_Equal)) {
        plotSelf();
        for (auto& bev : mBevData) {
            plotObj(bev.x, bev.y, bev.half_length, bev.half_width, bev.angle, bev.nearest_side,
                    ImVec4(0, 0, 0, 255), "bev");
        }
        ImPlot::EndPlot();
    }
}

void DataPlot::addBevData(float x, float y, float half_length, float half_width, float angle,
                          uint8_t nearest_side) {
    mBevData.push_back(BevObject{x, y, half_length, half_width, angle, nearest_side});
}

void DataPlot::Update() {
    ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
    if (ImGui::TreeNode("plotLine")) {
        plotLine();
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("plotBev")) {
        plotBev();
        ImGui::TreePop();
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

static std::vector<ImVec2> rotatedRect(float x, float y, float half_length, float half_width,
                                       float angle, uint8_t nearest_side) {
    float c = cos(angle);
    float s = sin(angle);
    if (nearest_side == 0) {
    } else if (nearest_side == 1) {
        x = x + half_length * c;
        y = y + half_length * s;
    } else if (nearest_side == 2) {
        x = x - half_length * c;
        y = y - half_length * s;
    } else if (nearest_side == 3) {
        x = x - half_width * s;
        y = y + half_width * c;
    } else if (nearest_side == 4) {
        x = x + half_width * s;
        y = y - half_width * c;
    }
    float r1x = -half_length * c - half_width * s;
    float r1y = -half_length * s + half_width * c;
    float r2x = half_length * c - half_width * s;
    float r2y = half_length * s + half_width * c;
    return {ImVec2(x + r1x, y + r1y), ImVec2(x + r2x, y + r2y), ImVec2(x - r1x, y - r1y),
            ImVec2(x - r2x, y - r2y), ImVec2(x + r1x, y + r1y)};
}

void DataPlot::plotObj(float x, float y, float half_length, float half_width, float angle,
                       uint8_t nearest_side, const ImVec4& col, const std::string& id) {
    auto points = rotatedRect(x, y, half_length, half_width, angle, nearest_side);
    ImPlot::SetNextLineStyle(col, 4.0);
    ImPlot::PlotLine("", &points[0].x, &points[0].y, 5, 0, 0, 2 * sizeof(float));
    ImPlot::SetNextLineStyle(ImVec4(0, 0, 0, 255), 8.0);
    ImPlot::SetNextLineStyle(col, 4.0);
    ImPlot::PlotText(id.c_str(), x, y);
}

void DataPlot::plotSelf() {
    auto points = rotatedRect(-1.15f, 0, 2.425f, 0.96f, 0, 1);
    ImPlot::SetNextLineStyle(ImVec4(0, 0, 0, 255), 4.0f);
    ImPlot::PlotLine("", &points[0].x, &points[0].y, 5, 0, 0, 2 * sizeof(float));
}

}  // namespace algorithmBase
