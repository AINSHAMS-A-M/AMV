#pragma once

#include "_db_structs.hpp"

extern MeshVector<User> users;
extern MeshVector<Poll> polls;
extern MeshVector<PollOption> pollOptions;
extern MeshVector<UserPoll> userPolls;

void load_data();
void save_data();
