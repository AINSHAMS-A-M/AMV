#pragma once

#include "_db_structs.hpp"
#include "data_structures.hpp"

extern MeshVector<User> users;
extern MeshVector<Poll> polls;
extern MeshVector<PollOption> pollOptions;
extern MeshVector<UserVote> userVotes;
extern User activeUser;
void load_data();
void save_data();
