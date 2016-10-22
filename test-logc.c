#include <stdio.h>
#include <pthread.h>

#include "logc.h"

#define LOG_COUNT            90000

void *thread_func1(void *arg)
{
    logc_t *log = (logc_t *)arg;

    for (int i = 0; i < LOG_COUNT; i++) {
        logc_fatal(log, "test");
        logc_fatal(log, "%s", "1234567890");
    }

    return NULL;
}
void *thread_func2(void *arg)
{
    logc_t *log = (logc_t *)arg;

    for (int i = 0; i < LOG_COUNT; i++) {
        logc_error(log, "test");
        logc_error(log, "%s", "1234567890");
    }

    return NULL;
}
void *thread_func3(void *arg)
{
    logc_t *log = (logc_t *)arg;

    for (int i = 0; i < LOG_COUNT; i++) {
        logc_warn(log, "test");
        logc_warn(log, "%s", "1234567890");
    }

    return NULL;
}
void *thread_func4(void *arg)
{
    logc_t *log = (logc_t *)arg;

    for (int i = 0; i < LOG_COUNT; i++) {
        logc_info(log, "test");
        logc_info(log, "%s", "1234567890");
    }

    return NULL;
}
void *thread_func5(void *arg)
{
    logc_t *log = (logc_t *)arg;

    for (int i = 0; i < LOG_COUNT; i++) {
        logc_debug(log, "test");
        logc_debug(log, "%s", "1234567890");
    }

    return NULL;
}
void *thread_func6(void *arg)
{
    logc_t *log = (logc_t *)arg;

    for (int i = 0; i < LOG_COUNT; i++) {
        logc_trace(log, "trace");
        logc_trace(log, "trace: %s", "1234567890");
    }

    return NULL;
}

typedef void * (*threadfunc)(void *arg);

static threadfunc func[] = {
    &thread_func1, &thread_func2, &thread_func3,
    &thread_func4, &thread_func5, &thread_func6,
};

int main()
{
    logc_t    *log;
    pthread_t  tid[6];

    log = logc_init("test.log");

    for (int i = 0; i < 6; ++i) {
        pthread_create(&tid[i], NULL, func[i], log);
    }

    for (int i = 0; i < 6; ++i) {
        pthread_join(tid[i], NULL);
    }
    
    logc_deinit(log);
    return 0;
}
