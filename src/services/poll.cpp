#include "data_structures.hpp"
#include "db.hpp"
#include "services.hpp"

/// @brief Creates a new poll.
/// This function initializes and stores a new poll with its associated options and metadata.
void create_poll(CreatePoll createPoll)
{
}

/// @brief Creates a new user poll, i.e. makes a user vote in the poll.

// Check if the user has voted or not
std::pair<bool, size_t> check_user_vote(int user_id, int poll_id)
{
    // Search for the user vote in UserPolls
    std::pair<bool, size_t> checkData = {0, -1};

    for (size_t i = 0; i < userVotes.size(); i++)
    {
        if (userVotes[i].poll_id == poll_id && userVotes[i].user_id == user_id)
        {
            checkData.first = true;
            checkData.second = i;
            break;
        }
    }
    return checkData;
}

/// Make sure the user hadn't voted before.
void create_user_vote(int user_id, int poll_id, int poll_option_id)
{
    // Check if the poll option is voted already
    auto checkData = check_user_vote(user_id, poll_id);

    if (!checkData.first)
    {
        // Create a new UserVote & fill it with formal parameters.
        UserVote UserChoiceData;
        size_t newid;

        // Check if the userpolls not empty
        if (!userVotes.size())
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
    else
        throw "You Have already voted";
}

void delete_user_vote(int user_id, int poll_id)
{
    // get the userpoll id
    auto checkData = check_user_vote(user_id, poll_id);
    if (checkData.first)
    {
        // Erase the user vote by userpoll id
        // userVotes.erase(userVotes.begin() + checkData.second);
    }
    else
        // If !found throws an error (100% invalid function call).
        throw std::invalid_argument("User Vote is not Found");
}

/// @brief Retrieves a public poll for general viewing.
/// This function returns basic poll information without revealing user-specific or private data.
RetrievePollDTO retrieve_public_poll(int poll_id)
{
    // Find the poll with the given poll_id
    Poll foundPoll;
    bool pollFound = false;
    for (const auto &p : polls)
    {
        if (p.id == poll_id) // Ensure matching type (poll_id is string)
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
RetrievePollResultAdmin retrieve_poll_as_owner(const int user_id, const int poll_id)
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

MeshVector<UserVote> retrieve_poll_for_user(int user_id)
{
    MeshVector<UserVote> votes_with_the_user_id;
    for (auto &v : userVotes)
    {
        if (user_id == v.user_id)
        {
            votes_with_the_user_id.push_back(v);
        }
    }
    return votes_with_the_user_id;
}


/// @brief Retrieves the last 10 polls the user has participated in.
/// @param user_id The ID of the user.
/// @return A list of up to 10 polls the user has voted in, including the selected option for each.
MeshVector<PollRead> retrieve_last_10_polls(int user_id)
{
    MeshVector<int> poll_id, option_id;

    for (long long last = userVotes.size() - 1; last >= 0; last--)
    {
        auto current = userVotes[last];
        if (current.user_id == user_id)
        {
            poll_id.push_back(current.poll_id);
            option_id.push_back(current.poll_option_id);

            if (poll_id.size() == 10)
            {
                break;
            }
        }
    }

    int number_of_polls = poll_id.size();

    if (number_of_polls == 0)
    {
        throw std::invalid_argument("There is no votes for this user :( ");
    }

    MeshVector<PollRead> last_polls(number_of_polls);
    for (int i = 0; i < number_of_polls; i++)
    {
        for (auto current : polls)
        {
            if (current.id == poll_id[i])
            {
                last_polls[i].poll = current;
                break;
            }
        }
        for (auto current : pollOptions)
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