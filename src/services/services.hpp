#pragma once

#include "structs.hpp"

void create_poll(CreatePoll createPoll);

void create_user_poll(CreateUserPoll createUserPoll);

void retrieve_poll(int voter_id);

void retrive_polls_as_owner(int user_id);

void retrieve_last_10_polls(int user_id);

RetrievePollResultAdmin retrieve_poll_as_owner(int user_id, int poll_id);

MeshVector<UserPoll> retrieve_poll_as_user(std::string user_id);

RetrievePollDTO retrieve_public_poll(int poll_id);