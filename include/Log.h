/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-23 20:32:04
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-28 22:20:59
 * @FilePath: /cplusplus/submodule/algorithmBase/include/Log.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once
#include <iostream>
#define LOG_INFO(...) algorithmBase::log(std::cout, __FILE__, __LINE__, " : ", __VA_ARGS__)
namespace algorithmBase {
template <typename... Args>
void log(std::ostream& os, Args&&... args) {
    (os << ... << args) << std::endl;
}

}  // namespace algorithmBase
