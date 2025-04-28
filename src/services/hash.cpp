#include <iostream>
#include <algorithm>
#include <cstdint>
#include <climits>
#include <string>

std::string hash_password(std::string password, int user_id)
{
    if (password.empty())
    {
        return "Em";
    }

    unsigned long long FIRST_XOR = (unsigned long long)557834610982;
    unsigned long long MOD = ULLONG_MAX; // 2^64 -1
    unsigned long long salt = user_id;
    unsigned long long result = 0;

    for (unsigned long long i = 0; i < password.size(); i++)
    {
        unsigned long long cur_char = (long long)password[i];

        // Perform the hash calculation
        cur_char += 43;
        cur_char ^= FIRST_XOR;
        unsigned long long temp = cur_char % MOD;
        cur_char = temp;
        cur_char *= (i + 7);
        cur_char ^= salt;
        temp = cur_char % MOD;
        cur_char = temp;

        // Add it to the result
        result += cur_char;
        result %= MOD;
    }

    // Convert the result to a string
    std::string hashed_password = std::to_string(result);
    return hashed_password;
}
