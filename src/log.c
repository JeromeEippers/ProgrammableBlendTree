#include "log.h"
#include <stdio.h>
#include <stdarg.h>

static struct {
    int level;
} LOG_ENV;

static const char *level_strings[] = {
    "\x1b[94m TRACE \x1B[0m", "\x1b[36m DEBUG \x1B[0m", "\x1b[32m INFO \x1B[0m", "\x1b[33m WARN \x1B[0m", "\x1b[31m ERROR \x1B[0m"
};

void pbt_log_set_level(int level)
{
    LOG_ENV.level = level;
}

void pbt_log(int level, const char *file, int line, const char *fmt, ...)
{
    if(level >= LOG_ENV.level)
    {
        fprintf(stdout, "%s %s:%d: ", level_strings[level], file, line);
        va_list args;
        va_start(args, fmt);
        vfprintf(stdout, fmt, args);
        va_end(args);
        fprintf(stdout, "\n");
    }
}