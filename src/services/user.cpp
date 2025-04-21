#include <string>

#include "data_structures.hpp"
#include "db.hpp"
#include "services.hpp"

/// @brief Creates a new user with the provided details and stores them in the system.
void create_user(CreateUser createUser)
{
}

/// @brief Edits the details of an existing user in the system.
void edit_user(EditUser editUser)
{
    
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
