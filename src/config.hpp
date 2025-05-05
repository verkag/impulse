#pragma once

#include "event.hpp"

class Config {
public:
    uint64_t table_number_;
    std::chrono::minutes start_;
    std::chrono::minutes end_;
    uint64_t price_per_hour_;
    std::vector<std::unique_ptr<Event>> events_;

    // TODO constructors

    Config() = default;
    Config(Config&&) = default;
    Config& operator=(Config&&) = default;

    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
};
