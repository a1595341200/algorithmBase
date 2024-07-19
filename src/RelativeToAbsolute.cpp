/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-07-19 14:48:58
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-07-19 15:02:40
 * @FilePath: /cplusplus/submodule/algorithmBase/src/RelativeToAbsolute.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "RelativeToAbsolute.h"
namespace algorithmBase {

Eigen::Vector3d RelativeToAbsolute::velocityTransform(
    const Eigen::Vector3d& relativeVelocity, const Eigen::Vector3d& relativePosition,
    const Eigen::Vector3d& angularVel, const Eigen::Vector3d& coordinateSystemVelocity) {
    return coordinateSystemVelocity + angularVel.cross(relativePosition) + relativeVelocity;
}

Eigen::Vector3d RelativeToAbsolute::accelerationTransform(
    const Eigen::Vector3d& relativeAcceleration, const Eigen::Vector3d& relativeVelocity,
    const Eigen::Vector3d& relativePosition, const Eigen::Vector3d& angularVel,
    const Eigen::Vector3d& angularAcc, const Eigen::Vector3d& coordinateSystemAcc) {
    return coordinateSystemAcc + angularAcc.cross(relativePosition) +
           angularVel.cross(angularVel.cross(relativePosition)) + relativeAcceleration +
           2 * angularVel.cross(relativeVelocity);
}

}  // namespace algorithmBase
