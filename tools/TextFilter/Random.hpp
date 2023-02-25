//
//  Random.hpp
//  WNITE
//
//  Created by Molybdenum on 12/5/21.
//

#ifndef Random_h
#define Random_h
#include <chrono>
#include <ctime>
#include <random>

namespace AN {
class Random {
    std::mt19937_64 generator;

    static auto _get_seed() {
        std::random_device os_seed;
        auto seed = os_seed() ^ ((std::mt19937_64::result_type)
                                         std::chrono::duration_cast<std::chrono::seconds>(
                                                 std::chrono::system_clock::now().time_since_epoch())
                                                 .count() +
                                 (std::mt19937_64::result_type)
                                         std::chrono::duration_cast<std::chrono::microseconds>(
                                                 std::chrono::high_resolution_clock::now().time_since_epoch())
                                                 .count());
        return seed;
    }


public:
    Random() : generator(_get_seed()) {}

    template<typename T>
    T get(T left, T right) {
        return std::uniform_int_distribution<T>(left, right)(generator);
    }

    bool getBool() {
        return std::uniform_int_distribution<>(0, 1)(generator);
    }

    std::string getString(std::string::size_type length) {
        static const char chrs[] = "0123456789"
                                   "abcdefghijklmnopqrstuvwxyz"
                                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        std::string s;

        s.reserve(length);

        while (length--) {
            s += chrs[get<short>(0UL, sizeof chrs - 2)];
        }

        return s;
    }
};


}

#endif /* Random_h */
