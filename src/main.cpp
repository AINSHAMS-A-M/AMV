#include <iostream>
#include <algorithm>
#include "data_structures.hpp"

int main()
{
    std::cout << "WELCOME TO AMV\n";
    AMArray<int> a;
    a.push_back(2);
    a.push_back(4);
    a.push_back(5);
    for (size_t i = 0; i < a.size(); ++i)
    {
        std::cout << a[i] << ' ';
    }
    for (int x : a)
    {
        std::cout << x << " ";
    }

    return 0;
}