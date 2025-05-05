#pragma once

#include "event.hpp"

class Config {
public:
    uint64_t table_number;
    std::chrono::minutes start;
    std::chrono::minutes end;
    uint64_t price_per_hour;
    std::vector<std::unique_ptr<Event>> events;

    // TODO constructors

    Config() = default;
    Config(Config&&) = default;
    Config& operator=(Config&&) = default;

    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
};
