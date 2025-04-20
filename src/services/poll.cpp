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
PublicPoll retrieve_public_poll(int poll_id)
{
    // Find the poll with the given poll_id
    Poll foundPoll;
    bool pollFound = false;
    for (const auto &p : polls)
    {
        if (p.id == std::to_string(poll_id)) // Ensure matching type (poll_id is string)
        {
            foundPoll = p;
            pollFound = true;
            break;
        }
    }

    // If the poll is not found, return an empty PublicPoll or handle it as needed
    if (!pollFound)
    {
        return PublicPoll(); // Return an empty PublicPoll object
    }

    // Create a PublicPoll object to return
    PublicPoll publicPoll;
    publicPoll.id = foundPoll.id;
    publicPoll.name = foundPoll.name;
    publicPoll.creation_date = foundPoll.created_at;

    // Add options to the public poll (without revealing any user-specific data)
    for (const auto &option : pollOptions)
    {
        if (option.poll_id == foundPoll.id) // Correct comparison of poll_id to option.poll_id
        {
            publicPoll.options.push_back(option); // Add only poll options related to the poll
        }
    }

    return publicPoll;
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

AMArray<UserPoll> retrieve_poll_for_user(std::string user_id)
{
    AMArray<UserPoll> votes_with_the_user_id;
    for (auto &v : userPolls)
    {
        if (user_id == v.user_id)
        {
            votes_with_the_user_id.push_back(v);
        }
    }
    return votes_with_the_user_id;
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