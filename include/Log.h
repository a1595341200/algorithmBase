/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-23 20:32:04
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-04-12 22:01:28
 * @FilePath: /cplusplus/submodule/algorithmBase/include/Log.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once
#include <iostream>
#include <vector>
#ifndef TAG
#define TAG "null"
#endif

#define LOG_INFO(...) \
    algorithmBase::log(std::cout, __FILE__, " ", __LINE__, " : ", TAG, " ", __VA_ARGS__)
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& items) {
    os << "start vector: \n";
    for (auto& item : items) {
        os << " item : \n" << item << " \n";
    }
    os << std::endl;
    return os;
}

namespace algorithmBase {
template <typename... Args>
void log(std::ostream& os, Args&&... args) {
    (os << ... << args) << std::endl;
}
}  // namespace algorithmBase
