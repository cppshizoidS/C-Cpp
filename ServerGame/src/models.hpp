#pragma once

#include <string>


struct user
{
    int64_t id;
    std::string name;
    std::string password;
};

struct user_info
{
    int64_t user_id;
    int64_t created;
    int64_t last_seen;
    int64_t rating;
};

struct game
{
    int64_t id;
    int64_t min_members;
    int64_t max_members;
    std::string name;
    std::string desc;
};

struct room
{
    int64_t game_id;
    int64_t owner_id;
    std::string member_ids;
};

struct session
{
    int64_t game_id;
    int64_t time_start;
    int64_t time_end;
};

