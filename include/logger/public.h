#ifndef __H_GOMBOC_PUBLIC__
#define __H_GOMBOC_PUBLIC__

#include "logger/types.h"

/**
 * @brief Logs a msg with the specified severity.
 *
 * @param msg The log msg.
 * @param rlt The relative path of the source file.
 * @param line The line number in the source file.
 * @param severity The severity of the log msg.
 * @param ... Additional arguments for the log msg.
 */
void gomboc_logger(char* msg, char* rlt, int line, LogSeverity severity, ...);

/**
 * @brief Adds a log handler to the logger.
 *
 * @param name The name of the log handler.
 * @param fnp The function pointer to the log handler.
 * @return int Status code indicating the result of the operation.
 * @retval GOMBOC_LOGGER_NO_ERROR Success.
 * @retval GOMBOC_LOGGER_HANDLER_LIMIT_ERROR Maximum number of handlers exceeded.
 */
extern int gomboc_add_log_handler(char* name, gomboc_log_handler fnp);

/**
 * @brief Removes a log handler from the logger.
 *
 * @param name The name of the log handler to remove.
 * @return int Status code indicating the result of the operation.
 * @retval GOMBOC_LOGGER_NO_ERROR Success.
 * @retval GOMBOC_LOGGER_HANDLER_NOT_FOUND Handler not found.
 */
extern int gomboc_remove_log_handler(char* name);

/**
 * @brief Sets the log attribute for the logger.
 *
 * @param attr The log attribute to set.
 * @return int Status code indicating the result of the operation.
 * @retval GOMBOC_LOGGER_NO_ERROR Success.
 * @retval GOMBOC_LOGGER_RESOURCE_ALLOCATION_PROBLEM Resource allocation problem.
 */
extern int gomboc_set_log_attribute(LogAttribute attr);

#endif  //__H_GOMBOC_PUBLIC__
