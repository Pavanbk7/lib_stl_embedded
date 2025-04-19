#ifndef ESTL_CONFIG_HPP
#define ESTL_CONFIG_HPP

// Configuration macros for the embedded STL library

// Assertion handling
#ifndef ESTL_ASSERT
    #ifdef NDEBUG
        #define ESTL_ASSERT(condition) ((void)0)
    #else
        #include <cassert>
        #define ESTL_ASSERT(condition) assert(condition)
    #endif
#endif

// Memory alignment
#ifndef ESTL_ALIGNOF
    #define ESTL_ALIGNOF(type) alignof(type)
#endif

// Platform-specific configurations
#if defined(__arm__) || defined(__thumb__) || defined(__ARM_ARCH)
    #define ESTL_PLATFORM_ARM
#elif defined(__AVR__)
    #define ESTL_PLATFORM_AVR
#endif

// Memory management configuration
// By default, no dynamic memory allocation is used
#ifndef ESTL_USE_DYNAMIC_MEMORY
    #define ESTL_USE_DYNAMIC_MEMORY 0
#endif

// Version information
#define ESTL_VERSION_MAJOR 0
#define ESTL_VERSION_MINOR 1
#define ESTL_VERSION_PATCH 0

#endif // ESTL_CONFIG_HPP
