#pragma once 

#include <filesystem>

#include "event.hpp"
#include "config.hpp"

class Parser {
public:
    Parser(Config&& c, const std::filesystem::path& p);
    Config parse();

private:
    void parse_config(std::ifstream& ifs);
    void parse_events(std::ifstream& ifs);
    std::unique_ptr<Event> parse_line(const std::string& str);

    Config conf_;
    std::filesystem::path path_;
};
