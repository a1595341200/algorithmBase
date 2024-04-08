/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-04-08 10:42:35
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-04-08 10:46:43
 * @FilePath: /cplusplus/src/cplusplus/DesignMode/PrototypePattern/include/Prototype.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once
#include <memory>
template <typename T>
class Prototype {
public:
    virtual std::unique_ptr<T> clone() = 0;
};
