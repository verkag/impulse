#include <cmath>

#include "session_manager.hpp"

void SessionManager::start_session(int table_number, const std::chrono::minutes& mins) {
    sessions_[table_number] = mins;
}

std::tuple<int, std::chrono::minutes> SessionManager::estimate(int table_number, std::chrono::minutes now) {
    std::chrono::minutes diff {now - sessions_[table_number]};
    std::tuple<int, std::chrono::minutes> result;

    int profit = (std::ceil(diff.count() / 60.)) * price_per_hour_;

    return std::make_tuple(profit, diff); 
}
