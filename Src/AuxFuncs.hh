/**
 *   @file: AuxFuncs.hh
 * @author: Nicholas Adkins
 *   @date: Sep 19 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"

//Reads contents of a file from the given path
//returns a string full of the file's contents if successful
//otherwise returns std::nullopt if it fails
optional<string> get_file_contents(path cref file_p);

namespace Log
{
    //TODO DESC: Initialize Logging stuff at a specified mode (refer to Log::log_mode variable desc)
    void init_logging(int mode);

    //TODO DESC: Set to value to enable certain log messages
    // 0 : Off
    // 1 : Errors
    // 2 : Errors + Warnings
    // 3 : Errors + Warnings + Info (All)
    int inline log_mode = 0;

    //TODO DESC: ofstream to handle sending data to log file
    std::ofstream inline log_file;

    //TODO DESC: uses std::format parameters & outputs to log file
    template<class... Args>
    inline void info(std::format_string<Args...> fmt, Args&&... args)
    {
        if (log_mode >= 3)
        {
            log_file << " INFO: "
                << format(fmt, std::forward<Args>(args)...)
                << '\n';
        }
    }

    //TODO DESC: uses std::format parameters & outputs to log file
    template<class... Args>
    inline void warn(std::format_string<Args...> fmt, Args&&... args)
    {
        if (log_mode >= 2)
        {
            log_file << " WARN: "
                << format(fmt, std::forward<Args>(args)...)
                << '\n';
        }
    }

    //TODO DESC: uses std::format parameters & outputs to log file
    template<class... Args>
    inline void error(std::format_string<Args...> fmt, Args&&... args)
    {
        if (log_mode >= 1)
        {
            log_file << "ERROR: "
                << format(fmt, std::forward<Args>(args)...)
                << '\n';
        }
    }
}
