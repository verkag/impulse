#pragma once 

#include <vector>
#include <filesystem>
#include <chrono>
#include <unordered_map>
#include <queue>

#include "event.hpp"
#include "utils.hpp"

class Server {
public: 
    Server(char* path);
    void run();

private: 
    void handle_event(const Event& e);  // need to move into Event class 

    std::filesystem::path path_;
    util::Config conf_;

    // under dev
    std::unordered_map<int, util::Config> zxc;
    std::unordered_map<int, std::tuple<std::string, int, std::chrono::minutes>> tables_; // table_number : <current_owner, total_salary, total_minutes>
    std::queue<std::string> queue_;
    //std::unordered_map<std::string, std::string> pool_; // client_name, start_time remake to std::set(unordered) 
    //std::vector<Session> sessions_; // Session: <table_number, time_spent_on_current_table>
};

