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

    size_t owner_id;

    MeshVector<std::pair<std::string,std::string>> options;
};

// i.e. vote in a poll
struct CreateUserPoll
{
};

// Service: Retrieve Poll - ADMIN, Service: Retrieve Poll - User, functions structs and returns:
struct RetrievePollResultAdmin
{
    struct Result
    {
        // holds a counter for a specific option in a vote:
        int poll_id;
        int option_id;
        std::string option_name;
        int option_votes_count;
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
    std::string creation_date;
    MeshVector<PollOption> options; // Holds options for the public poll
};

struct PollRead
{
    Poll poll;
    PollOption option;
};
