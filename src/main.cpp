#include <iostream>
#include <algorithm>

#include "services.hpp"
#include "data_structures.hpp"
#include "db.hpp"

int main()
{
    std::cout << "WELCOME TO AMV\n";
    // MeshVector<int> a;
    // a.push_back(2);
    // a.push_back(4);
    // a.push_back(5);
    // for (size_t i = 0; i < a.size(); ++i)
    // {
    //     std::cout << a[i] << ' ';
    // }
    // for (int x : a)
    // {
    //     std::cout << x << " ";
    // }
    load_data();
    std::cout << "DONE\n";
    // std::cout << users.size() << '\n';
    // for (auto x : users) std::cout << x.id << ' ' << x.name << ' ' << x.username << ' ' << x.hashed_password << '\n';
    return 0;
}