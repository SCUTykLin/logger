//
// Created by YangKai.Lin on 2026/3/12.
//

#include "Logger.h"
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <stdexcept>
#include <iostream>
#include <filesystem>

using namespace yazi::utility;

namespace fs = std::filesystem;

bool is_file_exist(const std::string& file_path) {
    // 核心函数：判断路径是否存在，且是普通文件（非目录）
    return fs::exists(file_path) && fs::is_regular_file(file_path);
}

const char * Logger::s_level[LEVEL_COUNT] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

Logger * Logger::m_instance = nullptr;

Logger::Logger() : m_level(DEBUG), m_max(0), m_len(0){

}

Logger::~Logger() {
    close();
}

Logger * Logger::instance() {
    if (m_instance == nullptr) {
        m_instance = new Logger();
    }
    return m_instance;
}

void Logger::open(const std::string &filename) {
    m_filename = filename;
    m_fout.open(filename, std::ios::app);
    if (m_fout.fail()) {
        throw std::logic_error("open file failed: " + filename);
    }
    m_fout.seekp(0, std::ios::end);
    m_len = m_fout.tellp();
}

void Logger::close() {
    m_fout.close();
}

void Logger::log(Level level, const char *file, int line, const char *format, ...) {

    if (m_level > level) {
        return;
    }

    if (m_fout.fail()) {
        throw std::logic_error("open file failed: " + m_filename);
    }
    time_t ticks = time(nullptr);
    struct tm * ptn = localtime(&ticks);
    char timestamp[32];
    memset(timestamp, 0, sizeof(timestamp));
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", ptn);
    const char * ftm = "%s %s %s:%d ";
    int size = snprintf(nullptr, 0, ftm, timestamp, s_level[level], file, line);
    if (size > 0) {
        char * buffer = new char[size + 1];
        memset(buffer, 0, size + 1);
        snprintf(buffer, size + 1, ftm, timestamp, s_level[  level], file, line);
        buffer[size] = '\0';
        m_fout << buffer;
        m_len += size;
        delete buffer;
    }
    va_list arg_ptr;
    va_start(arg_ptr, format);
    size = vsnprintf(nullptr, 0, format, arg_ptr); // 获取可变参数的长度
    va_end(arg_ptr);
    if (size > 0) {
        char * content = new char[size + 1];
        va_start(arg_ptr, format); // 把format后面的可变参数拿过来
        vsnprintf(content, size + 1, format, arg_ptr);
        va_end(arg_ptr);
        m_fout << content;
        m_len += size;
        delete content;
    }
    m_fout << std::endl;
    m_fout.flush();

    if (m_len >= m_max && m_max > 0) {
        rotate();
    }
}

void get_time(char* timestamp, size_t buf_size) {
    time_t ticks = time(nullptr);
    struct tm * ptm = localtime(&ticks);
    memset(timestamp, 0, sizeof(timestamp));
    strftime(timestamp, buf_size, "%Y-%m-%d_%H-%M-%S", ptm);
}

void Logger::rotate() {
    close();
    char timestamp[32];
    get_time(timestamp, 32);
    std::string filename = m_filename + timestamp;
    while (is_file_exist(filename)) {
        get_time(timestamp, 32);
        filename = m_filename + timestamp;
    }
    if (rename(m_filename.c_str(), filename.c_str()) != 0) {
        throw std::logic_error("rename file failed: " + filename);
    }
    open(m_filename);
}


