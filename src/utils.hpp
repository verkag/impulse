#pragma once

#include <chrono>
#include <iomanip>
#include <regex>

namespace util {

static std::chrono::minutes to_minutes(const std::string& str) {
    const std::regex format(R"(^\d{2}:\d{2}$)");
    if (!std::regex_match(str, format)) throw std::exception();

    std::istringstream iss(str);
    int h, m;
    char sep;
    iss >> h >> sep >> m;
    
    if (sep != ':' || h < 0 || h > 23 || m < 0 || m > 59) throw std::exception();
    char extra;
    if (iss >> extra) throw std::exception();

    return std::chrono::hours(h) + std::chrono::minutes(m);
}

static std::string time_to_string(const std::chrono::minutes& time) {
    std::ostringstream oss; 
    std::chrono::hh_mm_ss hms(time);
    oss << std::setfill('0') << std::setw(2) << hms.hours().count() << ":" << std::setw(2) << hms.minutes().count();
    return oss.str();
}

static bool is_valid_name(const std::string& str) {
    if (!std::all_of(str.begin(), str.end(), [](char c){
        return (c >= 'a' && c <= 'z') ||
        (c >= '0' && c <= '9') ||
        c == '-' ||
        c == '_';
    })) return false;
    return true;
}

}
