/**
 * @file logger.c
 * @brief Implementation of logging functions for the Gomboc Logger.
 *
 * This file contains the implementation of logging functions, including
 * the core logging mechanism, handler management, and log attributes.
 */

#include "logger/logger.h"
#include "conate/conate.h"

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

static LogAttribute* __log_attribute = NULL;

static const char* str_log_severities[] = {
    "VERBOSE", "INFO", "DEBUG", "WARN", "ERROR", "CRIT",
};

static LogHandler __log_handlers[GOMBOC_LOGGER_SET_MAX_HANDLERS + 1] = {
    {
        .name = "write_stdout_handler",
        .fnp = (gomboc_log_handler*)write_stdout_handler,
    },
    {
        .name = "write_file_handler",
        .fnp = (gomboc_log_handler*)write_file_handler,
    },
    {.name = NULL, .fnp = NULL},  // SENTINEL
};

static pthread_mutex_t write_file_handler_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t write_stdout_handler_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Writes a log event to stdout or stderr based on severity.
 *
 * This function writes a log message to stdout for lower severity levels and
 * to stderr for higher severity levels, including the date, time, severity,
 * source file, line number, and the log message.
 *
 * @param event The log event to write.
 */
static void write_stdout_handler(LogEvent event) {
  pthread_mutex_lock(&write_stdout_handler_mutex);

  char obuff[64];
  const char* msg = "[%s][%s] - %s:%d - %s\n";
  FILE* outf = (event.severity > DEBUG ? stderr : stdout);
  const char* cseverity = str_log_severities[event.severity];

  Timestamp tms;

  if (current_timestamp(&tms, true) != GOMBOC_CONATE_NO_ERROR) {
    tms.timestamp = 0;
    tms.milliseconds = 0;
  }

  if (date_time_mil_str(&tms, obuff, sizeof(obuff), TIME_FMT) !=
      GOMBOC_CONATE_NO_ERROR) {
    snprintf(obuff, sizeof(obuff), "UNKNOWN_TIME");
  }

  fprintf(outf, msg, obuff, cseverity, event.rlt, event.line, event.msg);
  fflush(outf);

  pthread_mutex_unlock(&write_stdout_handler_mutex);
}

/**
 * @brief Writes a log event to a file.
 *
 * This function writes a log message to a file specified in the log attributes,
 * including the date, time, severity, source file, line number, and the log 
 * message.
 *
 * @param event The log event to write.
 */
static void write_file_handler(LogEvent event) {
  pthread_mutex_lock(&write_file_handler_mutex);

  char fpath[256];
  static FILE* file = NULL;
  static int lcount = 0, fcount = 0;

  if (__log_attribute == NULL || __log_attribute->path == NULL) {
    pthread_mutex_unlock(&write_file_handler_mutex);
    return;
  }

  if (lcount >= __log_attribute->max_line_size) {
    fclose(file);
    lcount = 0;
    fcount++;
  }

  if (fcount == 0) {
    snprintf(fpath, sizeof(fpath), "%s", __log_attribute->path);
  } else {
    snprintf(fpath, sizeof(fpath), "%s.%d", __log_attribute->path, fcount);
  }

  if ((file = fopen(fpath, "a")) == NULL) {
    pthread_mutex_unlock(&write_file_handler_mutex);
    return;
  }

  char obuff[64];
  const char* msg = "[%s][%s] - %s:%d - %s\n";
  const char* cseverity = str_log_severities[event.severity];

  Timestamp tms;

  if (current_timestamp(&tms, true) != GOMBOC_CONATE_NO_ERROR) {
    tms.timestamp = 0;
    tms.milliseconds = 0;
  }

  if (date_time_mil_str(&tms, obuff, sizeof(obuff), TIME_FMT) !=
      GOMBOC_CONATE_NO_ERROR) {
    snprintf(obuff, sizeof(obuff), "UNKNOWN_TIME");
  }

  fprintf(file, msg, obuff, cseverity, event.rlt, event.line, event.msg);
  fclose(file);

  lcount++;

  pthread_mutex_unlock(&write_file_handler_mutex);
}

/**
 * @brief Thread callback function to handle log events.
 *
 * This function is executed in a separate thread to handle log events by
 * invoking the appropriate log handler function.
 *
 * @param vargs The arguments passed to the thread, containing the log event
 *              handler and the log event.
 * @return NULL
 */
static void* pthread_handler_fn(void* vargs) {
  LogEventHandler* ptr_log_event_handler = (LogEventHandler*)(vargs);
  LogEvent event = *ptr_log_event_handler->event;
  gomboc_log_handler handler =
      (gomboc_log_handler)((*ptr_log_event_handler).handler->fnp);

  // Calls the function with the appropriate injected event
  handler(event);

  return NULL;
}

/**
 * @brief Logs a message with the specified severity and details.
 *
 * This function logs a message with the specified severity, source file,
 * and line number. It creates a log event and dispatches it to all registered
 * log handlers.
 *
 * @param msg The log message format string.
 * @param rlt The source file name.
 * @param line The line number in the source file.
 * @param severity The log severity.
 * @param ... Additional arguments for the log message format string.
 */
