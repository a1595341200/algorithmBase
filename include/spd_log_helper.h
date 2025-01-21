/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-08-27 16:54:35
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2025-01-21 13:31:34
 * @FilePath: /cplusplus/submodule/algorithmBase/include/spd_log_helper.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once
#include <string>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#ifdef __QNX__
#define SPDLOG_HEADER_ONLY
#endif

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

void InitLog(const std::string& argv0, const std::string& log_path = "./build/log");
void ReleaseLog();

class LogManager {
public:
    static LogManager& instance() {
        static LogManager obj;
        return obj;
    }
    bool Init(const std::string& argv0, const std::string& file_name);

    spdlog::logger* logger() {
        return logger_.get();
    }

private:
    LogManager(/* args */) = default;
    ~LogManager() = default;
    LogManager(const LogManager&) = delete;
    LogManager& operator=(const LogManager&) = delete;

    spdlog::sink_ptr cosole_sink_{nullptr};
    spdlog::sink_ptr file_sink_{nullptr};
    std::shared_ptr<spdlog::logger> logger_{nullptr};
    std::atomic_bool is_init_{false};
};

#define LOG_TRACE(...) SPDLOG_LOGGER_TRACE(spdlog::default_logger(), __VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(spdlog::default_logger(), __VA_ARGS__)
#define LOG_INFO(...) SPDLOG_LOGGER_INFO(spdlog::default_logger(), __VA_ARGS__)
#define LOG_WARN(...) SPDLOG_LOGGER_WARN(spdlog::default_logger(), __VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_LOGGER_ERROR(spdlog::default_logger(), __VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(spdlog::default_logger(), __VA_ARGS__)

#define OBJ_CHECK(status, ...)                                          \
    do {                                                                \
        auto ret = (status);                                            \
        if (!ret) {                                                     \
            SPDLOG_LOGGER_ERROR(spdlog::default_logger(), __VA_ARGS__); \
            EPCV_THROW(__VA_ARGS__);                                    \
        }                                                               \
    } while (0)
