#pragma once 

#include <filesystem>

#include "event.hpp"
#include "utils.hpp"

class Parser {
public:
    Parser(util::Config& c, const std::filesystem::path& p);
    util::Config parse();

private:
    void parse_config(std::ifstream& ifs);
    void parse_events(std::ifstream& ifs);
    bool is_suitable(const Event& e);
    bool is_valid_name(const std::string& str);
    Event parse_line(const std::string& str);

    util::Config conf_;
    std::filesystem::path path_;
};
