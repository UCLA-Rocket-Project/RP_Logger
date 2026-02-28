# Rocket Project Logger
Log with different levels and subsystems

## Usage instructions

### Setup
1. Define, as `RP_LOGGER_COMPILE_LEVEL`, the minmum log level that you want to build for. This could also be a build flag
```C++
#define RP_LOGGER_LEVEL RP_LOGGER_TRACE
```

2. Instantiate the logger, giving the maximum log level that you want to view and the subsystems that you want to see logs for
```C++
rp_logger_init(RP_LOGGER_TRACE, RpLoggerSubsys::ALL);
```

3. Log away, like in the following examples
```C++
RP_LOG_TRACE(RADIO_CMD_TRANSPORT, "Got a command %x", command_code);

RP_LOG_INFO(ADC, "Read %u", adc_reading);

RP_LOG_WARN(CAN_BUS, "Could not send message");

RP_LOG_MUST(RESET_HANDLER, "Resetting all the boards");
``` 

### Details
This uses esp-idf's `puts` function which is thread free, so your logs appear nice and orderly