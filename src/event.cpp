#include "event.hpp"
#include "utils.hpp"

Event::Event(const std::string& str_time, int id, std::vector<std::string>&& args) :
        time_(util::to_minutes(str_time)), id_(id), args_(std::move(args)) {}

std::chrono::minutes Event::get_time() const { // to be refactored
    return time_;
}

int Event::get_id() const {
    return id_;
}

std::vector<std::string> Event::get_args() const {
    return args_;
}
