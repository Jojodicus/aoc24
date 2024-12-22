#include <fstream>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

long mix_n_prune(long a, long b) {
    return (a ^ b) % 16777216;
}

long hash(long n, std::map<std::tuple<int, int, int, int>, long>& banana, int iterations = 1) {
    std::vector<int> changes{};
    for (int i{0}; i < iterations; ++i) {
        long oldN{n};
        // hash
        n = mix_n_prune(n, n * 64);
        n = mix_n_prune(n, n / 32);
        n = mix_n_prune(n, n * 2048);

        // create change vector/map
        changes.push_back((n % 10) - (oldN % 10));
        if (i >= 3) {
            banana.insert({{changes[i-3], changes[i-2], changes[i-1], changes[i]}, n % 10});
        }
    }
    return n;
}

int main(int argc, char** argv) {
    // if argument is given, open file and redirect cin
    // otherwise, receive data via stdin
    std::ifstream file;
    if (argc > 1) {
        file = std::ifstream{argv[1]};
        std::cin.rdbuf(file.rdbuf());
    }

    long secretSum{};
    std::map<std::tuple<int, int, int, int>, long> allBanana{};

    long n;
    while (std::cin >> n) {
        std::map<std::tuple<int, int, int, int>, long> banana{};

        // for pt1
        secretSum += hash(n, banana, 2000);

        // for pt2
        for (auto& [tup, count] : banana) {
            if (allBanana.count(tup)) {
                // add
                allBanana.at(tup) += count;
            } else {
                // create
                allBanana.insert({tup, count});
            }
        }
    }

    std::cout << "part 1: " << secretSum << std::endl;

    long mostBanana{0};
    for (auto& [tup, count] : allBanana) {
        if (count > mostBanana) {
            mostBanana = count;
        }
    }

    std::cout << "part 2: " << mostBanana << std::endl;
}
