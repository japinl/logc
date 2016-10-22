#ifndef __LOGC_H__
#define __LOGC_H__

#define MAX_ERR_MSG            2048

typedef enum logc_level_e {
    LOGC_LEVEL_MIN = 0,
    LOGC_FATAL,
    LOGC_ERROR,
    LOGC_WARN,
    LOGC_INFO,
    LOGC_DEBUG,
    LOGC_TRACE,
    LOGC_LEVEL_MAX,
} logc_level_t;

typedef struct logc_s {
    int          fd;
    logc_level_t level;
} logc_t;


#define logc_stderr(fmt, ...)                \
    logc_stderr_core(__FILE__, __LINE__, fmt, ##__VA_ARGS__)


/*
 * @brief Initialize log structure.
 * @param pathname [in] The name of log file.
 * @return On success, the address of log is returned. 
 *  On error, NULL is returned.
 */
logc_t *logc_init(const char *pathname);

/*
 * @brief Release log resources.
 * @param log [in] The address of log.
 */
void logc_deinit(logc_t *log);

/*
 * @brief Set log level.
 * @param log [in] The address of log structure.
 * @param level [in] The level of log.
 */
void logc_set_level(logc_t *log, logc_level_t level);

/*
 * @brief Print message on stderr.
 * @param file [in] The name of current file.
 * @param line [in] The line number in file.
 * @param fmt [in] The message format.
 */
void logc_stderr_core(const char *file, int line, const char *fmt, ...);

/*
 * @brief Log message in log file.
 * @param log [in] Address of log structure.
 */
void logc_fatal(logc_t *log, const char *fmt, ...);
void logc_error(logc_t *log, const char *fmt, ...);
void logc_warn(logc_t *log, const char *fmt, ...);
void logc_info(logc_t *log, const char *fmt, ...);
void logc_debug(logc_t *log, const char *fmt, ...);
void logc_trace(logc_t *log, const char *fmt, ...);

#endif /* __LOGC_H__ */
