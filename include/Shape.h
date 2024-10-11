#ifndef SHAPE_H
#define SHAPE_H
#include <array>

namespace Shape {
using shapeArray = std::array<float, 4>;
/**
 * @brief 平面 ax + by + cz + d = 0
 */
struct Plane {
    explicit Plane(const shapeArray& _coeff) : coeff(_coeff) {
    }
    enum class CoefficientType { A, B, C, D };
    shapeArray coeff;
};
/**
 * @brief 二维曲线 c0 + c1x + c2x^2 + C3x^3= 0
 */
struct Lane {
    explicit Lane(const shapeArray& _coeff) : coeff(_coeff) {
    }
    enum class CoefficientType { C0, C1, C2, C3 };
    shapeArray coeff;
};

};  // namespace Shape

#endif /* SHAPE_H */
