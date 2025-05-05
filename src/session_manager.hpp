#pragma once

#include <chrono>
#include <tuple>
#include <unordered_map>

class SessionManager {
public:
    SessionManager(int price_per_hour) : price_per_hour_(price_per_hour) {}; 
    void start_session(int table_nuber, const std::chrono::minutes& mins);
    std::tuple<int, std::chrono::minutes> estimate(int table_number, std::chrono::minutes now);
private:
    int price_per_hour_;
    std::unordered_map<int, std::chrono::minutes> sessions_;
};

