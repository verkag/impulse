#pragma once 

#include <filesystem>

#include "state.hpp"
#include "config.hpp"


class Server {
public: 
    Server(char* path);
    void run();

private: 
    std::filesystem::path path_;
    Config cfg_;
    State state_; 
};

