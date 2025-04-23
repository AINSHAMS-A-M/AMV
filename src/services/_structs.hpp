#pragma once

#include <string>
#include "db.hpp"

struct CreateUser
{
    int id;
    std::string name;
    std::string hashed_password;
    std::string username;
};

struct EditUser
{
    int user_id;
    std::string new_username;
    std::string new_real_name;
};

struct EditUserPassword
{
    int id;
    std::string hashed_password;
    std::string now_password;
};

struct CreatePoll
{
    struct CreatePollOption
    {
        std::string name;
        std::string description;
    };

    std::string voter_id;

    std::string poll_name;

    int owner_id;

    MeshVector<CreatePollOption> options;
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
