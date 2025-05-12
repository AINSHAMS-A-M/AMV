#pragma once

#include "QDatetime"
#include <string>

struct User
{
	size_t id;

	std::string name;
	std::string email;
	std::string address;
	std::string phone_number;

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
	bool is_finished;
};

struct PollOption
{
	size_t id;

	size_t poll_id;
	std::string name;
};
