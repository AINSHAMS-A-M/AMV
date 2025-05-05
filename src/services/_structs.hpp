#pragma once

#include <string>
#include "db.hpp"

struct CreateUser
{
    size_t id;
    std::string name;
    std::string hashed_password;
    std::string username;
    std::string email;
    std::string phone_number;
    std::string address;
};

struct EditUser
{
    size_t user_id;
    std::string new_username;
    std::string new_real_name;
    std::string email;
    std::string phone_number;
    std::string address;
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
    size_t idx;
};

struct RetrievePollDTO
{
    size_t id;
    std::string name;
    std::string desc;
    QDateTime creation_date;
    MeshVector<PollOption> options; // Holds options for the public poll
    bool is_finished;
};

struct PollRead
{
    Poll poll;
    PollOption option;
};

struct voteData
{
    size_t pollId;
    size_t optionId;
    std::string pollTitle;
    std::string votedOption;
    std::string pollCreator;
};

typedef std::unordered_map <std::string,MeshVector<User>> PollVoters;
