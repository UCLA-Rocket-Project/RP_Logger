#include "RP_Logger.h"
#include "freertos/idf_additions.h"
#include <cstdio>

int _rp_logger_log_level = RP_LOGGER_TRACE;
int _rp_logger_subsys_mask = RpLoggerSubsys::ALL;

void rp_logger_init(int level, int subsystems_to_log) {
    if (!Serial) {
        Serial.begin(RP_LOGGER_SERIAL_BAUDRATE);
    }

    _rp_logger_log_level = level;
    _rp_logger_subsys_mask = subsystems_to_log;
}

static inline const char *path_to_filename(const char *path) {
    const char *slash = strrchr(path, '/');
    if (!slash)
        slash = strrchr(path, '\\');
    return slash ? slash + 1 : path;
}

static inline const char *rp_logger_level_to_string(int level) {
    switch (level) {
    case RP_LOGGER_MUST:
        return "MUST";
    case RP_LOGGER_WARN:
        return "WARN";
    case RP_LOGGER_INFO:
        return "INFO";
    case RP_LOGGER_TRACE:
        return "TRACE";
    default:
        return "UNKNOWN";
    }
}

const char *rp_logger_subsys_to_string(RpLoggerSubsys subsys) {
    switch (subsys) {
    case INIT:
        return "INIT";
    case RADIO_CMD_TRANSPORT:
        return "RADIO";
    case CAN_BUS:
        return "CAN_BUS";
    case MUTEX:
        return "MUTEX";
    case SERIAL_CMD_TRANSPORT:
        return "SERIAL";
    case CMD_HANDLER:
        return "CMD_H";
    case STATE_MACHINE:
        return "SM";
    case ADXL:
        return "ADXL";
    case SECONDARY_SENSORS:
        return "SEC";
    case GLOBAL_STORE:
        return "GLB_STR";
    case SD_CARD:
        return "SD";
    case ADC:
        return "ADC";
    case RESET_HANDLER:
        return "RST";
    case TIME_SYNC:
        return "SYNC";
    case ALL:
        return "ALL";
    default:
        return "UNKNOWN";
    }
}

void rp_logger_log_backend(
    int level, RpLoggerSubsys subsys, const char *file, int line, const char *fmt, ...
) {
    if (level > _rp_logger_log_level || (subsys & _rp_logger_subsys_mask) == 0) {
        return;
    }

    char full_buffer[RP_LOGGER_PRINT_BUFFER_SIZE]{};

    int prefix_len = snprintf(
        full_buffer,
        sizeof(full_buffer),
        "[%lu | %-8.8s | %-5s | %-5.5s | %6.6s:%-4d]: ",
        millis(),
        pcTaskGetName(NULL),
        rp_logger_level_to_string(level),
        rp_logger_subsys_to_string(subsys),
        path_to_filename(file),
        line
    );

    if (prefix_len < sizeof(full_buffer)) {
        va_list args;
        va_start(args, fmt);
        vsnprintf(full_buffer + prefix_len, sizeof(full_buffer) - prefix_len, fmt, args);
        va_end(args);
    }

    puts(full_buffer);
}