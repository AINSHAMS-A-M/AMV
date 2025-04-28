#include <fstream>
#include <string>
#include <iostream>
#include "data_structures.hpp"
#include "services.hpp"

// Global data structures
MeshVector<User> users;
MeshVector<Poll> polls;
MeshVector<PollOption> pollOptions;
MeshVector<UserVote> userVotes;
User activeUser;

/// @brief Loads data from CSV files and populates in-memory data structures.
void load_data()
{
    std::string path = __FILE__;
    size_t db_pos = path.find("db");
    std::string CWD = ".";
    if (db_pos != std::string::npos)
    {
        // Check if "db" is followed by a path separator or is at the end
        if (db_pos + 2 >= path.length() || path[db_pos + 2] == '/' || path[db_pos + 2] == '\\')
        {
            CWD = path.substr(0, db_pos);
        }

    }

    std::string data_dir = CWD + "/data/"; // Construct data directory path

    // --- Load users ---
    std::fstream csv_in;
    std::string users_path = data_dir + "users.csv";
    csv_in.open(users_path, std::ios::in);

    if (!csv_in.is_open())
    {
        std::cerr << "Error: Unable to open user database file: " << users_path << std::endl;
        return;
    }

    User current_user;

    while (csv_in >> current_user.id >> current_user.name >> current_user.username >> current_user.hashed_password)
    {
        users.push_back(current_user);
    }
    // Check if the loop terminated due to an error other than EOF
    if (!csv_in.eof() && csv_in.fail()) {
        std::cerr << "Error reading users.csv. Check file format." << std::endl;
    }
    csv_in.close();

    // --- Load polls ---
    std::string polls_path = data_dir + "polls.csv";
    csv_in.open(polls_path, std::ios::in);

    if (!csv_in.is_open())
    {
        std::cerr << "Error: Unable to open poll database file: " << polls_path << std::endl;
        return;
    }

    Poll current_poll;
    while (csv_in >> current_poll.id >> current_poll.name >> current_poll.voter_id >> current_poll.owner_id >> current_poll.created_at)
    {
        polls.push_back(current_poll);
    }
    if (!csv_in.eof() && csv_in.fail()) {
        std::cerr << "Error reading polls.csv. Check file format." << std::endl;
    }
    csv_in.close();

    // --- Load pollOptions ---
    std::string pollOptions_path = data_dir + "pollOptions.csv";
    csv_in.open(pollOptions_path, std::ios::in);

    if (!csv_in.is_open())
    {
        std::cerr << "Error: Unable to open poll option database file: " << pollOptions_path << std::endl;
        return;
    }

    PollOption current_poll_option;
    while (csv_in >> current_poll_option.id >> current_poll_option.poll_id >> current_poll_option.name >> current_poll_option.description)
    {
        pollOptions.push_back(current_poll_option);
    }
    if (!csv_in.eof() && csv_in.fail()) {
        std::cerr << "Error reading pollOptions.csv. Check file format." << std::endl;
    }
    csv_in.close();

    // --- Load UserVotes ---
    std::string userVotes_path = data_dir + "userVotes.csv";
    csv_in.open(userVotes_path, std::ios::in);

    if (!csv_in.is_open())
    {
        std::cerr << "Error: Unable to open user vote database file: " << userVotes_path << std::endl;
        return;
    }

    UserVote current_user_vote;
    while (csv_in >> current_user_vote.id >> current_user_vote.user_id >> current_user_vote.poll_id >> current_user_vote.poll_option_id)
    {
        userVotes.push_back(current_user_vote);
    }
    if (!csv_in.eof() && csv_in.fail()) {
        std::cerr << "Error reading userVotes.csv. Check file format." << std::endl;
    }
    csv_in.close();
}

/// @brief Saves current data to CSV files for persistent storage.
void save_data()
{
    std::string path = __FILE__;
    size_t db_pos = path.find("db");
    std::string CWD = ".";

    if (db_pos != std::string::npos)
    {

        if (db_pos + 2 >= path.length() || path[db_pos + 2] == '/' || path[db_pos + 2] == '\\')
        {
            CWD = path.substr(0, db_pos);
        }

    }

    std::string data_dir = CWD + "/data/";

    // --- Save users ---
    std::fstream csv_out;
    std::string users_path = data_dir + "users.csv";
    // Open in output mode, truncating the file if it exists
    csv_out.open(users_path, std::ios::out | std::ios::trunc);

    if (!csv_out.is_open())
    {
        std::cerr << "Error: Unable to open user database file for writing: " << users_path << std::endl;
        return; // Or handle error appropriately
    }

    // Use const& to avoid copying objects
    for (const User& current_user : users)
    {
        // Assuming space-separated CSV and operator<< is defined or works by default
        // ADDED space delimiters and newline
        csv_out << current_user.id << ' '
                << current_user.name << ' '
                << current_user.username << ' '
                << current_user.hashed_password << '\n';
    }
    // Check for errors after writing loop
    if (csv_out.fail()) {
        std::cerr << "Error writing to users.csv." << std::endl;
    }
    csv_out.close();

    // --- Save polls ---
    std::string polls_path = data_dir + "polls.csv";
    csv_out.open(polls_path, std::ios::out | std::ios::trunc);

    if (!csv_out.is_open())
    {
        std::cerr << "Error: Unable to open poll database file for writing: " << polls_path << std::endl;
        return;
    }

    for (const Poll& current_poll : polls)
    {
        // ADDED space delimiters and newline
        csv_out << current_poll.id << ' '
                << current_poll.name << ' '
                << current_poll.voter_id << ' '
                << current_poll.owner_id << ' '
                << current_poll.created_at << '\n';
    }
    if (csv_out.fail()) {
        std::cerr << "Error writing to polls.csv." << std::endl;
    }
    csv_out.close();

    // --- Save pollOptions ---
    std::string pollOptions_path = data_dir + "pollOptions.csv";
    csv_out.open(pollOptions_path, std::ios::out | std::ios::trunc);

    if (!csv_out.is_open())
    {
        std::cerr << "Error: Unable to open poll option database file for writing: " << pollOptions_path << std::endl;
        return;
    }

    for (const PollOption& current_poll_option : pollOptions)
    {
        // ADDED space delimiters and newline
        csv_out << current_poll_option.id << ' '
                << current_poll_option.poll_id << ' '
                << current_poll_option.name << ' '
                << current_poll_option.description << '\n'; // Added newline
    }
    if (csv_out.fail()) {
        std::cerr << "Error writing to pollOptions.csv." << std::endl;
    }
    csv_out.close();

    // --- Save UserVotes ---
    std::string userVotes_path = data_dir + "userVotes.csv";
    csv_out.open(userVotes_path, std::ios::out | std::ios::trunc);

    if (!csv_out.is_open())
    {
        std::cerr << "Error: Unable to open user vote database file for writing: " << userVotes_path << std::endl;
        return;
    }

    for (const UserVote& current_user_vote : userVotes)
    {
        // ADDED space delimiters and newline
        csv_out << current_user_vote.id << ' '
                << current_user_vote.user_id << ' '
                << current_user_vote.poll_id << ' '
                << current_user_vote.poll_option_id << '\n'; // Added newline
    }
    if (csv_out.fail()) {
        std::cerr << "Error writing to userVotes.csv." << std::endl;
    }
    csv_out.close();
}
