#include "DataPlot.h"

#include "Helpers.h"
namespace algorithmBase {
void DataPlot::Update() {
    for (auto& plotName : mPlotNames) {
        ImGui::SameLine();
        ImGui::Checkbox(plotName.c_str(), &mCheckBoxes[plotName]);
    }
    for (auto& plotName : mPlotNames) {
        if (!mCheckBoxes[plotName]) {
            continue;
        }
        if (ImPlot::BeginPlot(plotName.c_str())) {
            ImPlot::SetupAxes("x", "y");
            ImPlot::SetNextLineStyle(mColors[plotName], 2.0);
            for (auto& [subPlotName, data] : mData[plotName]) {
                ImPlot::PlotLine(subPlotName.c_str(), &data[0].x, &data[0].y, data.size(), 0, 0,
                                 2 * sizeof(float));
            }
            ImPlot::EndPlot();
        }
    }
}

void DataPlot::addPlot(const std::string& plotName) {
    addPlot(plotName, plotName);
}

void DataPlot::addPlot(const std::string& plotName, const std::string& subPlotName) {
    auto it = std::find(mPlotNames.begin(), mPlotNames.end(), plotName);
    if (it == mPlotNames.end()) {
        mPlotNames.push_back(plotName);
    }
    mCheckBoxes[plotName] = true;
    mColors[plotName] = RandomColor();
}
void DataPlot::addData(const std::string& plotName, float x, float y) {
    addData(plotName, plotName, x, y);
}
void DataPlot::addData(const std::string& plotName, const std::string& subPlotName, float x,
                       float y) {
    mData[plotName][subPlotName].push_back(ImVec2(x, y));
}
}  // namespace algorithmBase
