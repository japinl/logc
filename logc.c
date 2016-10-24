#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "logc.h"

static logc_t            slog;
static pthread_mutex_t   log_mutex = PTHREAD_MUTEX_INITIALIZER;


static const char *level_names[] = {
    "MIN", "FATAL", "ERROR", "WARN",
    "INFO", "DEBUG", "TRACE", "MAX",
};


/*
 * @brief Write log message using thread mutex.
 */
static void logc_lock_write(int fd, const void *buf, size_t count)
{
    pthread_mutex_lock(&log_mutex);
    write(fd, buf, count);
    pthread_mutex_unlock(&log_mutex);
}

/*
 * @brief Get time and format it in buf.
 * @param buf [in/out] The buffer starting pointer.
 * @param len [in] The length of buf.
 * @return The number of bytes that written in buf.
 */
static size_t logc_format_time(char *buf, size_t len)
{
    size_t         rs;
    struct tm      tm;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    gmtime_r(&tv.tv_sec, &tm);
    rs = snprintf(buf, len, "%04d/%02d/%02d %02d:%02d:%02d.%ldZ",
                  tm.tm_year + 1990, tm.tm_mon + 1, tm.tm_mday,
                  tm.tm_hour, tm.tm_min, tm.tm_sec,tv.tv_usec);
    return rs;
}

/*
 * @brief Core function for logging message in file.
 * @param level [in] The level for logging message.
 * @param log [in] Tha address of log structure.
 * @param fmt [in] The message format.
 * @param args [in] The parameters.
 */
static void logc_core(logc_level_t level, logc_t *log,
                      const char *fmt, va_list args)
{
    char       errmsg[MAX_ERR_MSG];
    size_t     max = MAX_ERR_MSG - 1;
    size_t     len = 0;
    
    len += logc_format_time(errmsg, max - len);

    len += snprintf(errmsg + len, max - len, " [%s] ", level_names[level]);
    
    len += vsnprintf(errmsg + len, max - len, fmt, args);
    
    errmsg[len++] = '\n';
    write(log->fd, errmsg, len);
//    logc_lock_write(log->fd, errmsg, len);
}


/****************************************************/


logc_t *logc_init(const char *pathname)
{
    slog.fd = open(pathname, O_CREAT | O_APPEND | O_RDWR, 0666);
    if (-1 == slog.fd) {
        logc_stderr("open(%s) failed", pathname);
        return NULL;
    }

    slog.level = LOGC_DEBUG;

    return &slog;
}

void logc_deinit(logc_t *log)
{
    if (log && -1 != log->fd) {
        close(log->fd);
    }
}

void logc_set_level(logc_t *log, logc_level_t level)
{
    if (level <= LOGC_LEVEL_MIN || level >= LOGC_LEVEL_MAX) {
        logc_warn(log, "invalid log level");
        return;
    }

    log->level = level;
}

void logc_stderr_core(const char *file, int line, const char *fmt, ...)
{
    char       errmsg[MAX_ERR_MSG];
    size_t     max = MAX_ERR_MSG - 1;
    size_t     len = 0;
    va_list    args;

    
    len += snprintf(errmsg + len, max - len, "%s:%d: ", file, line);

    va_start(args, fmt);
    len += vsnprintf(errmsg + len, max - len, fmt, args);
    va_end(args);

    errmsg[len++] = '\n';
    write(STDERR_FILENO, errmsg, len);
}

void logc_fatal(logc_t *log, const char *fmt, ...)
{
    va_list args;

    if (log->level >= LOGC_FATAL) {
        va_start(args, fmt);
        logc_core(LOGC_FATAL, log, fmt, args);
        va_end(args);
    }
}

void logc_error(logc_t *log, const char *fmt, ...)
{
    va_list args;
    
    if (log->level >= LOGC_ERROR) {    
        va_start(args, fmt);
        logc_core(LOGC_ERROR, log, fmt, args);
        va_end(args);
    }
}

void logc_warn(logc_t *log, const char *fmt, ...)
{
    va_list args;

    if (log->level >= LOGC_WARN) {    
        va_start(args, fmt);
        logc_core(LOGC_WARN, log, fmt, args);
        va_end(args);
    }
}

void logc_info(logc_t *log, const char *fmt, ...)
{
    va_list args;

    if (log->level >= LOGC_INFO) {    
        va_start(args, fmt);
        logc_core(LOGC_INFO, log, fmt, args);
        va_end(args);
    }
}

void logc_debug(logc_t *log, const char *fmt, ...)
{
    va_list args;

    if (log->level >= LOGC_DEBUG) {    
        va_start(args, fmt);
        logc_core(LOGC_DEBUG, log, fmt, args);
        va_end(args);
    }
}

void logc_trace(logc_t *log, const char *fmt, ...)
{
    va_list args;

    if (log->level >= LOGC_TRACE) {
        va_start(args, fmt);
        logc_core(LOGC_TRACE, log, fmt, args);
        va_end(args);
    }
}
