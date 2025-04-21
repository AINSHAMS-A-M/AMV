#pragma once

#include "structs.hpp"

void create_poll(CreatePoll createPoll);

void create_user_poll(std::string, std::string, std::string);

void delete_user_vote(std::string, std::string, std::string);

std::pair<bool, size_t> check_user_vote(std::string, std::string);

void retrieve_poll(int voter_id);

void retrive_polls_as_owner(int user_id);

void retrieve_last_10_polls(int user_id);

RetrievePollResultAdmin retrieve_poll_as_owner(int user_id, int poll_id);

MeshVector<UserPoll> retrieve_poll_as_user(std::string user_id);

RetrievePollDTO retrieve_public_poll(int poll_id);