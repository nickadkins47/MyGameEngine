/**
 *   @file: AuxFuncs.cc
 * @author: Nicholas Adkins
 *   @date: Sep 19 2025
 *  @brief: 
 */

#include <chrono>
#include <sstream>

#include "AuxFuncs.hh"

optional<string> get_file_contents(path cref file_p)
{
    //Lack of logging here bc i prefer doing it in the
    //places where this function is used, rather than here
    std::ifstream in_file(file_p);
    if (in_file.fail())
    {
        return nullopt;
    }
    else return (std::stringstream() << in_file.rdbuf()).str();
    //in_file goes out of scope -> close()
}

void Log::init_logging(int mode)
{
    if (Log::log_mode != 0) return; //already set
    Log::log_mode = mode;

    auto now_utc = std::chrono::system_clock::now();
    auto now_local = std::chrono::zoned_time(std::chrono::current_zone(), now_utc);
    string log_f_name = format("_D-{:%F}_T-{:%r}_.log", now_local, now_local);
    for (char ref c : log_f_name)
        if (c == ':' || c == ' ') c = '-';

    Log::log_file.open("Logs/" + log_f_name);
    if (Log::log_file.fail())
    {
        print("ERROR: FAILED to open log\n");
        Log::log_mode = 0;
    }
}
