#include <string>
#include "services.hpp"
#include "structs.hpp"
#include "hash.cpp"
#include "..\data_structures.hpp"
#include "..\db\structs.hpp"

/// @brief Creates a new user with the provided details and stores them in the system.
void create_user(CreateUser createUser)
{
}

/// @brief Edits the details of an existing user in the system.
void edit_user(EditUser editUser , MeshVector<User> &users)
{
    bool found = 0;
    for(int client = 0 ; client < users.size() ; client++)
    {
        if(editUser.id == users[client].id && editUser.name == users[client].name)
        {
            if(editUser.hashed_password == users[client].hashed_password)
            {
                found = 1;
                std:: string now_password_hashed = hash_password(editUser.now_password , editUser.id);
                users[client].hashed_password = now_password_hashed;
                break;
            }
            
        }
    }
    if(!found)
    {
        throw std:: invalid_argument("unnable to find the user :( ");
    }
}

/// @brief Edits the password for an existing user in the system.
void edit_user_password(EditUserPassword editUserPassword)
{
}
