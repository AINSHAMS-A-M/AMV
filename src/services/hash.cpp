#include <iostream>
#include <cstdint> 
#include <structs.hpp>
#include <algorithm>
#include <string>
#include <cstdint>

std::string uint64_to_string(uint64_t value) {
    if (value == 0) return "0";

    std::string result;
    while (value > 0) 
    {
        char digit = '0' + (value % 10);
        result = digit + result;
        value /= 10;
    }
    return result;
}

std::string hash_password(std::string password, std::string user_id) 
{
    if (password.empty()) {
        throw std::invalid_argument("Password cannot be empty");
    }
    if (user_id.empty()) {
        throw std::invalid_argument("User ID cannot be empty");
    }

    uint64_t FIRST_XOR = (uint64_t)55783461098202684762;
    uint64_t MOD = (uint64_t(170141182460469231731687303715884105727)); // 2^127 -1
    uint64_t salt = stoll(user_id);
    uint64_t result = 0;

    for (unsigned long long i = 0; i < password.size(); i++) 
    {
        uint64_t cur_char = (long long)password[i];

        // Perform the hash calculation
        cur_char += 43;
        cur_char ^= FIRST_XOR;
        uint64_t temp = cur_char % MOD;
        cur_char = temp;
        cur_char *= (i + 13);
        cur_char ^= salt;
        temp = cur_char % MOD;
        cur_char = temp;

        // Add it to the result
        result += cur_char;
        result %= MOD;
    }

    // Convert the result to a string
    std::string hashed_password = uint64_to_string(result);
    return hashed_password;
}




