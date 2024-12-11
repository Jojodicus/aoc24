#include <fstream>
#include <iostream>
#include <vector>

constexpr int ITERATIONS{75};

int numberOfDigits(long n) {
    int digits;
    for (digits = 0; n != 0; n /= 10) {
        digits++;
    }
    return digits;
}

std::pair<long, long> splitHalf(long n) {
    int digits{numberOfDigits(n)};

    long divisor{1};
    for (int i{}; i < digits / 2; ++i) {
        divisor *= 10;
    }

    return {n / divisor, n % divisor};
}

int main(int argc, char** argv) {
    // if argument is given, open file and redirect cin
    // otherwise, receive data via stdin
    std::ifstream file;
    if (argc > 1) {
        file = std::ifstream{argv[1]};
        std::cin.rdbuf(file.rdbuf());
    }

    std::vector<std::pair<long, int>> stones{};
    long value;
    while (std::cin >> value) {
        stones.push_back({value, ITERATIONS});
    }

    // oh boy
    long numberOfStones{};
    while (!stones.empty()) {
        auto [work, it] = stones.back();
        stones.pop_back();

        if (!it) {
            // done
            numberOfStones++;
            continue;
        }

        // rules
        if (work == 0) {
            stones.push_back({1, it - 1});
        } else if (numberOfDigits(work) % 2 == 0) {
            auto [left, right] = splitHalf(work);
            stones.push_back({left, it - 1});
            stones.push_back({right, it - 1});
        } else {
            stones.push_back({work * 2024, it - 1});
        }
    }

    std::cout << "part 1: " << numberOfStones << std::endl;
}