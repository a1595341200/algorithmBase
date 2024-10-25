/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git
 * config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-11-10 17:17:18
 * @LastEditors: yao.xie 1595341200@qq.com
 * git config user.email & please set dead value or install git & please set dead value or install
 * git
 * @LastEditTime: 2024-11-14 14:46:16
 * @FilePath: /cplusplus/submodule/algorithmBase/include/Singleton.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

template <typename T>
struct Singleton {
    static T* getInstance() {
        // C++ 11 保证函数内部static变量的初始化是线程安全的
        static T ins;
        return &ins;
    }

private:
    // 禁止外部构造
    Singleton() {
    }
    // 显示声明禁止外部析构, 同时移动构造和移动赋值操作也是禁止的
    ~Singleton() {
    }
    // 防止拷贝构造和赋值操作, 同时移动构造和移动赋值操作也是禁止的
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};
