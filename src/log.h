#ifndef _INC_PBT_LOG_
#define _INC_PBT_LOG_

enum {PbtLOG_TRACE, PbtLOG_DEBUG, PbtLOG_WARN, PbtLOG_ERROR};

#define pbt_log_trace(...)  pbt_log(PbtLOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define pbt_log_debug(...)  pbt_log(PbtLOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define pbt_log_warn(...)   pbt_log(PbtLOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define pbt_log_error(...)  pbt_log(PbtLOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)

void pbt_log_set_level(int level);
void pbt_log(int level, const char *file, int line, const char *fmt, ...);
void pbt_log_python_error();

#endif