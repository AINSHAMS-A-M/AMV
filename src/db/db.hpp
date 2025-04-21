#pragma once
#include "structs.hpp"
#include "data_structures.hpp"

MeshVector<User> users;
MeshVector<Poll> polls;
MeshVector<PollOption> pollOptions;
MeshVector<UserPoll> userPolls;

void load_data();
void save_data();