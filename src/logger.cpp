#include "../include/logger.h"

#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>

void log_message(const std::string &msg)
{
    std::ofstream log("logs/server.log", std::ios::app);

    if (!log.is_open())
        return;

    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    log << "[" << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S")
        << "] " << msg << "\n";
} 
