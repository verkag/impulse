#include <iostream>
#include "server.hpp"


int main(int argc, char** argv) {
    if (argc == 2) {
        Server server(argv[1]); 
        server.run();
    } else {
        std::cerr << "incorrect usage, run: ./app path/to/test_file.txt" << std::endl;
    }
}
