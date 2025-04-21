#pragma once

#include "_db_structs.hpp"

extern MeshVector<User> users;
extern MeshVector<Poll> polls;
extern MeshVector<PollOption> pollOptions;
extern MeshVector<UserVote> userVotes;

void load_data();
void save_data();
