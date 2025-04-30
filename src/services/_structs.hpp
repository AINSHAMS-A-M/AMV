#pragma once

#include <string>
#include "db.hpp"

struct CreateUser
{
    size_t id;
    std::string name;
    std::string hashed_password;
    std::string username;
};

struct EditUser
{
    size_t user_id;
    std::string new_username;
    std::string new_real_name;
};

struct EditUserPassword
{
    size_t id;
    std::string hashed_password;
    std::string now_password;
};

struct CreatePoll
{
    std::string voter_id;

    std::string poll_name;

    std::string poll_desc;

    size_t owner_id;

    MeshVector<std::string> options;
};

// Service: Retrieve Poll - ADMIN, Service: Retrieve Poll - User, functions structs and returns:
struct RetrievePollResultAdmin
{
    struct Result
    {
        // holds a counter for a specific option in a vote:
        size_t poll_id;
        size_t option_id;
        std::string option_name;
        size_t option_votes_count;
    };

    Poll pollInfo;

    MeshVector<Result> results;
    bool success;
    std::string error_msg;
};

struct RetrievePollDTO
{
    std::string id;
    std::string name;
    std::string desc;
    std::string creation_date;
    MeshVector<PollOption> options; // Holds options for the public poll
};

struct PollRead
{
    Poll poll;
    PollOption option;
};
