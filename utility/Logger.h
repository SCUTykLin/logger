//
// Created by YangKai.Lin on 2026/3/12.
//

#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H
#include <string>
#include <fstream>

namespace yazi {
    namespace utility {

    #define debug(format, ...) \
        Logger::instance()->log(Logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__);

    #define info(format, ...) \
        Logger::instance()->log(Logger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__);

    #define warn(format, ...) \
        Logger::instance()->log(Logger::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__);

    #define error(format, ...) \
        Logger::instance()->log(Logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__);

    #define fatal(format, ...) \
        Logger::instance()->log(Logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__);

        class Logger {
        public:
            enum Level {
                DEBUG = 0,
                INFO,
                WARN,
                ERROR,
                FATAL,
                LEVEL_COUNT
            };
            static Logger * instance();
            void open(const std::string & filename);
            void close();
            void log(Level level, const char* file, int line, const char* format, ...);
            void level(Level level) {
                m_level = level;
            }
            void max(int bytes) {
                m_max = bytes;
            }

        private:
            Logger();
            ~Logger();
            void rotate();

            std::string m_filename;
            std::ofstream m_fout;
            Level m_level;
            int m_max;
            int m_len;
            static const char * s_level[LEVEL_COUNT];
            static Logger * m_instance;
        };
    }
}

#endif //LOGGER_LOGGER_H