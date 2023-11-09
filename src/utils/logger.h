#ifndef LOGGER_H_
#define LOGGER_H_

#ifndef SPDLOG_ACTIVE_LEVEL
    #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif//SPDLOG_ACTIVE_LEVEL

#include <spdlog/spdlog.h>

#ifdef LOGGING

    #define TRACE SPDLOG_TRACE
    #define DEBUG SPDLOG_DEBUGd
    #define INFO SPDLOG_INFO
    #define WARNING SPDLOG_WARN
    #define ERROR SPDLOG_ERROR
    #define CRITICAL SPDLOG_CRITICAL
#else
    #define TRACE(...)
    #define DEBUG(...)
    #define INFO(...)
    #define WARNING(...)
    #define ERROR(...)
    #define CRITICAL(...)
#endif

#endif /* !LOGGER_H_ */