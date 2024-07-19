/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-07-19 14:39:38
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-07-19 15:03:51
 * @FilePath: /cplusplus/submodule/algorithmBase/include/RelativeToAbsolute.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#ifndef RELATIVETOABSOLUTE_H
#define RELATIVETOABSOLUTE_H
#include <eigen3/Eigen/Dense>
namespace algorithmBase {
class RelativeToAbsolute {
public:
    static Eigen::Vector3d velocityTransform(const Eigen::Vector3d& relativeVelocity,
                                             const Eigen::Vector3d& relativePosition,
                                             const Eigen::Vector3d& angularVel,
                                             const Eigen::Vector3d& coordinateSystemVelocity);

    static Eigen::Vector3d accelerationTransform(const Eigen::Vector3d& relativeAcceleration,
                                                 const Eigen::Vector3d& relativeVelocity,
                                                 const Eigen::Vector3d& relativePosition,
                                                 const Eigen::Vector3d& angularVel,
                                                 const Eigen::Vector3d& angularAcc,
                                                 const Eigen::Vector3d& coordinateSystemAcc);

private:
};
}  // namespace algorithmBase
#endif /* RELATIVETOABSOLUTE_H */
