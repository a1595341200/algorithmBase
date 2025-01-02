/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2025-01-02 15:40:08
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2025-01-02 15:40:34
 * @FilePath: /cplusplus/submodule/algorithmBase/include/PolarCoordinates.h
 * @Description: 极坐标与笛卡尔坐标转换
 */

#ifndef POLAR_COORDINATES_H
#define POLAR_COORDINATES_H

#include <cmath>

namespace algorithmBase {

class PolarCoordinates {
public:
    // 构造函数
    PolarCoordinates(double r = 0.0, double theta = 0.0);
    
    // 设置极坐标
    void setPolar(double r, double theta);
    
    // 从笛卡尔坐标设置
    void setCartesian(double x, double y);
    
    // 获取极坐标
    double getR() const;
    double getTheta() const;
    
    // 获取笛卡尔坐标
    double getX() const;
    double getY() const;
    
    // 转换到笛卡尔坐标
    void toCartesian(double& x, double& y) const;
    
    // 从笛卡尔坐标转换
    static PolarCoordinates fromCartesian(double x, double y);

    // 新增：静态方法直接转换极坐标到笛卡尔坐标
    static void polarToCartesian(double r, double theta, double& x, double& y);

private:
    double r_{};     // 极径（到原点的距离）
    double theta_{}; // 极角（与x轴的夹角，弧度制）
};

} // namespace algorithmBase

#endif // POLAR_COORDINATES_H
