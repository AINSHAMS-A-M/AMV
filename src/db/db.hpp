#pragma once

#include "_db_structs.hpp"
#include <vector>

extern MeshVector<User> users;
extern MeshVector<Poll> polls;
extern MeshVector<PollOption> pollOptions;
extern std::vector<UserVote> userVotes;
extern User activeUser;
void load_data();
void save_data();
