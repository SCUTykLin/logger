//
// Created by YangKai.Lin on 2026/3/12.
//

#include "Logger.h"
#include "Logger.h"

using namespace yazi::utility;

const char * Logger::s_level[5] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

Logger * Logger::m_instance = nullptr;

Logger::Logger() {

}

Logger::~Logger() {

}

Logger * Logger::instance() {
    if (m_instance == nullptr) {
        m_instance = new Logger();
    }
    return m_instance;
}

void Logger::open(const std::string &filename) {

}


