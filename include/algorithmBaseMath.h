/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-04-18 16:02:59
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-04-18 16:16:23
 * @FilePath: /cplusplus/submodule/algorithmBase/include/algorithmBaseMath.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include <numeric>
#include <vector>

namespace algorithmBase {

template <typename T>
auto mean(const T& t) -> typename T::value_type {
    return std::accumulate(t.begin(), t.end(), 0.0) / static_cast<typename T::value_type>(t.size());
}

template <typename T, size_t N>
T mean(T (&arr)[N]) {
    return std::accumulate(std::begin(arr), std::end(arr), 0.0) / static_cast<T>(N);
}

}  // namespace algorithmBase
