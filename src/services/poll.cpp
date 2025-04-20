#include "services.hpp"
#include "data_structures.hpp"
#include "structs.hpp"
#include "db.hpp"

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
RetrievePollResultAdmin retrieve_poll_as_owner(const std::string &user_id, const std::string &poll_id)
{
    RetrievePollResultAdmin result;
    result.success = false;
    result.error_msg = "";

    // 1) find the poll
    Poll foundPoll;
    bool pollFound = false;
    for (const auto &p : polls)
    {
        if (p.id == poll_id)
        {
            foundPoll = p;
            pollFound = true;
            break;
        }
    }
    if (!pollFound)
    {
        result.error_msg = "No such poll with that poll_id";
        return result;
    }

    // 2) check ownership
    if (foundPoll.owner_id != user_id)
    {
        result.error_msg = "This user_id is not the admin_id of the poll";
        return result;
    }

    // 3) prepare a zeroed Result entry for every option
    AMArray<RetrievePollResultAdmin::Result> tally;
    for (const auto &opt : pollOptions)
    {
        if (opt.poll_id == poll_id)
        {
            RetrievePollResultAdmin::Result r;
            r.poll_id = poll_id;
            r.option_id = opt.id;
            r.option_name = opt.name;
            r.option_votes_count = 0;
            tally.push_back(r);
        }
    }

    // 4) scan userPolls to count votes
    for (const auto &up : userPolls)
    {
        if (up.poll_id == poll_id)
        {
            // find the matching option in tally
            for (size_t i = 0; i < tally.size(); i++)
            {
                if (tally[i].option_id == up.poll_option_id)
                {
                    tally[i].option_votes_count++;
                    break;
                }
            }
        }
    }

    // 5) done!
    result.results = tally;
    result.success = true;

    result.pollInfo = foundPoll;

    return result;
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