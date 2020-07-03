#pragma once

#if _DEBUG

#define DEBUG(x) x

#define DEBUG_PRINT(value, ...) \
    printf(value, __VA_ARGS__)

#else

#define DEBUG(x)

#define DEBUG_PRINT(value, ...)

#endif