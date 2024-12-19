#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

unsigned long getPossible(std::string pattern,
    std::unordered_set<std::string>& availableTowels,
    std::unordered_map<std::string, unsigned long>& cachePossible) {
    if (pattern == "") {
        // one way to make the empty pattern (base case)
        return 1;
    }

    // memo
    if (cachePossible.contains(pattern)) {
        return cachePossible.at(pattern);
    }

    unsigned long ways{};
    for (unsigned sub{1}; sub <= pattern.length(); ++sub) {
        // brwrr -> b rwrr
        //          br wrr
        //          brw rr
        //          brwr r
        //          brwrr
        std::string front = pattern.substr(0, sub);
        std::string back = pattern.substr(sub, pattern.length());

        if (availableTowels.contains(front)) {
            ways += getPossible(back, availableTowels, cachePossible);
        }
    }

    cachePossible.insert({pattern, ways});
    return ways;
}

int main(int argc, char** argv) {
    // if argument is given, open file and redirect cin
    // otherwise, receive data via stdin
    std::ifstream file;
    if (argc > 1) {
        file = std::ifstream{argv[1]};
        std::cin.rdbuf(file.rdbuf());
    }

    std::unordered_set<std::string> availableTowels{};
    std::unordered_map<std::string, unsigned long> cachePossible{};

    std::string s;
    std::istringstream sstream;
    // parse available towels
    while (std::getline(std::cin, s, ',')) {
        sstream = std::istringstream{s};
        sstream >> s;
        availableTowels.insert(s);
    }

    // search
    unsigned long possible{};
    unsigned long possibleAll{};
    while (sstream >> s) {
        unsigned long forCurrent = getPossible(s, availableTowels, cachePossible);
        possible += static_cast<bool>(forCurrent);
        possibleAll += forCurrent;
    }

    std::cout << "part 1: " << possible << std::endl;
    std::cout << "part 2: " << possibleAll << std::endl;
}
