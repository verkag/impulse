#include <iostream>

#include "event.hpp"
#include "state.hpp"
#include "utils.hpp"

Event::Event(const std::string& str_time, int id) :
        time_(util::to_minutes(str_time)), id_(id) {}

std::chrono::minutes Event::get_time() const { 
    return time_;
}

int Event::get_id() const {
    return id_;
}

// client come
Event1::Event1(const std::string& str_time, int id, const std::string& client) : Event(str_time, id), client_(client) {}
void Event1::handle(State& s) {
    std::cout << util::time_to_string(get_time()) << " " << get_id() << " " << get_client() << std::endl;
    if (s.pool.contains(get_client())) {
        EventFactory::create(util::time_to_string(get_time()), 13, {"YouShallNotPass"})->handle(s);
        return;
    }
    if (s.start > get_time() || get_time() > s.end) {
        EventFactory::create(util::time_to_string(get_time()), 13, {"NotOpenYet"})->handle(s);
        return;
    }

    s.pool.insert(get_client());
    return;
}
std::string Event1::get_client() const {
    return client_;
}


// client sat 
Event2::Event2(const std::string& str_time, int id, const std::string& client, int table_number) : Event(str_time, id), client_(client), table_number_(table_number) {}
void Event2::handle(State& s) {
    std::cout << util::time_to_string(get_time()) << " " << get_id() << " " << get_client() << " " << get_table_number() << std::endl;
    if (s.tables[get_table_number()].current_owner != "") {
        EventFactory::create(util::time_to_string(get_time()), 13, "PlaceIsBusy")->handle(s);
        return;
    }
    if (!s.pool.contains(get_client())) {
        EventFactory::create(util::time_to_string(get_time()), 13, "ClientUnknown")->handle(s);
        return;
    }
    for (int i = 1; i < s.table_number + 1; i++) { // not optimal
        if (s.tables[i].current_owner == get_client()) {
            s.tables[i].current_owner = ""; 
        }
    }

    s.tables[get_table_number()].current_owner = get_client();
    s.session_manager.start_session(get_table_number(), get_time());
    return;
}
std::string Event2::get_client() const {
    return client_;
}
int Event2::get_table_number() const {
    return table_number_;
}


// client wait  
Event3::Event3(const std::string& str_time, int id, const std::string& client) : Event(str_time, id), client_(client) {}
void Event3::handle(State& s) {
    std::cout << util::time_to_string(get_time()) << " " << get_id() << " " << get_client() << std::endl;
    for (int i = 1; i < s.table_number + 1; i++) { // not optimal
        if (s.tables[i].current_owner == "") {
            EventFactory::create(util::time_to_string(get_time()), 13, "ICanWaitNoLonger")->handle(s);
            return;
        }
    }
    if (s.queue.size() + 1 > s.table_number) {                                                // !!!!
        EventFactory::create(util::time_to_string(get_time()), 11, get_client())->handle(s);
        return;
    }
    s.queue.push(get_client());
    return;
}
std::string Event3::get_client() const {
    return client_;
}


// client gone 
Event4::Event4(const std::string& str_time, int id, const std::string& client) : Event(str_time, id), client_(client) {}
void Event4::handle(State& s) {
    std::cout << util::time_to_string(get_time()) << " " << get_id() << " " << get_client() << std::endl;
    if (!s.pool.contains(get_client())) {
        EventFactory::create(util::time_to_string(get_time()), 13, "ClientUnknown")->handle(s);
        return;
    }

    for (int i = 1; i < s.table_number + 1; i++) {
        if (s.tables[i].current_owner == get_client()) {
            if (s.queue.size() > 0) {
                auto [salary, minutes] = s.session_manager.estimate(i, get_time());
                s.tables[i].total_salary += salary;
                s.tables[i].total_minutes += minutes;
                
                s.pool.erase(get_client());
                s.tables[i].current_owner = "";
                EventFactory::create(util::time_to_string(get_time()), 12, s.queue.front(), i)->handle(s);
            } else {
                auto [salary, minutes] = s.session_manager.estimate(i, get_time());
                s.tables[i].total_salary += salary;
                s.tables[i].total_minutes += minutes;

                s.pool.erase(s.tables[i].current_owner);
                s.tables[i].current_owner = "";
            }
            return;
        }
    }

    s.pool.erase(get_client());
    return;
}
std::string Event4::get_client() const {
    return client_;
}


// client gone 
Event11::Event11(const std::string& str_time, int id, const std::string& client) : Event(str_time, id), client_(client) {}
void Event11::handle(State& s) {
    std::cout << util::time_to_string(get_time()) << " " << get_id() << " " << get_client() << std::endl;
    
    for (int i = 1; i < s.table_number + 1; i++) {
        if (s.tables[i].current_owner == get_client()) {
            auto [salary, minutes] = s.session_manager.estimate(i, get_time());
            s.tables[i].total_salary += salary;
            s.tables[i].total_minutes += minutes;
            s.tables[i].current_owner = "";
        }
    }

    s.pool.erase(get_client()); 
    return;
}
std::string Event11::get_client() const {
    return client_;
}


// client sat
Event12::Event12(const std::string& str_time, int id, const std::string& client, int table_number) : Event(str_time, id), client_(client), table_number_(table_number) {}
void Event12::handle(State& s) {
    std::cout << util::time_to_string(get_time()) << " " << get_id() << " " << get_client() << " " << get_table_number() << std::endl;
    s.tables[get_table_number()].current_owner = s.queue.front(); 
    s.queue.pop();

    s.session_manager.start_session(get_table_number(), get_time());
    return;
}
std::string Event12::get_client() const {
    return client_;
}
int Event12::get_table_number() const {
    return table_number_;
}


// error 
Event13::Event13(const std::string& str_time, int id, const std::string& err_msg) : Event(str_time, id), err_msg_(err_msg) {}
void Event13::handle(State& s) {
    std::cout << util::time_to_string(get_time()) << " " << get_id() << " " << get_err_msg() << std::endl;
    return;
}
std::string Event13::get_err_msg() const {
    return err_msg_;
}


