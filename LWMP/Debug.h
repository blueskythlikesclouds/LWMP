#pragma once

#if _DEBUG

#define DEBUG(x) x

#define DEBUG_PRINT(value, ...) \
    printf(value, __VA_ARGS__)

#define DEBUG_NAMEOF(x) NAMEOF(x)

#define DEBUG_NAMEOF_TYPE(x) NAMEOF_TYPE(x)

#else

#define DEBUG(x)

#define DEBUG_PRINT(value, ...)

#define DEBUG_NAMEOF(x) std::string_view()

#define DEBUG_NAMEOF_TYPE(x) std::string_view()

#endif