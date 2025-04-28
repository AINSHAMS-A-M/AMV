#pragma once

#include <string>
#include "data_structures.hpp"
#include <chrono>

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

    int user_id;
    int poll_id;
    int poll_option_id;
};

struct Poll
{
    size_t id;

    // id GIVEN to users/voters to allow them to vote in the poll
    std::string name;
    std::string voter_id;
    size_t owner_id;
    std::time_t created_at;
};

struct PollOption
{
    size_t id;

    size_t poll_id;
    std::string name;
    // used in GUI, a brief description of the option, can be an empty string.
    std::string description;
};
