#pragma once
#include <string>

/// most of these are incomplete, please add fields as needed

struct CreateUser
{
};
struct EditUser
{
};
struct EditUserPassword
{
};
struct CreatePoll
{
    std::string voter_id;
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
        std::string poll_id;
        std::string option_id;
        std::string option_name;
        int option_votes_count;
    };

    Poll pollInfo;

    MeshVector<Result> results;
    bool success;
    std::string error_msg;
};

struct PublicPoll
{
    std::string id;
    std::string name;
    std::string creation_date;
    MeshVector<PollOption> options; // Holds options for the public poll
};