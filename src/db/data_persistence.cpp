#include "data_structures.hpp"
#include "services.hpp"

MeshVector<User> users;
MeshVector<Poll> polls;
MeshVector<PollOption> pollOptions;
// MeshVector<UserPoll> userPolls;
MeshVector<UserVote> userVotes;

/// @brief Loads data from a CSV file and populates in-memory data structures.
void load_data() {}

/// @brief Saves current data to a CSV file for persistent storage.
void save_data() {}
