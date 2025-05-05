#pragma once 

#include <chrono>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "session_manager.hpp"

class State {
public:
    State() : session_manager(-1) {};

    State(uint64_t table_number, const std::chrono::minutes& start,
          const std::chrono::minutes& end, uint64_t price_per_hour) : 
        table_number(table_number), start(start), end(end), price_per_hour(price_per_hour),
        session_manager(price_per_hour) {
        for (int i = 0; i < table_number + 1; i++) {
            tables[i] = State::TableData(); 
        }
    }

    struct TableData {
    public:
        std::string current_owner;
        int total_salary;
        std::chrono::minutes total_minutes;
    };

    uint64_t table_number;
    std::chrono::minutes start;
    std::chrono::minutes end;
    uint64_t price_per_hour;

    std::unordered_set<std::string> pool; // all clients who in the building
    SessionManager session_manager;

    std::unordered_map<int, TableData> tables; // table_number : <current_owner, total_salary, total_minutes>
    std::queue<std::string> queue;
};
