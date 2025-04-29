#include <string>
#include "_hash.hpp"
#include "data_structures.hpp"
#include "db.hpp"
#include "services.hpp"
#include <QMessageBox>

/// @brief logs the user into the system
std::string log_in(std::string username, std::string password)
{
    for (int user = 0; user < users.size(); user++)
    {
        if (users[user].username == username)
        {
            std::string hashed = hash_password(password,users[user].id);
            if (hashed == users[user].hashed_password)
            {
                return "true";
            }
            else
            {
                return "Incorrect Password, Try Again!";
            }
        }

    }
    return "User Not Found, Please check your credentials!";
}

/// @brief Creates a new user with the provided details and stores them in the system.
std::string create_user(CreateUser createUser)
{

    if (createUser.name.empty() || createUser.hashed_password.empty() || createUser.username.empty())
    {
        return "Em";
    }

    for (const auto& user : users)
    {
        if (user.id == createUser.id)
        {
            return "Already Exist";
        }

        if (user.username == createUser.username)
        {
            return "Already Exist";
        }
    }

    User newUser;
    newUser.id = createUser.id;
    newUser.username = createUser.username;
    newUser.name = createUser.name;
    newUser.hashed_password = createUser.hashed_password;

    users.push_back(newUser);
    return "Success";
}


/// @brief Retrieves a user by their ID.
User get_user_by_id(const size_t& id) {
    for (const auto& user : users) {
        if (user.id == id) {
            return user;
        }
    }
    throw std::invalid_argument("User not found!");
}

User get_id_by_user(const std::string& username) {
    for (const auto& user : users) {
        if (user.username == username) {
            return user;
        }
    }
    throw std::invalid_argument("User not found!");
}

/// @brief Edits the details of an existing user in the system.
std::string edit_user(EditUser editUser)
{
    for (int user = 0; user < users.size(); user++)
    {
        if (editUser.new_username == users[user].username && editUser.user_id != users[user].id)
        {
            return ("A user with the same username already exists!");
        }
    }
    for (int user = 0; user < users.size(); user++)
    {
        if (editUser.user_id == users[user].id)
        {
            users[user].username = editUser.new_username;
            users[user].name = editUser.new_real_name;
            return "done";
        }
    }
    return("Can't find user!");
}

/// @brief Edits the password for an existing user in the system.
std::string edit_user_password(EditUserPassword editUserPassword)
{
    bool found = 0;
    for (int client = 0; client < users.size(); client++)
    {
        if (editUserPassword.id == users[client].id)
        {
            if (editUserPassword.hashed_password == users[client].hashed_password)
            {
                found = 1;
                std::string now_password_hashed = hash_password(editUserPassword.now_password, editUserPassword.id);
                users[client].hashed_password = now_password_hashed;
                return "done";
            }
        }
    }
    if (!found)
    {
        return ("The Old Password is wrong, Please try again!");
    }
}
