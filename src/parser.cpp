#include <sstream>
#include <fstream>
#include <algorithm> 
#include <iostream>

#include "parser.hpp"

Parser::Parser(Config&& c, const std::filesystem::path& p) : conf_(std::move(c)), path_(p) {}

std::unique_ptr<Event> Parser::parse_line(const std::string& str) {
    std::istringstream iss(str);
    std::string tmp;

    std::getline(iss, tmp, ' ');
    std::string time = tmp; 

    std::getline(iss, tmp, ' ');
    int id = std::stol(tmp); 

    int table_number = 0;

    std::string str_arg;
    std::getline(iss, str_arg, ' ');
    if (std::getline(iss, tmp, ' ')) {
        table_number = std::stol(tmp);
        if (table_number > conf_.table_number_ || table_number == 0) throw std::exception();
    }

    if (std::isspace(static_cast<unsigned char>(str.back()))) throw std::exception();
    return EventFactory::create(time, id, str_arg, table_number); 
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
            auto event = parse_line(tmp);
            if (last != nullptr) {
                if (*last > event->get_time()) throw std::exception();
            }

            last = std::make_unique<std::chrono::minutes>(event->get_time());

            conf_.events_.push_back(std::move(event));
        } catch (...) {
            std::cout << tmp << std::endl; 
            std::exit(EXIT_FAILURE);
        }
    }
}

Config Parser::parse() {
    std::ifstream stream(path_); 
    if (!stream) {
        std::cerr << "failed to open file" << std::endl;
        std::exit(EXIT_FAILURE);
    }


    parse_config(stream);
    parse_events(stream);


    return std::move(conf_);
}
