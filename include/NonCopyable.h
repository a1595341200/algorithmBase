/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-04-10 17:49:42
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-04-10 17:53:17
 * @FilePath: /cplusplus/submodule/algorithmBase/include/NonCopyable.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once

class NonCopyable {
public:
    NonCopyable() = default;

    virtual ~NonCopyable() = default;

private:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable& operator=(NonCopyable&&) = delete;
};
