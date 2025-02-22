#pragma once

#include "processable.h"
#include <jsoncpp/json/json.h>
#include <map>
#include <set>

class img : public processable {
private:
    std::map<std::string, int64_t> images;
    std::map<int64_t, bool> is_adding;
    std::map<int64_t, std::string> add_name;

    std::map<int64_t, bool> is_deling;
    std::map<int64_t, std::string> del_name;

    std::map<int64_t, Json::Value> belongs;
    std::set<std::string> default_img;

public:
    img();
    void save();
    void del_all(std::string name);
    void del_single(std::string name, int index);
    int add_image(std::string name, std::string image, int64_t group_id);
    void belong_to(std::string name, int64_t group_id);
    std::string commands(std::string message, const msg_meta &conf);
    void process(std::string message, const msg_meta &conf);
    bool check(std::string message, const msg_meta &conf);
    std::string help();
};
