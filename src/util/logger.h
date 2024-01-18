// Copyright (C) 2023, 2024 Ethan Uppal. All rights reserved.

#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <cstdbool>

/** A wrapper around output logging. */
class Logger {
public:
    struct LocationInfo {
        std::string file;
        int line;
        std::string func;

        LocationInfo() {}

        LocationInfo(const std::string& file, int line, const std::string& func)
            : file(file), line(line), func(func) {}
    };

private:
    /** The output stream to log to. */
    std::ostream& out;

    /** Whether any location information has been provided. */
    bool has_location_info;

    /** The most recent location information. */
    LocationInfo location_info;

public:
    /** Global logger enable flag. */
    static bool main_enabled;

    /** The global logger instance. */
    static Logger& main() {
        static Logger instance(std::cerr);
        return instance;
    }

    /** Constructs a new Logger that logs to `out`. */
    Logger(std::ostream& out): out(out), has_location_info(false) {}

    /** Logs `val`. */
    template<typename T>
    inline Logger& operator<<(T val) {
#if !defined(RELEASE_BUILD) && !defined(NO_LOGGING)
        if (main_enabled) {
            if (has_location_info) {
                out << "[" << location_info.func << "] ";
                has_location_info = false;
            }
            out << val;
        }
#endif
        return *this;
    }

    /** Sets location information to `location_info`. */
    inline Logger& operator<<(const LocationInfo& location_info) {
        this->location_info = location_info;
        this->location_info.file =
            std::filesystem::path(location_info.file).filename().string();
        has_location_info = true;
        return *this;
    }
};

// See Logger::main
#define BW_Log                                                                 \
    Logger::main() << Logger::LocationInfo(__FILE__, __LINE__, __func__)