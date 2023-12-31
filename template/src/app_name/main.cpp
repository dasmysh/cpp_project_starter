/**
 * @file   main.cpp
 * @author @AUTHOR <@AUTHOR_EMAIL>
 * @date   @DATE
 *
 * @brief  Implements the applications entry point for windows.
 */

#include "main.h"
#include "app_constants.h"
#include "core/spdlog/sinks/filesink.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/async.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <iostream>


int main(int /* argc */, const char** /* argv */) // NOLINT(bugprone-exception-escape)
{
    try {
        constexpr std::string_view directory; // = "";
        constexpr std::string_view name = @NAMESPACE::logFileName;

        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::warn);
        console_sink->set_pattern(fmt::format("[{}] [%^%l%$] %v", @NAMESPACE::logTag));

        auto devenv_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
        devenv_sink->set_level(spdlog::level::err);
        devenv_sink->set_pattern(fmt::format("[{}] [%^%l%$] %v", @NAMESPACE::logTag));

        std::shared_ptr<spdlog::sinks::base_sink<std::mutex>> file_sink;
        if constexpr (@NAMESPACE::debug_build) {
            file_sink = std::make_shared<mysh::core::spdlog::sinks::rotating_open_file_sink_mt>(
                directory.empty() ? std::string{name} : std::string{directory}.append("/").append(name), 5);
            file_sink->set_level(spdlog::level::trace);
        } else {
            file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
                directory.empty() ? std::string{name} : std::string{directory}.append("/").append(name), 5);
            file_sink->set_level(spdlog::level::trace);
        }

        const spdlog::sinks_init_list sink_list = {file_sink, console_sink, devenv_sink};
        auto logger = std::make_shared<spdlog::logger>(@NAMESPACE::logTag.data(), sink_list.begin(), sink_list.end());

        spdlog::set_default_logger(logger);
        spdlog::flush_on(spdlog::level::err);

        if constexpr (@NAMESPACE::debug_build) {
            spdlog::set_level(spdlog::level::trace);
        } else {
            spdlog::set_level(spdlog::level::err);
        }

        spdlog::info("Log created.");

    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
        return 0;
    }


    spdlog::debug("Starting main loop.");

    spdlog::debug("Main loop ended.");

    return 0;
}
