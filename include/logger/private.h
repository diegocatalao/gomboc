#ifndef __H_GOMBOC_LOGGER_PRIVATE__
#define __H_GOMBOC_LOGGER_PRIVATE__

#include "logger/types.h"

/**
 * @brief Writes a log event to stdout or stderr based on severity.
 *
 * @param event The log event to write.
 */
static void write_stdout_handler(LogEvent event);

/**
 * @brief Dummy function for handling log events to a file.
 *
 * @param event The log event to handle.
 */
static void write_file_handler(LogEvent event);

/**
 * @brief Call the thread-handler function with this wrapper.
 * 
 * @param vargs The `LogEventHandler` will be injected here.
 */
static void* pthread_handler_fn(void* vargs);

#endif  // __H_GOMBOC_LOGGER_PRIVATE__
