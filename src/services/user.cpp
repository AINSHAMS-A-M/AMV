#include <string>
#include "_hash.hpp"
#include "data_structures.hpp"
#include "db.hpp"
#include "services.hpp"

/// @brief logs the user into the system
bool log_in(std::string username, std::string password)
{
    for (int user = 0; user < users.size(); user++)
    {
        if (users[user].username == username)
        {
            std::string hashed = hash_password(password,users[user].id);
            if (hashed == users[user].hashed_password)
            {
                return true;
            }
            else
            {
                std::cerr << "Wrong password!";
                return false;
            }
        }

    }
    std::cerr << "User not found!";
    return false;
}

/// @brief Creates a new user with the provided details and stores them in the system.
void create_user(CreateUser createUser)
{

    if (createUser.name.empty() || createUser.hashed_password.empty() || createUser.username.empty())
    {
        throw std::invalid_argument("ID, name, username, or password cannot be empty!");
    }

    for (const auto& user : users)
    {
        if (user.id == createUser.id)
        {
            throw std::invalid_argument("A user with the same ID already exists!");
        }

        if (user.username == createUser.username)
        {
            throw std::invalid_argument("A user with the same Username already exists!");
        }
    }

    User newUser;
    newUser.id = createUser.id;
    newUser.username = createUser.name;
    newUser.name = createUser.username;
    newUser.hashed_password = hash_password(createUser.hashed_password, createUser.id);

    users.push_back(newUser);
}


/// @brief Retrieves a user by their ID.
User get_user_by_id(const int& id) {
    for (const auto& user : users) {
        if (user.id == id) {
            return user;
        }
    }
    throw std::invalid_argument("User not found!");
}


/// @brief Edits the details of an existing user in the system.
void edit_user(EditUser editUser)
{
    for (int user = 0; user < users.size(); user++)
    {
        if (editUser.new_username == users[user].username && editUser.user_id != users[user].id)
        {
            throw std::invalid_argument("A user with the same username already exists!");
            return;
        }
    }
    for (int user = 0; user < users.size(); user++)
    {
        if (editUser.user_id == users[user].id)
        {
            users[user].username = editUser.new_username;
            users[user].name = editUser.new_real_name;
            return;
        }
    }
    throw std::invalid_argument("Can't find user!");
}

/// @brief Edits the password for an existing user in the system.
void edit_user_password(EditUserPassword editUserPassword)
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
                break;
            }
        }
    }
    if (!found)
    {
        throw std::invalid_argument("unnable to find the user :( ");
    }
}
