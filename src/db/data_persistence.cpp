#include <fstream>
#include "data_structures.hpp"
#include "services.hpp"

MeshVector<User> users;
MeshVector<Poll> polls;
MeshVector<PollOption> pollOptions;
// MeshVector<UserPoll> userPolls;
MeshVector<UserVote> userVotes;

/// @brief Loads data from a CSV file and populates in-memory data structures.
void load_data()
{
    // get cwd
    std::string path = __FILE__;
    std::string CWD = path.substr(0, path.find("/db"));

    // load users
    std::fstream csv_in;
    csv_in.open(CWD + "/data/users.csv", std::ios::in);

    if (!csv_in.is_open())
    {
        std::cerr << "Error: Unable to connect to database.";
        return;
    }

    while (!csv_in.eof())
    {
        User current_user;
        csv_in >> current_user.id >> current_user.name >> current_user.username >> current_user.hashed_password;
        users.push_back(current_user);
    }

    csv_in.close();

    // load polls
    csv_in.open(CWD + "/data/polls.csv", std::ios::in);

    if (!csv_in.is_open())
    {
        std::cerr << "Error: Unable to connect to database.";
        return;
    }

    while (!csv_in.eof())
    {
        Poll current_poll;
        csv_in >> current_poll.id >> current_poll.name >> current_poll.voter_id >> current_poll.owner_id >> current_poll.created_at;
        polls.push_back(current_poll);
    }

    csv_in.close();

    // load pollOptions
    csv_in.open(CWD + "/data/pollOptions.csv", std::ios::in);

    if (!csv_in.is_open())
    {
        std::cerr << "Error: Unable to connect to database.";
        return;
    }

    while (!csv_in.eof())
    {
        PollOption current_poll_option;
        csv_in >> current_poll_option.id >> current_poll_option.poll_id >> current_poll_option.name >> current_poll_option.description;
        pollOptions.push_back(current_poll_option);
    }

    csv_in.close();

    // load UserVotes
    csv_in.open(CWD + "/data/userVotes.csv", std::ios::in);

    if (!csv_in.is_open())
    {
        std::cerr << "Error: Unable to connect to database.";
        return;
    }

    while (!csv_in.eof())
    {
        UserVote current_user_vote;
        csv_in >> current_user_vote.id >> current_user_vote.user_id >> current_user_vote.poll_id >> current_user_vote.poll_option_id;
        userVotes.push_back(current_user_vote);
    }

    csv_in.close();
}

/// @brief Saves current data to a CSV file for persistent storage.
void save_data()
{
    // get cwd
    std::string path = __FILE__;
    std::string CWD = path.substr(0, path.find("/db"));

    // save users
    std::fstream csv_out;
    csv_out.open(CWD + "/data/users.csv", std::ios::out);

    if (!csv_out.is_open())
    {
        std::cerr << "Error: Unable to connect to database.";
        return;
    }

    for (User current_user : users)
    {
        csv_out << current_user.id << current_user.name << current_user.username << current_user.hashed_password << '\n';
    }

    csv_out.close();

    // save polls
    csv_out.open(CWD + "/data/polls.csv", std::ios::out);

    if (!csv_out.is_open())
    {
        std::cerr << "Error: Unable to connect to database.";
        return;
    }

    for (Poll current_poll : polls)
    {
        csv_out << current_poll.id << current_poll.name << current_poll.voter_id << current_poll.owner_id << current_poll.created_at;
    }

    csv_out.close();

    // save pollOptions
    csv_out.open(CWD + "/data/pollOptions.csv", std::ios::out);

    if (!csv_out.is_open())
    {
        std::cerr << "Error: Unable to connect to database.";
        return;
    }

    for (PollOption current_poll_option : pollOptions)
    {
        csv_out << current_poll_option.id << current_poll_option.poll_id << current_poll_option.name << current_poll_option.description;
    }

    csv_out.close();

    // save UserVotes
    csv_out.open(CWD + "/data/userVotes.csv", std::ios::out);

    if (!csv_out.is_open())
    {
        std::cerr << "Error: Unable to connect to database.";
        return;
    }

    for (UserVote current_user_vote : userVotes)
    {
        csv_out << current_user_vote.id << current_user_vote.user_id << current_user_vote.poll_id << current_user_vote.poll_option_id;
    }

    csv_out.close();
}
