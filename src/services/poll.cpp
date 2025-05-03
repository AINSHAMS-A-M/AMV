#include "data_structures.hpp"
#include "db.hpp"
#include "services.hpp"

/// @brief Creates a new poll.
/// This function initializes and stores a new poll with its associated options and metadata.
Poll create_poll(CreatePoll createPoll)
{
    size_t poll_id = polls.size();
    Poll new_poll =
        {
            poll_id,
            createPoll.poll_name,
            createPoll.poll_desc,
            createPoll.voter_id,
            createPoll.owner_id,
            QDateTime::currentDateTime(),
            false,
        };
    polls.push_back(new_poll);

    size_t poll_option_id = pollOptions.size();
    for (auto &option : createPoll.options)
    {
        PollOption new_option =
            {
                poll_option_id,
                poll_id,
                option};
        pollOptions.push_back(new_option);
        poll_option_id++;
    }

    return new_poll;
}

// Check if the user has voted or not
std::pair<bool, size_t> check_user_vote(size_t user_id, size_t poll_id)
{

    for (size_t i = 0; i < userVotes.size(); i++)
    {
        if (userVotes[i].poll_id == poll_id && userVotes[i].user_id == user_id)
        {
            return {true, i};
        }
    }
    return {false, -1};
}

/// @brief Makes a user vote in the poll.
std::string create_user_vote(size_t user_id, size_t poll_id, size_t poll_option_id)
{
    Poll cur_poll;
    for (auto poll: polls) {
        if (poll.id == poll_id){
            cur_poll = poll;
            break;
        }
     }

    if (cur_poll.owner_id == user_id) {
         return "You can't vote in your own vote.";
    }

    if (cur_poll.is_finished) {
        return "You can't vote in a finished poll.";
    }


    // Create a new UserVote & fill it with formal parameters.
    UserVote UserChoiceData;
    size_t newid;

    // Check if the userpolls not empty
    if (userVotes.size())
    {
        newid = userVotes[userVotes.size() - 1].id + 1;
    }
    else
    {
        newid = 0;
    }

    // Fill the UserChoiceData
    UserChoiceData.id = newid;
    UserChoiceData.poll_id = poll_id;
    UserChoiceData.poll_option_id = poll_option_id;
    UserChoiceData.user_id = user_id;

    // Push the uservote to the UserPolls.
    userVotes.push_back(UserChoiceData);
}

void delete_user_vote(size_t user_id, size_t poll_id)
{
    // get the userpoll id
    auto checkData = check_user_vote(user_id, poll_id);
    if (checkData.first)
    {
        // Erase the user vote by userpoll id
        userVotes.erase(checkData.second);
    }
    else
        // If !found throws an error (100% invalid function call).
        throw std::invalid_argument("User Vote is not Found");
}

/// @brief Retrieves a public poll for general viewing.
/// This function returns basic poll information without revealing user-specific or private data.
RetrievePollDTO retrieve_public_poll(size_t poll_id)
{
    // Find the poll with the given poll_id
    Poll foundPoll;
    bool pollFound = false;
    for (const auto &p : polls)
    {
        if (p.id == poll_id) // Ensure matching type
        {
            foundPoll = p;
            pollFound = true;
            break;
        }
    }

    // If the poll is not found, return an empty RetrievePollDTO or handle it as needed
    if (!pollFound)
    {
        return RetrievePollDTO(); // Return an empty RetrievePollDTO object
    }

    // Create a RetrievePollDTO object to return
    RetrievePollDTO publicPoll;
    publicPoll.id = foundPoll.id;
    publicPoll.name = foundPoll.name;
    publicPoll.creation_date = foundPoll.created_at;
    publicPoll.desc = foundPoll.desc;

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
RetrievePollResultAdmin retrieve_poll_results(const size_t user_id, const size_t poll_id)
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
    MeshVector<RetrievePollResultAdmin::Result> tally;
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

    // 4) scan userVotes to count votes
    for (const auto &up : userVotes)
    {
        if (up.poll_id == poll_id)
        {
            // find the matching option in tally
            for (long long i = 0; i < tally.size(); i++)
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

/// @brief Retrieves the polls the user has participated in.
/// @param user_id The ID of the user.
/// @return A list of  polls the user has voted in, including the selected option for each.
MeshVector<PollRead> retrieve_polls(size_t user_id)
{
    MeshVector<size_t> poll_id, option_id;

    for (long long last = ((long long)userVotes.size()) - 1; last >= 0; last--)
    {
        auto current = userVotes[last];
        if (current.user_id == user_id)
        {
            poll_id.push_back(current.poll_id);
            option_id.push_back(current.poll_option_id);
        }
    }

    size_t number_of_polls = poll_id.size();

    MeshVector<PollRead> last_polls(number_of_polls);
    for (size_t i = 0; i < number_of_polls; i++)
    {
        for (auto &current : polls)
        {
            if (current.id == poll_id[i])
            {
                last_polls[i].poll = current;
                break;
            }
        }
        for (auto &current : pollOptions)
        {
            if (current.id == option_id[i])
            {
                last_polls[i].option = current;
                break;
            }
        }
    }

    return last_polls;
}

std::string getPollId(std::string &voterId)
{
    for (size_t i = 0; i < polls.size(); i++)
    {
        // find the poll id
        if (polls[i].voter_id == voterId)
        {
            return std::to_string(polls[i].id);
        }
    }
    return "f";
}

size_t getPollOptionId(size_t pollId)
{
    for (auto &option : pollOptions)
    {
        if (option.poll_id == pollId)
        {
            return option.id;
        }
    }
}

MeshVector<size_t> show_created_polls(size_t user_id)
{
    MeshVector<size_t> poll_id;

    for (long long last = polls.size() - 1; last >= 0; last--)
    {
        if (polls[last].owner_id == user_id)
        {
            poll_id.push_back(polls[last].id);
        }
    }
    return poll_id;
}

// delete poll and all its related options

void delete_poll(size_t pollID)
{
    // erase poll with id
    for (size_t i = 0; i < polls.size(); i++)
    {
        if (polls[i].id == pollID)
        {
            polls.erase(i);
            break;
        }
    }

    // erase all poll options related to the poll
    for (size_t i = 0; i < pollOptions.size(); i++)
    {
        if (pollOptions[i].poll_id == pollID)
        {
            pollOptions.erase(i);
            // decrease index by one in order to not skip the next element
            i--;
        }
    }

    // erase all votes related to the poll
    for (size_t i = 0; i < userVotes.size(); i++)
    {
        if (userVotes[i].poll_id == pollID)
        {
            userVotes.erase(i);
            i--;
        }
    }
}
