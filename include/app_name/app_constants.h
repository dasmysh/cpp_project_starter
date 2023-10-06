/**
 * @file   constants.h
 * @author @AUTHOR <@AUTHOR_EMAIL>
 * @date   @DATE
 *
 * @brief  Contains global constant definitions.
 */

#pragma once

#include <cstdint>
#include <string_view>

namespace @NAMESPACE {
    /** The application name. */
    constexpr std::string_view applicationName = "@APP_NAME";
    /** The application version. */
    constexpr std::uint32_t applicationVersionMajor = 0;
    constexpr std::uint32_t applicationVersionMinor = 0;
    constexpr std::uint32_t applicationVersionPatch = 1;
    constexpr std::uint32_t applicationVersion = ((applicationVersionMajor << 22u) | (applicationVersionMinor << 12u) | applicationVersionPatch);

    /** The log file name. */
    constexpr std::string_view logFileName = "application.log";
    /** Use a timestamp for the log files. */
    constexpr bool LOG_USE_TIMESTAMPS = false;
    /** Log file application tag. */
    constexpr std::string_view logTag = "@NAMESPACE";

#ifdef NDEBUG
    constexpr bool debug_build = false;
#else
    constexpr bool debug_build = true;
#endif
}
