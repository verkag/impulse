#pragma once 

#include <string>
#include <chrono>
#include <vector>

class Server;

class Event {
    friend class Server;
public:
    Event(const std::string& str_time, int id, std::vector<std::string>&& args);

    std::chrono::minutes get_time() const;
    int get_id() const;
    std::vector<std::string> get_args() const;

    //void handle();

private:
    std::chrono::minutes time_;
    int id_;
    std::vector<std::string> args_;
};


