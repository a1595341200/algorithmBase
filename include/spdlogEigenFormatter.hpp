/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2025-01-16 17:17:10
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2025-01-16 17:21:37
 * @FilePath: /cplusplus/submodule/algorithmBase/include/spdlogEigenFormatter.hpp
 * @Description:
 *
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved.
 */
#pragma once
#include <eigen3/Eigen/Dense>
#include <fmt/format.h>

// 为 Eigen 类型添加格式化支持
template <typename Scalar, int Rows, int Cols>
struct fmt::formatter<Eigen::Matrix<Scalar, Rows, Cols>> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Eigen::Matrix<Scalar, Rows, Cols>& matrix, FormatContext& ctx)
        -> decltype(ctx.out()) {
        std::stringstream ss;
        ss << matrix;
        return fmt::format_to(ctx.out(), "{}", ss.str());
    }
};

// 为四元数添加格式化支持
template <typename Scalar>
struct fmt::formatter<Eigen::Quaternion<Scalar>> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Eigen::Quaternion<Scalar>& q, FormatContext& ctx) -> decltype(ctx.out()) {
        return fmt::format_to(ctx.out(), "[w={}, x={}, y={}, z={}]", q.w(), q.x(), q.y(), q.z());
    }
};
