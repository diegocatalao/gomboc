#ifndef __H_GOMBOC_LOGGER_TYPES__
#define __H_GOMBOC_LOGGER_TYPES__

/**
 * @enum LogSeverity
 * @brief Enumeration of log severity levels.
 */
typedef enum {
  VERBOSE, /**< Detailed information */
  INFO,    /**< General information */
  DEBUG,   /**< Debugging information */
  WARN,    /**< Warning conditions */
  ERROR,   /**< Error conditions */
  CRIT,    /**< Critical conditions */
} LogSeverity;

/**
 * @struct LogAttribute
 * @brief Attributes for configuring log files.
 */
typedef struct {
  char* path;        /**< Path to the log file */
  int max_line_size; /**< Maximum size of a log lines in file */
} LogAttribute;

/**
 * @struct LogEvent
 * @brief Structure representing a log event.
 */
typedef struct {
  long long timestamp;  /**< Timestamp of the event */
  int line;             /**< Line number in the source code */
  char* rlt;            /**< Relative path of the source file */
  char* msg;            /**< Log message */
  LogSeverity severity; /**< Severity of the log event */
} LogEvent;

/**
 * @struct LogHandler
 * @brief Structure representing a log handler.
 */
typedef struct {
  char* name; /**< Name of the handler */
  void* fnp;  /**< Function pointer to the handler */
} LogHandler;

/**
 * @struct LogEventHandler
 * @brief Structure representing an event and its handler.
 */
typedef struct {
  LogEvent* event;     /**< Pointer to the log event */
  LogHandler* handler; /**< Log handler */
} LogEventHandler;

/**
 * @typedef gomboc_log_handler
 * @brief Function pointer type for log handlers.
 */
typedef void (*gomboc_log_handler)(LogEvent event);

#endif  // __H_GOMBOC_TYPES__
