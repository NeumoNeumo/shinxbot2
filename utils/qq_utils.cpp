#include "utils.h"
#include <filesystem>

bool is_folder_exist(const int64_t &group_id, const std::string &path){
    Json::Value J;
    J["group_id"] = group_id;
    J = string_to_json(cq_send("get_group_root_files", J))["data"]["folders"];
    Json::ArrayIndex sz = J.size();
    for(Json::ArrayIndex i=0;i<sz;i++){
        if(J[i]["folder_name"].asString() == path){
            return true;
        }
    }
    return false;
}

std::string get_folder_id(const int64_t &group_id, const std::string &path){
    Json::Value J;
    J["group_id"] = group_id;
    J = string_to_json(cq_send("get_group_root_files", J))["data"]["folders"];
    Json::ArrayIndex sz = J.size();
    for(Json::ArrayIndex i=0;i<sz;i++){
        if(J[i]["folder_name"].asString() == path){
            return J[i]["folder_id"].asString();
        }
    }
    return "/";
}

/**
 * file: reletive path
*/
void upload_file(const std::filesystem::path &file, const int64_t &group_id, const std::string &path){
    try{
        if(!is_folder_exist(group_id, path) && is_group_op(group_id, get_botqq())){
            Json::Value J;
            J["group_id"] = group_id;
            J["name"] = path;
            J = string_to_json(cq_send("create_group_file_folder", J));
        } // Cannot use?
        std::string id = get_folder_id(group_id, path);
        Json::Value J;
        J["group_id"] = group_id;
        J["file"] = (std::filesystem::current_path() / file).lexically_normal().string();
        J["name"] = file.filename().string();
        J["folder"] = id;
        // cq_send(J.toStyledString(), "group", -1, group_id);
        J = string_to_json(cq_send("upload_group_file", J));
        if(J.isMember("msg")){
            cq_send(J.toStyledString(), "group", -1, group_id);
        }
    } catch (...) {
        setlog(LOG::WARNING, "File upload failed.");
        cq_send("File upload failed.", "group", -1, group_id);
    }
}

bool is_group_op(const int64_t &group_id, const int64_t &user_id){
    Json::Value J;
    J["group_id"] = group_id;
    J["user_id"] = user_id;
    J = string_to_json(cq_send("get_group_member_info", J))["data"];
    return J["role"].asString() != "member";
}