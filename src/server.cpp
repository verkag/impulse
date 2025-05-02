#include <iostream>
#include <sstream>
#include <fstream>
#include "server.hpp"

std::chrono::minutes Server::to_minutes(const std::string& str) {
    std::istringstream iss(str);
    int h, m;
    char sep;
    iss >> h >> sep >> m;
    return std::chrono::hours(h) + std::chrono::minutes(m);
}


Server::Event Server::parse_line(const std::string& str) {
    std::istringstream iss(str);
    std::string time; 
    int id; 
    std::vector<std::string> args;
    std::string token;

    iss >> time >> id;
    while (iss >> token) {
        args.push_back(token);
    }
    return Event(time, id, std::move(args)); 
}

Server::Server(char* path) {
    path_ = path;
    if (path_.is_relative()) {
        path_ = std::filesystem::absolute(path_);
    }
    parse();
}

void Server::parse() {
    std::ifstream stream(path_); 
    if (!stream) {
        std::cerr << "failed to open file" << std::endl;
        return;
    }
    

    stream >> table_number_;

    std::string time1, time2;
    stream >> time1 >> time2;
    start_ = to_minutes(time1); 
    end_ = to_minutes(time2); 

    stream >> price_per_hour_;

    std::string tmp;
    std::getline(stream, tmp); // throw away newline char
    while (std::getline(stream, tmp)) {
        std::cout << "processing line: " << tmp << std::endl;
        events_.push_back(parse_line(tmp));
    }
}

void Server::run() {
    std::cout << events_.size() << std::endl;;
    for (const auto& i : events_) {
        std::cout << i.get_time() << std::endl;
    }
}
