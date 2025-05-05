#include <iostream>
#include <cctype>

#include "server.hpp"
#include "utils.hpp"
#include "parser.hpp"

Server::Server(char* path) {
    path_ = path;
    if (path_.is_relative()) {
        path_ = std::filesystem::absolute(path_);
    }

    Parser p(std::move(cfg_), path_);
    cfg_ = p.parse();

    state_.price_per_hour_ = cfg_.price_per_hour_;
    state_.table_number_ = cfg_.table_number_;
    state_.start_ = cfg_.start_;
    state_.end_ = cfg_.end_;

    for (int i = 0; i < state_.table_number_ + 1; i++) {
        state_.tables_[i] = State::TableData(); 
    }
}

//void Server::handle_event(const Event& e) {
//    std::cout << util::time_to_string(e.get_time()) << " " << e.get_id();
//    for (const auto& arg: e.get_args()) {
//        std::cout << " " << arg; 
//    }
//    std::cout << std::endl;
//
//    switch (e.get_id()) {
//        case 1: 
//            if (pool_.contains(e.get_args()[0])) {
//                handle_event(Event(util::time_to_string(e.get_time()), 13, {"YouShallNotPass"}));
//                return;
//            }
//            if (conf_.start_ > e.get_time()) {
//                handle_event(Event(util::time_to_string(e.get_time()), 13, {"NotOpenYet"}));
//                return;
//            }
//            pool_[e.get_args()[0]] = util::time_to_string(std::chrono::minutes(0));
//            return;
//        case 2: 
//            if (tables_[std::stol(e.get_args()[1])] != "") {
//                handle_event(Event(util::time_to_string(e.get_time()), 13, {"PlaceIsBusy"}));
//                return;
//            }
//            if (!pool_.contains(e.get_args()[0])) {
//                handle_event(Event(util::time_to_string(e.get_time()), 13, {"ClientUnknown"}));
//                return;
//            }
//            for (int i = 1; i < tables_.size(); i++) { // not optimal
//                if (tables_[i] == e.get_args()[0]) {
//                    tables_[i] = ""; 
//                }
//            }
//
//            tables_[std::stol(e.get_args()[1])] = e.get_args()[0];
//            return;
//        case 3: 
//            for (int i = 1; i < tables_.size(); i++) { // not optimal
//                if (tables_[i] == "") {
//                    handle_event(Event(util::time_to_string(e.get_time()), 13, {"ICanWaitNoLonger"}));
//                    return;
//                }
//            }
//            if (queue_.size() + 1 > conf_.table_number_) {                                                // !!!!
//                handle_event(Event(util::time_to_string(e.get_time()), 11, std::move(e.get_args())));
//                return;
//            }
//            queue_.push(e.get_args()[0]);
//            return;
//        case 4: 
//            if (!pool_.contains(e.get_args()[0])) {
//                handle_event(Event(util::time_to_string(e.get_time()), 13, {"ClientUnknown"}));
//                return;
//            }
//            
//            for (int i = 1; i < tables_.size(); i++) {
//                if (tables_[i] == e.get_args()[0]) {
//                    if (queue_.size() > 0) {
//                        auto args = e.get_args();
//                        args.push_back(std::to_string(i));
//                        pool_.erase(tables_[i]);
//                        tables_[i] = "";
//                        handle_event(Event(time_to_string(e.get_time()), 12, std::move(args)));
//                    } else {
//                        pool_.erase(tables_[i]);
//                        tables_[i] = "";
//                    }
//                    return;
//                }
//            }
//            return;
//        case 11: 
//            pool_.erase(e.get_args()[0]); 
//            return;
//        case 12: 
//            tables_[std::stol(e.get_args()[1])] = queue_.front(); 
//            queue_.pop();
//            return;
//        case 13: 
//            return;
//        default:
//            return;
//    }
//}

void Server::run() {
    std::cout << util::time_to_string(state_.start_) << std::endl;
    
    for (int i = 0; i < cfg_.events_.size(); i++) {
        cfg_.events_[i]->handle(state_); 
    }

    // TODO: rest
    
    std::vector<std::string> to_be_sorted(state_.pool_.begin(), state_.pool_.end());
    std::sort(to_be_sorted.begin(), to_be_sorted.end());

    for (const auto& client: to_be_sorted) {
        EventFactory::create(util::time_to_string(state_.end_), 11, client)->handle(state_);
    }

    std::cout << util::time_to_string(state_.end_) << std::endl;
}

