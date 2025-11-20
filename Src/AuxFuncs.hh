/**
 *   @file: AuxFuncs.hh
 * @author: Nicholas Adkins
 *   @date: Sep 19 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"

//Anything that needs to be done on program's initialization
void init_program();

//Reads contents of a file from the given path
//returns a string full of the file's contents if successful
//otherwise returns std::nullopt if it fails
optional<string> get_file_contents(string_view file_path);

//TODO DESC: Returns GL error from glGetError() if something fails, else returns nullopt
optional<string> get_gl_error();

//TODO DESC:
//Declare this at the start of a function to track log data
class Log
{
    public:

    //TODO DESC: Start tracking log data when constructed
    template<class... Args>
    Log(std::format_string<Args...> fmt, Args&&... args)
    {
        if (enabled)
        {
            start_time = get_time();
            out_file << string(num_tabs, '\t')
                << '['
                << format(fmt, std::forward<Args>(args)...)
                << "]: START {\n";
            num_tabs++;
        }
    }
    
    //TODO DESC: If section of code is successful, state as such
    ~Log()
    {
        if (enabled && success)
        {
            num_tabs--;
            out_file << string(num_tabs, '\t') 
                << "} SUCCESS"
                << format(" (T: {} s)\n", get_time() - start_time);
        }
    }

    delete_other_ops(Log)

    //TODO DESC: Send additional data to log file
    template<class... Args>
    void info(std::format_string<Args...> fmt, Args&&... args)
    {
        if (enabled)
        {
            out_file << string(num_tabs, '\t')
                << "INFO ("
                << format(fmt, std::forward<Args>(args)...)
                << ")\n";
        }
    }

    //TODO DESC: Upon a section of code failing, call this func with description
    template<class... Args>
    void fail(std::format_string<Args...> fmt, Args&&... args)
    {
        if (enabled && success)
        {
            success = false;
            num_tabs--;
            string const msg = std::format(fmt, std::forward<Args>(args)...);
            out_file << string(num_tabs, '\t') 
                << "} FAILED (" << msg << ')'
                << format(" (T: {} s)\n", get_time() - start_time);
            out_file.flush();
            throw std::runtime_error(msg);
        }
    }

    //TODO DESC: Check latest GL function call via glGetError(), and if it failed print the error code
    template<class... Args>
    void gl_check(string_view message)
    {
        if (enabled && success)
        {
            optional<string> err = get_gl_error();
            if (err != nullopt)
                fail("GL function failed ({}: {})", message, err.value_or("?"));
        }
    }

    //Call this function to enable logging
    static void init_logging();

    protected:

    bool success = true;
    double start_time = 0;

    bool inline static enabled = false;
    int inline static num_tabs = 0;

    std::ofstream inline static out_file;

    static double get_time();
};
