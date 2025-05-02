#include <vector>
#include <filesystem>
#include <chrono>

class Server {
public: 
    Server(char* path);
    void run();

private: 
    class Event {
    public:
        Event(const std::string& str_time, int id, std::vector<std::string>&& args) 
        : time_(to_minutes(str_time)), id_(id), args_(std::move(args)) {}

        int get_time() const {
            return time_.count();
        }

    private:
        std::chrono::minutes time_;
        int id_;
        std::vector<std::string> args_;
    };

    void parse();
    Event parse_line(const std::string& str);
    static std::chrono::minutes to_minutes(const std::string& str);

    std::filesystem::path path_;
    uint64_t table_number_;
    std::chrono::minutes start_;
    std::chrono::minutes end_;
    uint64_t price_per_hour_;
    std::vector<Event> events_;
};
