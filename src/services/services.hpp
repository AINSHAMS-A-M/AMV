#pragma once

#include "_hash.hpp"
#include "_structs.hpp"

User get_user_by_id(const int& id);

User get_id_by_user(const std::string& username);

std::string create_user(CreateUser createUser);

Poll create_poll(CreatePoll createPoll);

void fixIds(int);

std::pair<bool, size_t> check_user_vote(std::string, std::string);

void create_user_vote(std::string, std::string, std::string);

void delete_user_vote(std::string, std::string, std::string);

std::pair<bool, size_t> check_user_vote(int user_id, int poll_id);

// void retrieve_poll(int voter_id);

MeshVector<PollRead> retrieve_last_10_polls(int user_id);

RetrievePollResultAdmin retrieve_poll_as_owner(int user_id, int poll_id);

MeshVector<UserVote> retrieve_poll_for_user(int user_id);

RetrievePollDTO retrieve_public_poll(int poll_id);

int get_new_id(MeshVector<UserVote>);

std::string log_in(std::string username, std::string password);
