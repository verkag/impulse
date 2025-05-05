#pragma once 

#include <chrono>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>

class State {
public:
    uint64_t table_number_;
    std::chrono::minutes start_;
    std::chrono::minutes end_;
    uint64_t price_per_hour_;

    //std::unordered_map<std::string, std::string> pool_; // client_name, start_time remake to std::set(unordered) 
    //std::vector<Session> sessions_; // Session: <table_number, time_spent_on_current_table>
    //std::unordered_map<int, util::Config> zxc;
    std::unordered_set<std::string> pool_; // all clients who in the building

    struct TableData {
    public:
        std::string current_owner;
        int total_salary;
        std::chrono::minutes total_minutes;
    };
    std::unordered_map<int, TableData> tables_; // table_number : <current_owner, total_salary, total_minutes>
    std::queue<std::string> queue_;
    int int_state;
};
