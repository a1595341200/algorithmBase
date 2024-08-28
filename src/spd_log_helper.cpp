/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-08-27 16:54:38
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-08-27 17:13:10
 * @FilePath: /cplusplus/src/spdlog/src/spd_log_helper.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "spd_log_helper.h"

#include <iostream>
#include <memory>
#include <sstream>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "file.h"

void InitLog(const std::string& argv0, const std::string& log_path) {
    if (argv0.empty() || log_path.empty()) {
        throw std::runtime_error("argv0 or file_name or log_path is empty");
    }

    std::string root_path = log_path;
    std::string full_log_path = root_path + "/";
    EnsureDirectory(full_log_path);
    std::stringstream ss;
    ss << full_log_path  << argv0 << ".log";
    LogManager::instance().Init(argv0, ss.str());
}

void ReleaseLog() {
}

bool LogManager::Init(const std::string& argv0, const std::string& file_name) {
    std::cout << "file_name:" << file_name << std::endl;
    // [2014-10-31 23:46:59.678] [mylogger] [info] Some message
    std::stringstream ss;
    ss << "[%D %H:%M:%S.%e][" << argv0 << "] [%^%L%$] %s:%# [%!] %v";

    constexpr auto max_size = 1024 * 1024 * 20;
    constexpr auto max_files = 10;
    file_sink_ =
        std::make_shared<spdlog::sinks::rotating_file_sink_mt>(file_name, max_size, max_files);
    file_sink_->set_level(spdlog::level::trace);
    file_sink_->set_pattern(ss.str());
#if (SPDLOG_ACTIVE_LEVEL < SPDLOG_LEVEL_WARN) || (!defined(__QNX__))
    cosole_sink_ = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    cosole_sink_->set_level((spdlog::level::level_enum)SPDLOG_ACTIVE_LEVEL);
    cosole_sink_->set_pattern(ss.str());

    // auto it = std::initializer_list<spdlog::sink_ptr>({cosole_sink_, file_sink_});
    logger_ = std::make_shared<spdlog::logger>(
        argv0, std::initializer_list<spdlog::sink_ptr>({cosole_sink_, file_sink_}));
    logger_->set_level((spdlog::level::level_enum)SPDLOG_ACTIVE_LEVEL);
#else
    logger_ = std::make_shared<spdlog::logger>(argv0, file_sink_);
    logger_->set_level(spdlog::level::warn);
#endif
    logger_->flush_on(spdlog::level::err);
    spdlog::set_default_logger(logger_);
    return true;
}
