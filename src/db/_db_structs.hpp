#pragma once

#include <string>
#include "QDatetime"

struct User
{
    size_t id;

    // the actual name
    std::string name;

    // used for login
    std::string username;
    std::string hashed_password;
};

struct UserVote
{
    size_t id;

    size_t user_id;
    size_t poll_id;
    size_t poll_option_id;
};

struct Poll
{
    size_t id;

    std::string name;
    std::string desc;
    std::string voter_id;
    size_t owner_id;
    QDateTime created_at;
};

struct PollOption
{
    size_t id;

    size_t poll_id;
    std::string name;
};
