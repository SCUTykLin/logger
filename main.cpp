/*
 * 功能：
 * 日志存储：文本文件
 * 日志内容：时间，级别，文件，行号，内容
 * 日志级别：debug < info < warn < error < fatal
 * 日志翻滚：设置日志的大小
 */

#include <iostream>
#include "utility/Logger.h"

using namespace yazi::utility;

int main() {
    Logger::instance()->open("./test.log");
    Logger::instance()->level(Logger::INFO);
    Logger::instance()->max(2048);
    for (int i = 0; i < 1000; i++)
        error("name is %s, age is %d", "opake", 289);
    return 0;
}