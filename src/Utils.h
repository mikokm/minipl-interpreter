#ifndef UTILS_H
#define UTILS_H

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

inline void printf_stderr(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
}

#define LOG_ENABLED 0
#if LOG_ENABLED
#define LOG(...) printf_stderr(__VA_ARGS__)
#define LOG_ERROR(...) \
  do {                 \
    LOG(__VA_ARGS__);  \
    abort();           \
  } while (0)
#else
#define LOG(...)
#define LOG_ERROR(...)
#endif

#endif  // UTILS_H
