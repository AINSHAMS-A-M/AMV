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
