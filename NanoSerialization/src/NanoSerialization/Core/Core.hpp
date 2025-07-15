#pragma once

#include <Nano/Nano.hpp>

namespace Nano::Serialization
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Logging & Asserting macros
    ////////////////////////////////////////////////////////////////////////////////////
    #ifndef NS_CONFIG_DIST
        #define NS_LOG_TRACE(fmt, ...)       ::Nano::Log::PrintLvl<::Nano::Log::Level::Trace>(fmt __VA_OPT__(,) __VA_ARGS__)
        #define NS_LOG_INFO(fmt, ...)        ::Nano::Log::PrintLvl<::Nano::Log::Level::Info>(fmt __VA_OPT__(,) __VA_ARGS__)
        #define NS_LOG_WARN(fmt, ...)        ::Nano::Log::PrintLvl<::Nano::Log::Level::Warn>(fmt __VA_OPT__(,) __VA_ARGS__)
        #define NS_LOG_ERROR(fmt, ...)       ::Nano::Log::PrintLvl<::Nano::Log::Level::Error>(fmt __VA_OPT__(,) __VA_ARGS__)
        #define NS_LOG_FATAL(fmt, ...)       ::Nano::Log::PrintLvl<::Nano::Log::Level::Fatal>(fmt __VA_OPT__(,) __VA_ARGS__)

        #define NS_ASSERT(x, fmt, ...)  \
            do                          \
            {                           \
                if (!(x))               \
                {                       \
                    NS_LOG_FATAL("Assertion failed: ({0}), {1}.", #x, ::Nano::Text::Format(fmt __VA_OPT__(,) __VA_ARGS__)); \
                    NANO_DEBUG_BREAK(); \
                }                       \
            } while (false)

        #define NS_VERIFY(x, fmt, ...)  \
            do                          \
            {                           \
                if (!(x))               \
                {                       \
                    NS_LOG_FATAL("Verify failed: ({0}), {1}.", #x, ::Nano::Text::Format(fmt __VA_OPT__(,) __VA_ARGS__)); \
                }                       \
            } while (false)

        #define NS_UNREACHABLE() NS_ASSERT(false, "Unreachable")

    #else
        #define NS_LOG_TRACE(fmt, ...) 
        #define NS_LOG_INFO(fmt, ...) 
        #define NS_LOG_WARN(fmt, ...) 
        #define NS_LOG_ERROR(fmt, ...) 
        #define NS_LOG_FATAL(fmt, ...) 

        #define NS_ASSERT(x, fmt, ...)
        #define NS_VERIFY(x, fmt, ...)
        #define NS_UNREACHABLE()
    #endif

}