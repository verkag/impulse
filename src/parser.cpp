#include <sstream>
#include <fstream>
#include <algorithm> 
#include <iostream>

#include "parser.hpp"

Parser::Parser(util::Config& c, const std::filesystem::path& p) : conf_(c), path_(p) {}

bool Parser::is_valid_name(const std::string& str) {
    if (!std::all_of(str.begin(), str.end(), [](char c){
        return (c >= 'a' && c <= 'z') ||
        (c >= '0' && c <= '9') ||
        c == '-' ||
        c == '_';
    })) return false;
    return true;
}

bool Parser::is_suitable(const Event& e) {
    const auto args = e.get_args();
    switch (e.get_id()) {
        case 1: 
            if (args.size() != 1) return false;
            if (!is_valid_name(args[0])) return false;
            return true;
        case 2: 
            if (args.size() != 2) return false;
            if (!is_valid_name(args[0])) return false;
            if (std::stoi(args[1]) > conf_.table_number_) return false;
            return true;
        case 3: 
            if (args.size() != 1) return false;
            if (!is_valid_name(args[0])) return false;
            return true;
        case 4: 
            if (args.size() != 1) return false;
            if (!is_valid_name(args[0])) return false;
            return true;
        default: 
            return false;
    }
}

Event Parser::parse_line(const std::string& str) {
    std::istringstream iss(str);
    std::string tmp;

    std::getline(iss, tmp, ' ');
    std::string time = tmp; 

    std::getline(iss, tmp, ' ');
    int id = std::stol(tmp); 

    std::vector<std::string> args;
    while(std::getline(iss, tmp, ' ')) {
        args.push_back(tmp);
    }

    if (std::isspace(static_cast<unsigned char>(str.back()))) throw std::exception();
    return Event(time, id, std::move(args)); 
}
void Parser::parse_config(std::ifstream& ifs) {
    std::string tmp;

    try {
        std::getline(ifs, tmp);
        bool only_digits = !tmp.empty() && std::all_of(tmp.begin(), tmp.end(), ::isdigit);
        if (!only_digits) throw std::exception();
        conf_.table_number_ = std::stol(tmp);
        if (conf_.table_number_ <= 0) throw std::exception();
    } catch (...) {
        std::cout << tmp << std::endl;
        std::exit(EXIT_FAILURE);
    }

    try {
        std::getline(ifs, tmp);
        std::istringstream iss(tmp);
        std::string tmp2; 
        std::getline(iss, tmp2, ' '); 
        conf_.start_ = util::to_minutes(tmp2);
        std::getline(iss, tmp2, ' '); 
        conf_.end_ = util::to_minutes(tmp2);
        if (std::getline(iss, tmp2, ' ')) throw std::exception();
        if (std::isspace(static_cast<unsigned char>(tmp.back()))) throw std::exception();
        if (conf_.start_ > conf_.end_) throw std::exception();
    } catch (...) {
        std::cout << tmp << std::endl;
        std::exit(EXIT_FAILURE);
    }

    try {
        std::getline(ifs, tmp);
        bool only_digits = !tmp.empty() && std::all_of(tmp.begin(), tmp.end(), ::isdigit);
        if (!only_digits) throw std::exception();
        conf_.price_per_hour_ = std::stol(tmp); 
        if (conf_.price_per_hour_ <= 0) throw std::exception();
    } catch (...) {
        std::cout << tmp << std::endl; 
        std::exit(EXIT_FAILURE);
    }
}

void Parser::parse_events(std::ifstream& ifs) {
    std::string tmp;

    std::unique_ptr<std::chrono::minutes> last = nullptr;
    while (std::getline(ifs, tmp)) {
        try {
            const auto event = parse_line(tmp);
            if (last != nullptr) {
                if (*last > event.get_time()) throw std::exception();
            }
            if (is_suitable(event)) {
                conf_.events_.push_back(event);
                last = std::make_unique<std::chrono::minutes>(event.get_time());
            } else { 
                std::cout << tmp << std::endl;
                std::exit(EXIT_FAILURE);
            }
        } catch (...) {
            std::cout << tmp << std::endl; 
            std::exit(EXIT_FAILURE);
        }
    }
}

util::Config Parser::parse() {
    std::ifstream stream(path_); 
    if (!stream) {
        std::cerr << "failed to open file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    parse_config(stream);
    parse_events(stream);

    return conf_;
}
