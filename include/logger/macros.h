#ifndef __H_GOMBOC_LOGGER_MACROS__
#define __H_GOMBOC_LOGGER_MACROS__

#include <string.h>
#include "logger/logger.h"

#define GOMBOC_LOGGER_NO_ERROR                     0x2000
#define GOMBOC_LOGGER_RESOURCE_ALLOCATION_PROBLEM  GOMBOC_LOGGER_NO_ERROR + 0x01
#define GOMBOC_LOGGER_HANDLER_LIMIT_ERROR          GOMBOC_LOGGER_NO_ERROR + 0x02
#define GOMBOC_LOGGER_HANDLER_NOT_FOUND            GOMBOC_LOGGER_NO_ERROR + 0x03
#define GOMBOC_LOGGER_LOG_ATTR_ALREADY_INITIALIZED GOMBOC_LOGGER_NO_ERROR + 0x04

/**
 * @def GOMBOC_LOGGER_BUFFER_SIZE
 * @brief Size of the formatted log buffer.
 */
#define GOMBOC_LOGGER_BUFFER_SIZE 1024

/**
 * @def GOMBOC_LOGGER_SET_MAX_HANDLERS
 * @brief Maximum number of global log handlers.
 */
#define GOMBOC_LOGGER_SET_MAX_HANDLERS 100

/**
 * @def TIME_FMT
 * @brief Format for date and time in logs.
 */
#define TIME_FMT "%d-%m-%Y %H:%M:%S"

/**
 * @def PFX_SRC_PATH
 * @brief Prefix for source path in log messages.
 */
#define PFX_SRC_PATH "src"

/**
 * @def LABEL
 * @brief Format for log labels.
 */
#define LABEL " %s:%d - "

/**
 * @def __RLT__
 * @brief Macro to get the relative path of the source file.
 */
#define __RLT__ \
  (strstr(__FILE__, PFX_SRC_PATH) ? strstr(__FILE__, PFX_SRC_PATH) : __FILE__)

/**
 * @def DEBUG
 * @brief Macro to log a debug message.
 *
 * @param message The debug message.
 * @param ... Additional arguments for the debug message.
 */
#define DEBUG(message, ...) \
  gomboc_logger(message, __RLT__, __LINE__, DEBUG, ##__VA_ARGS__);

/**
 * @def INFO
 * @brief Macro to log an informational message.
 *
 * @param message The informational message.
 * @param ... Additional arguments for the informational message.
 */
#define INFO(message, ...) \
  gomboc_logger(message, __RLT__, __LINE__, INFO, ##__VA_ARGS__);

/**
 * @def WARN
 * @brief Macro to log a warning message.
 *
 * @param message The warning message.
 * @param ... Additional arguments for the warning message.
 */
#define WARN(message, ...) \
  gomboc_logger(message, __RLT__, __LINE__, WARN, ##__VA_ARGS__);

/**
 * @def ERROR
 * @brief Macro to log an error message.
 *
 * @param message The error message.
 * @param ... Additional arguments for the error message.
 */
#define ERROR(message, ...) \
  gomboc_logger(message, __RLT__, __LINE__, ERROR, ##__VA_ARGS__);

/**
 * @def CRIT
 * @brief Macro to log a critical message.
 *
 * @param message The critical message.
 * @param ... Additional arguments for the critical message.
 */
#define CRIT(message, ...) \
  gomboc_logger(message, __RLT__, __LINE__, CRIT, ##__VA_ARGS__);

/**
 * @def RAISE_STATUS
 * @brief Macro to change status to bad code and go to clean up.
 *
 * @param __status The status code to set.
 * @param message The critical message.
 * @param ... Additional arguments for the critical message.
 */
#define RAISE_STATUS(__status, message, ...)                       \
  gomboc_logger(message, __RLT__, __LINE__, ERROR, ##__VA_ARGS__); \
  status = __status;                                               \
  goto clean_up

#endif  // __H_GOMBOC_LOGGER_MACROS__
