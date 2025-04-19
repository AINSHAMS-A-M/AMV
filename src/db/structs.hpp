#pragma once

#include <string>

struct User
{
    std::string id;

    // the actual name
    std::string name;

    // used for login
    std::string username;
    std::string hashed_password;
};

struct UserPoll
{
    std::string id;
    std::string user_id;
    std::string poll_id;
    std::string poll_option;
};

struct Poll
{
    std::string id;

    // id GIVEN to users/voters to allow them to vote in the poll
    std::string voter_id;
    std::string owner_id;
    std::time_t name;
    int created_at;
};

struct PollOption
{
    std::string id;
    std::string poll_id;
    std::string name;
    // used in GUI, a brief description of the option, can be an empty string.
    std::string description;
};