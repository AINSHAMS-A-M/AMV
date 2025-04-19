#include "services.hpp"

/// @brief Creates a new poll.
/// This function initializes and stores a new poll with its associated options and metadata.
void create_poll(CreatePoll createPoll)
{
}

/// @brief Creates a new user poll, i.e. makes a user vote in the poll.
/// Make sure the user hadn't voted before.
void create_user_poll(CreateUserPoll createUserPoll)
{
}

/// @brief Retrieves a public poll for general viewing.
/// This function returns basic poll information without revealing user-specific or private data.
void retrieve_poll(int poll_id)
{
}

/// @brief Retrieves detailed information about a poll owned by a specific user.
/// @param user_id The ID of the poll owner.
/// @param poll_id The ID of the poll to retrieve.
/// @return Full poll details including all options, metadata, and configuration settings.
void retrieve_poll_as_owner(int user_id, int poll_id)
{
}

/// @brief Retrieves all polls created by a specific user (admin/owner).
/// @param user_id The ID of the user whose polls should be fetched.
/// @return A list of all polls created by the user.
void retrive_polls_as_owner(int user_id)
{
}

/// @brief Retrieves the last 10 polls the user has participated in.
/// @param user_id The ID of the user.
/// @return A list of up to 10 polls the user has voted in, including the selected option for each.
void retrieve_last_10_polls(int user_id)
{
}
