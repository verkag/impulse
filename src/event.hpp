#pragma once 

#include <string>
#include <chrono>

#include "utils.hpp"
#include "state.hpp"

class Event {
public:
    Event(const std::string& str_time, int id);

    std::chrono::minutes get_time() const;
    int get_id() const;

    virtual void handle(State& s) = 0;

protected:
    std::chrono::minutes time_;
    int id_;
};


class Event1 : public Event {
public:
    Event1(const std::string& str_time, int id, const std::string& client);
    void handle(State& s) override;
    std::string get_client() const;
protected:
    std::string client_;
};


class Event2 : public Event {
public:
    Event2(const std::string& str_time, int id, const std::string& client, int table_number);
    void handle(State& s) override;
    std::string get_client() const;
    int get_table_number() const;
protected:
    std::string client_;
    int table_number_;
};


class Event3 : public Event {
public:
    Event3(const std::string& str_time, int id, const std::string& client);
    void handle(State& s) override;
    std::string get_client() const;
protected:
    std::string client_;
};


class Event4 : public Event {
public:
    Event4(const std::string& str_time, int id, const std::string& client);
    void handle(State& s) override;
    std::string get_client() const;
protected:
    std::string client_;
};


class Event11 : public Event {
public:
    Event11(const std::string& str_time, int id, const std::string& client);
    void handle(State& s) override;
    std::string get_client() const;
protected:
    std::string client_;
};


class Event12 : public Event {
public:
    Event12(const std::string& str_time, int id, const std::string& client, int table_number);
    void handle(State& s) override;
    std::string get_client() const;
    int get_table_number() const;
protected:
    std::string client_;
    int table_number_;
};


class Event13 : public Event {
public:
    Event13(const std::string& str_time, int id, const std::string& err_msg);
    void handle(State& s) override;
    std::string get_err_msg() const;
protected:
    std::string err_msg_;
};

class EventFactory {
public: 
    static std::unique_ptr<Event> create(const std::string& str_time, int id, const std::string& str, int table_number = 0) {
        //std::cout << str_time << " " << id << " " << str << " " << table_number << std::endl;
        switch (id) {
            case 1: 
                if (table_number != 0) throw std::exception();
                if (!util::is_valid_name(str)) throw std::exception();
                return std::make_unique<Event1>(str_time, id, str);
            case 2: 
                if (table_number == 0) throw std::exception();
                if (!util::is_valid_name(str)) throw std::exception();
                return std::make_unique<Event2>(str_time, id, str, table_number);
            case 3: 
                if (table_number != 0) throw std::exception();
                if (!util::is_valid_name(str)) throw std::exception();
                return std::make_unique<Event3>(str_time, id, str);
            case 4: 
                if (table_number != 0) throw std::exception();
                if (!util::is_valid_name(str)) throw std::exception();
                return std::make_unique<Event4>(str_time, id, str);
            case 11: 
                if (table_number != 0) throw std::exception();
                if (!util::is_valid_name(str)) throw std::exception();
                return std::make_unique<Event11>(str_time, id, str);
            case 12: 
                if (table_number == 0) throw std::exception();
                if (!util::is_valid_name(str)) throw std::exception();
                return std::make_unique<Event12>(str_time, id, str, table_number);
            case 13: 
                if (table_number != 0) throw std::exception();
                return std::make_unique<Event13>(str_time, id, str);
            default: 
                throw std::exception();
        }
    }
};
