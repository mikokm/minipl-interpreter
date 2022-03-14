#ifndef UTILS_H
#define UTILS_H

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

extern bool gLog;

inline void printf_stderr(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
}

#define LOG(...) \
  if (gLog) printf_stderr(__VA_ARGS__)
#define LOG_ERROR(...)          \
  do {                          \
    printf_stderr(__VA_ARGS__); \
    abort();                    \
  } while (0)

#endif  // UTILS_H
