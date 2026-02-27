#ifndef RP_LOGGER_H
#define RP_LOGGER_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <stdint.h>

// every log should show:
// timestamp, calling thread, level, trace subsystem, filename, filenumber

#define RP_LOGGER_THREAD_NAME_MAXLEN 12
#define RP_LOGGER_SERIAL_BAUDRATE    115200
#define RP_LOGGER_PRINT_BUFFER_SIZE  512

enum RpLoggerLevel { MUST = 0, WARN = 1, INFO = 2, TRACE = 3 };
enum RpLoggerSubsys : uint16_t {
    INIT = (1),
    RADIO_CMD_TRANSPORT = (1 << 1),
    CAN_BUS = (1 << 2),
    MUTEX = (1 << 3),
    SERIAL_CMD_TRANSPORT = (1 << 4),
    ALL = 65535
};

extern RpLoggerLevel _rp_logger_log_level;
extern RpLoggerSubsys _rp_logger_subsys_mask;

void rp_logger_init(
    RpLoggerLevel level = RpLoggerLevel::TRACE, RpLoggerSubsys subsys = RpLoggerSubsys::ALL
);

void rp_logger_log_backend(
    RpLoggerLevel level, RpLoggerSubsys subsys, const char *file, int line, const char *fmt, ...
);

#define RP_LOG_INTERNAL(level, subsys, fmt, ...)                                                   \
    do {                                                                                           \
        rp_logger_log_backend(level, subsys, __FILE__, __LINE__, fmt, ##__VA_ARGS__);              \
    } while (0)

#define RP_LOG_MUST(subsys, fmt, ...)                                                              \
    RP_LOG_INTERNAL(RpLoggerLevel::MUST, subsys, fmt, ##__VA_ARGS__)

#define RP_LOG_WARN(subsys, fmt, ...)                                                              \
    RP_LOG_INTERNAL(RpLoggerLevel::WARN, subsys, fmt, ##__VA_ARGS__)

#define RP_LOG_INFO(subsys, fmt, ...)                                                              \
    RP_LOG_INTERNAL(RpLoggerLevel::INFO, subsys, fmt, ##__VA_ARGS__)

#define RP_LOG_TRACE(subsys, fmt, ...)                                                             \
    RP_LOG_INTERNAL(RpLoggerLevel::TRACE, subsys, fmt, ##__VA_ARGS__)

#endif