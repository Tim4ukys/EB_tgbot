#pragma once

#include <vector>
#include <map>

#include "Config.hpp"

class CoordsData {
public:
    struct coord {
        std::string name;
        float x, y, z;
    };
    using list_coord = std::vector<coord>;
    using MemberData = std::map<int64_t, list_coord>;
    using ChatsData = std::map<int64_t, MemberData>;

private:
    // MemberData m_membData;
    ChatsData m_chatsData;
    Config m_conf;

public:
    explicit CoordsData(const std::string& baseName) : m_conf(baseName) {
        for (json::iterator cit = m_conf.m_json.begin(); cit != m_conf.m_json.end(); ++cit) {
            for (json::iterator it = cit.value().begin(); it != cit.value().end(); ++it) {
                auto& vl = it.value();
                auto& data = m_chatsData[std::stoll(cit.key())][std::stoll(it.key())];
                data.resize(vl.size());
                for (size_t i{}; i < vl.size(); ++i) {
                    data[i].name = vl[i]["name"].get<std::string>();
                    data[i].x = vl[i]["coord"][0].get<float>();
                    data[i].y = vl[i]["coord"][1].get<float>();
                    data[i].z = vl[i]["coord"][2].get<float>();
                }
            }
        }
    }
    ~CoordsData() {
        auto& j = m_conf.m_json;
        j.clear();
        for (auto& [chatid, memb] : m_chatsData) {
            for (auto& [id, ls] : memb) {
                for (size_t i{}; i < ls.size(); ++i) {
                    j[std::to_string(chatid)][std::to_string(id)][i]["name"] = ls[i].name;
                    j[std::to_string(chatid)][std::to_string(id)][i]["coord"] = { ls[i].x, ls[i].y, ls[i].z };
                }
            }
        }
    }

    inline auto& operator[] (uint64_t chatId) {
        return m_chatsData[chatId];
    }

    inline auto& getMembData(int64_t chatId) {
        return m_chatsData[chatId];
    }
};