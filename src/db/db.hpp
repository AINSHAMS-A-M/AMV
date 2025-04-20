#pragma once
#include "structs.hpp"
#include "data_structures.hpp"

AMArray<User> users;
AMArray<Poll> polls;
AMArray<PollOption> pollOptions;
AMArray<UserPoll> userPolls;

void load_data();
void save_data();
