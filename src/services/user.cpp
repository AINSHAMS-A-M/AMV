#include <string>

#include "data_structures.hpp"
#include "db.hpp"
#include "services.hpp"


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