void gomboc_logger(char* msg, char* rlt, int line, LogSeverity severity, ...) {
  va_list ap;
  va_start(ap, severity);

  char buffer[GOMBOC_LOGGER_BUFFER_SIZE];
  vsnprintf(buffer, GOMBOC_LOGGER_BUFFER_SIZE, msg, ap);

  va_end(ap);

  LogHandler* ptr_log_handler = __log_handlers;
  LogEvent* ptr_log_event = malloc(sizeof(LogEvent));

  ptr_log_event->line = line;
  ptr_log_event->rlt = strdup(rlt);
  ptr_log_event->msg = strdup(buffer);
  ptr_log_event->severity = severity;
  ptr_log_event->timestamp = (int)time(NULL);

  while (ptr_log_handler->name != NULL && ptr_log_handler->fnp != NULL) {
    pthread_t thread_id;
    LogEventHandler* log_event_handler = malloc(sizeof(LogEventHandler));

    if (log_event_handler == NULL) {
      free(ptr_log_event->msg);
      free(ptr_log_event->rlt);
      return;
    }

    log_event_handler->event = ptr_log_event;
    log_event_handler->handler = ptr_log_handler;

    pthread_create(&thread_id, NULL, pthread_handler_fn, log_event_handler);
    pthread_detach(thread_id);
    ptr_log_handler++;
  }
}

/**
 * @brief Adds a log handler to the logger.
 *
 * This function adds a new log handler to the logger, allowing it to process
 * log events. It returns an error code if the maximum number of handlers is
 * exceeded.
 *
 * @param name The name of the log handler.
 * @param fnp The function pointer to the log handler.
 * @return int Status code indicating the result of the operation.
 * @retval GOMBOC_LOGGER_NO_ERROR Success.
 * @retval GOMBOC_LOGGER_HANDLER_LIMIT_ERROR Maximum number of handlers 
 *         exceeded.
 */
int gomboc_add_log_handler(char* name, gomboc_log_handler fnp) {
  int status = GOMBOC_LOGGER_NO_ERROR;

  int sent_index = -1;
  LogHandler handler = {.name = name, .fnp = (gomboc_log_handler*)fnp};
  int nhandlers = sizeof(__log_handlers) / sizeof(__log_handlers[0]);

  for (int i = 0; i < nhandlers; i++) {
    if (__log_handlers[i].name == NULL && __log_handlers[i].fnp == NULL) {
      sent_index = i;
      break;
    }
  }

  if (sent_index == -1 || sent_index >= GOMBOC_LOGGER_SET_MAX_HANDLERS) {
    status = GOMBOC_LOGGER_HANDLER_LIMIT_ERROR;
    goto clean_up;
  }

  if (sent_index > GOMBOC_LOGGER_SET_MAX_HANDLERS - 1) {
    status = GOMBOC_LOGGER_HANDLER_LIMIT_ERROR;
    goto clean_up;
  }

  __log_handlers[sent_index + 1] = __log_handlers[sent_index];
  __log_handlers[sent_index] = handler;

clean_up:
  return status;
}

/**
 * @brief Removes a log handler from the logger.
 *
 * This function removes a log handler by name. It returns an error code if the
 * handler is not found.
 *
 * @param name The name of the log handler to remove.
 * @return int Status code indicating the result of the operation.
 * @retval GOMBOC_LOGGER_NO_ERROR Success.
 * @retval GOMBOC_LOGGER_HANDLER_NOT_FOUND Handler not found.
 */
int gomboc_remove_log_handler(char* name) {
  int status = GOMBOC_LOGGER_NO_ERROR;

  int n = 0;
  int found_index = -1;

  while (__log_handlers[n].name != NULL && __log_handlers[n].fnp != NULL) {
    if (strcmp(__log_handlers[n].name, name) == 0) {
      found_index = n;
      break;
    }
    n++;
  }

  if (found_index == -1) {
    status = GOMBOC_LOGGER_HANDLER_NOT_FOUND;
    goto clean_up;
  }

  while (__log_handlers[n].name != NULL && __log_handlers[n].fnp != NULL) {
    __log_handlers[n] = __log_handlers[n + 1];
    n++;
  }

  __log_handlers[n].name = NULL;
  __log_handlers[n].fnp = NULL;

clean_up:
  return status;
}

/**
 * @brief Sets the log attribute for the logger.
 *
 * This function sets the log attribute, including the log file path, maximum
 * size, and maximum line size. It returns an error code if there is an issue
 * with resource allocation.
 *
 * @param attr The log attribute to set.
 * @return int Status code indicating the result of the operation.
 * @retval GOMBOC_LOGGER_NO_ERROR Success.
 * @retval GOMBOC_LOGGER_RESOURCE_ALLOCATION_PROBLEM Resource allocation 
 *         problem.
 */
int gomboc_set_log_attribute(LogAttribute attr) {
  int status = GOMBOC_LOGGER_NO_ERROR;

  if (__log_attribute != NULL) {
    status = GOMBOC_LOGGER_LOG_ATTR_ALREADY_INITIALIZED;
    goto clean_up;
  }

  if ((__log_attribute = malloc(sizeof(LogAttribute))) == NULL) {
    status = GOMBOC_LOGGER_RESOURCE_ALLOCATION_PROBLEM;
    goto clean_up;
  }

  if (memset(__log_attribute, 0, sizeof(LogAttribute)) == NULL) {
    status = GOMBOC_LOGGER_RESOURCE_ALLOCATION_PROBLEM;
    goto clean_up;
  }

  if ((__log_attribute->path = malloc(strlen(attr.path) + 1)) == NULL) {
    status = GOMBOC_LOGGER_RESOURCE_ALLOCATION_PROBLEM;
    goto clean_up;
  }

  if (strcpy(__log_attribute->path, attr.path) == NULL) {
    status = GOMBOC_LOGGER_RESOURCE_ALLOCATION_PROBLEM;
    goto clean_up;
  }

  __log_attribute->max_line_size = attr.max_line_size;

clean_up:
  return status;
}
