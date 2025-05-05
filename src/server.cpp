#include <iostream>
#include <cctype>

#include "server.hpp"
#include "utils.hpp"
#include "parser.hpp"

Server::Server(char* path) {
    path_ = path;
    if (path_.is_relative()) {
        path_ = std::filesystem::absolute(path_);
    }

    Parser p(std::move(cfg_), path_);
    cfg_ = p.parse();
    

    state_ = State(cfg_.table_number, cfg_.start, cfg_.end, cfg_.price_per_hour); 
}

void Server::run() {
    std::cout << util::time_to_string(state_.start) << std::endl;
    
    for (int i = 0; i < cfg_.events.size(); i++) {
        cfg_.events[i]->handle(state_); 
    }
    
    std::vector<std::string> to_be_sorted(state_.pool.begin(), state_.pool.end());
    std::sort(to_be_sorted.begin(), to_be_sorted.end());

    for (const auto& client: to_be_sorted) {
        EventFactory::create(util::time_to_string(state_.end), 11, client)->handle(state_);
    }

    std::cout << util::time_to_string(state_.end) << std::endl;

    for (int i = 1; i < state_.tables.size(); i++) {
        std::cout << i << " " << state_.tables[i].total_salary
                  << " " << util::time_to_string(state_.tables[i].total_minutes)
                  << std::endl;
    }
}

