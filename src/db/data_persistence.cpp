#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "data_structures.hpp"
#include "services.hpp"
#include <QApplication>
#include <QDir>
#include <QString>
#include <QDatetime>

MeshVector<User> users;
MeshVector<Poll> polls;
MeshVector<PollOption> pollOptions;
MeshVector<UserVote> userVotes;
User activeUser;

/// @brief Loads data from CSV files and populates in-memory data structures.
void load_data()
{
    QString qDataDir = QApplication::applicationDirPath() + "/data";
    QDir().mkpath(qDataDir);

    std::string data_dir = qDataDir.toStdString() + '/';

    // --- Load users ---
    std::fstream csv_in;
    std::string users_path = data_dir + "users.csv";
    csv_in.open(users_path, std::ios::in);

    if (!csv_in.is_open())
    {
        std::cerr << "Error: Unable to open user database file: " << users_path << std::endl;
        return;
    }

    std::string line;

    // Read header line if it exists (and discard)
    // std::getline(csv_in, line);
    while (std::getline(csv_in, line))
    {
        std::stringstream ss(line);
        std::string segment;
        User current_user;

        try {
            // Read id
            if (std::getline(ss, segment, '`')) current_user.id = std::stoi(segment); else throw std::runtime_error("Missing user id");
            // Read name (can contain spaces)
            if (std::getline(ss, segment, '`')) current_user.name = segment; else throw std::runtime_error("Missing user name");
            // Read username (can contain spaces)
            if (std::getline(ss, segment, '`')) current_user.username = segment; else throw std::runtime_error("Missing user username");
            // Read hashed_password (can contain spaces)
            if (std::getline(ss, segment, '`')) current_user.hashed_password = segment; else throw std::runtime_error("Missing user hashed_password");
            // Read email (can contain spaces)
            if (std::getline(ss, segment, '`')) current_user.email = segment; else throw std::runtime_error("Missing user hashed_password");
            // Read address (can contain spaces)
            if (std::getline(ss, segment, '`')) current_user.address = segment; else throw std::runtime_error("Missing user address");
            // Read phone_numver (can contain spaces)
            if (std::getline(ss, segment, '`')) current_user.phone_number = segment; else throw std::runtime_error("Missing user phone_number");

            users.push_back(current_user);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing users.csv line: '" << line << "' - " << e.what() << std::endl;
            // Decide whether to skip the line or stop loading
        }
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

    // Read header line if it exists (and discard)
    // std::getline(csv_in, line);

    while (std::getline(csv_in, line))
    {
        std::stringstream ss(line);
        std::string segment;
        Poll current_poll;

        try {
            // Read id
            if (std::getline(ss, segment, '`')) current_poll.id = std::stoi(segment); else throw std::runtime_error("Missing poll id");
            // Read name (can contain spaces)
            if (std::getline(ss, segment, '`')) current_poll.name = segment; else throw std::runtime_error("Missing poll name");
            // Read description
            if (std::getline(ss, segment, '`')) current_poll.desc = segment; else throw std::runtime_error("Missing poll description");
            // Read voter_id
            if (std::getline(ss, segment, '`')) current_poll.voter_id = segment; else throw std::runtime_error("Missing poll voter_id");
            // Read owner_id
            if (std::getline(ss, segment, '`')) current_poll.owner_id = std::stoi(segment); else throw std::runtime_error("Missing poll owner_id");

            // Read created_at (can contain spaces)
            if (std::getline(ss, segment, '`')) current_poll.is_finished = (std::stoi(segment)==1); else throw std::runtime_error("Missing poll created_at");

            // Read created_at (can contain spaces)
            if (std::getline(ss, segment, '`')) current_poll.created_at = QDateTime::fromString(QString::fromStdString(segment),"yyyy-MM-dd"); else throw std::runtime_error("Missing poll created_at");

            polls.push_back(current_poll);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing polls.csv line: '" << line << "' - " << e.what() << std::endl;

        }
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

    // Read header line if it exists (and discard)
    // std::getline(csv_in, line);

    while (std::getline(csv_in, line))
    {
        std::stringstream ss(line);
        std::string segment;
        PollOption current_poll_option;

        try {
            // Read id
            if (std::getline(ss, segment, '`')) current_poll_option.id = std::stoi(segment); else throw std::runtime_error("Missing poll option id");
            // Read poll_id
            if (std::getline(ss, segment, '`')) current_poll_option.poll_id = std::stoi(segment); else throw std::runtime_error("Missing poll option poll_id");
            // Read name (can contain spaces)
            if (std::getline(ss, segment, '`')) current_poll_option.name = segment; else throw std::runtime_error("Missing poll option name");

            pollOptions.push_back(current_poll_option);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing pollOptions.csv line: '" << line << "' - " << e.what() << std::endl;
            // Decide whether to skip the line or stop loading
        }
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

    // Read header line if it exists (and discard)
    // std::getline(csv_in, line);

    while (std::getline(csv_in, line))
    {
        std::stringstream ss(line);
        std::string segment;
        UserVote current_user_vote;

        try {
            // Read id
            if (std::getline(ss, segment, '`')) current_user_vote.id = std::stoi(segment); else throw std::runtime_error("Missing user vote id");
            // Read user_id
            if (std::getline(ss, segment, '`')) current_user_vote.user_id = std::stoi(segment); else throw std::runtime_error("Missing user vote user_id");
            // Read poll_id
            if (std::getline(ss, segment, '`')) current_user_vote.poll_id = std::stoi(segment); else throw std::runtime_error("Missing user vote poll_id");
            // Read poll_option_id
            if (std::getline(ss, segment, '`')) current_user_vote.poll_option_id = std::stoi(segment); else throw std::runtime_error("Missing user vote poll_option_id");

            userVotes.push_back(current_user_vote);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing userVotes.csv line: '" << line << "' - " << e.what() << std::endl;
            // Decide whether to skip the line or stop loading
        }
    }
    csv_in.close();
}

/// @brief Saves current data to CSV files for persistent storage.
void save_data()
{
    QString qDataDir = QApplication::applicationDirPath() + "/data";
    QDir().mkpath(qDataDir);

    std::string data_dir = qDataDir.toStdString() + '/';

    // --- Save users ---
    std::fstream csv_out;
    std::string users_path = data_dir + "users.csv";
    // Open in output mode, truncating the file if it exists
    csv_out.open(users_path, std::ios::out | std::ios::trunc);

    if (!csv_out.is_open())
    {
        std::cerr << "Error: Unable to open user database file for writing: " << users_path << std::endl;
        return;
    }

    // Write users data, comma-delimited
    for (const User& current_user : users)
    {
        csv_out << current_user.id << '`'
                << current_user.name << '`'
                << current_user.username << '`'
                << current_user.hashed_password << '`'
                << current_user.email << '`'
                << current_user.address << '`'
                << current_user.phone_number << '\n';
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
        csv_out << current_poll.id << '`'
                << current_poll.name << '`'
                << current_poll.desc << '`'
                << current_poll.voter_id << '`'
                << current_poll.owner_id << '`'
                << (current_poll.is_finished ? "1" : "0") << '`'
                << current_poll.created_at.toString("yyyy-MM-dd").toStdString() << '\n';
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

    // Write poll options data, comma-delimited
    for (const PollOption& current_poll_option : pollOptions)
    {
        csv_out << current_poll_option.id << '`'
                << current_poll_option.poll_id << '`'
                << current_poll_option.name << '\n';
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

    // Write user votes data, comma-delimited
    for (const UserVote& current_user_vote : userVotes)
    {
        csv_out << current_user_vote.id << '`'
                << current_user_vote.user_id << '`'
                << current_user_vote.poll_id << '`'
                << current_user_vote.poll_option_id << '\n';
    }
    if (csv_out.fail()) {
        std::cerr << "Error writing to userVotes.csv." << std::endl;
    }
    csv_out.close();
}
