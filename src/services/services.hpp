#pragma once

#include "_hash.hpp"
#include "_structs.hpp"

User get_id_by_user(const std::string& username);

std::string create_user(CreateUser createUser);

std::string edit_user(EditUser editUser);

std::string edit_user_password(EditUserPassword editUserPassword);

Poll create_poll(CreatePoll createPoll);

void delete_poll(size_t);

std::pair<bool, long long> check_user_vote(size_t user_id, size_t poll_id);

std::string create_user_vote(size_t user_id, size_t poll_id, size_t poll_option_id);

void delete_user_vote(size_t user_id, size_t poll_id);

MeshVector<PollRead> retrieve_polls(size_t user_id);

RetrievePollResultAdmin retrieve_poll_results(size_t poll_id);

RetrievePollDTO retrieve_public_poll(size_t poll_id);

std::string log_in(std::string username, std::string password);

std::string getPollId(std::string& voterId);

User get_user_by_id(const size_t& id);

MeshVector<size_t> show_created_polls(size_t user_id);

bool endPoll(size_t id);

void change_poll_options(size_t pollId, MeshVector<std::string>& newOptions);

PollVoters retrieve_poll_voters(size_t pollId);
