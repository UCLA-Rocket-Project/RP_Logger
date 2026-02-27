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

#define RP_LOGGER_MUST               0
#define RP_LOGGER_WARN               1
#define RP_LOGGER_INFO               2
#define RP_LOGGER_TRACE              3

enum RpLoggerSubsys : uint16_t {
    INIT = (1),
    RADIO_CMD_TRANSPORT = (1 << 1),
    CAN_BUS = (1 << 2),
    MUTEX = (1 << 3),
    SERIAL_CMD_TRANSPORT = (1 << 4),
    CMD_HANDLER = (1 << 5),
    STATE_MACHINE = (1 << 6),
    ADXL = (1 << 7),
    SECONDARY_SENSORS = (1 << 8),
    GLOBAL_STORE = (1 << 9),
    SD_CARD = (1 << 10),
    ADC = (1 << 11),
    RESET_HANDLER = (1 << 12),
    TIME_SYNC = (1 << 13),
    ALL = 65535
};

extern int _rp_logger_log_level;
extern RpLoggerSubsys _rp_logger_subsys_mask;

void rp_logger_init(int level = RP_LOGGER_TRACE, RpLoggerSubsys subsys = RpLoggerSubsys::ALL);

void rp_logger_log_backend(
    int level, RpLoggerSubsys subsys, const char *file, int line, const char *fmt, ...
);

#ifndef RP_LOGGER_COMPILE_LEVEL
    #define RP_LOGGER_COMPILE_LEVEL RP_LOGGER_TRACE
#endif

// 3. The internal macro (Still includes the fast runtime filter for subsystems)
#define RP_LOG_INTERNAL(level, subsys, fmt, ...)                                                   \
    do {                                                                                           \
        if ((level) <= _rp_logger_log_level && ((subsys) & _rp_logger_subsys_mask) != 0) {         \
            rp_logger_log_backend(level, subsys, __FILE__, __LINE__, fmt, ##__VA_ARGS__);          \
        }                                                                                          \
    } while (0)

#if RP_LOGGER_COMPILE_LEVEL >= RP_LOG_COMPILE_MUST
    #define RP_LOG_MUST(subsys, fmt, ...)                                                          \
        RP_LOG_INTERNAL(RP_LOGGER_MUST, subsys, fmt, ##__VA_ARGS__)
#else
    #define RP_LOG_MUST(subsys, fmt, ...)                                                          \
        do {                                                                                       \
        } while (0)
#endif

#if RP_LOGGER_COMPILE_LEVEL >= RP_LOG_COMPILE_WARN
    #define RP_LOG_WARN(subsys, fmt, ...)                                                          \
        RP_LOG_INTERNAL(RP_LOGGER_WARN, subsys, fmt, ##__VA_ARGS__)
#else
    #define RP_LOG_WARN(subsys, fmt, ...)                                                          \
        do {                                                                                       \
        } while (0)
#endif

#if RP_LOGGER_COMPILE_LEVEL >= RP_LOG_COMPILE_INFO
    #define RP_LOG_INFO(subsys, fmt, ...)                                                          \
        RP_LOG_INTERNAL(RP_LOGGER_INFO, subsys, fmt, ##__VA_ARGS__)
#else
    #define RP_LOG_INFO(subsys, fmt, ...)                                                          \
        do {                                                                                       \
        } while (0)
#endif

#if RP_LOGGER_COMPILE_LEVEL >= RP_LOG_COMPILE_TRACE
    #define RP_LOG_TRACE(subsys, fmt, ...)                                                         \
        RP_LOG_INTERNAL(RP_LOGGER_TRACE, subsys, fmt, ##__VA_ARGS__)
#else
    #define RP_LOG_TRACE(subsys, fmt, ...)                                                         \
        do {                                                                                       \
        } while (0)
#endif

#endif