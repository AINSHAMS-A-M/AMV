#pragma once
#include "structs.hpp"

const int N = 1e5;

User users[N];
Poll polls[N];
PollOption pollOptions[N];
UserPoll userPolls[N];

void load_data();
void save_data();
