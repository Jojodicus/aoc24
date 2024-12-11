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

    std::vector<long> stones{};
    long value;
    while (std::cin >> value) {
        stones.push_back(value);
    }

    // little bit of caching
    std::vector<std::vector<long>> cache{};
    for (int iteration{0}; iteration <= ITERATIONS; ++iteration) {
        std::vector<long> itVec{};
        for (int digit{0}; digit < 10; ++digit) {
            std::vector<std::pair<long, int>> worklist{};
            worklist.push_back({digit, iteration});
            long totalIterations{};

            while (!worklist.empty()) {
                auto [work, it] = worklist.back();
                worklist.pop_back();

                if (!it) {
                    // done
                    totalIterations++;
                    continue;
                }

                if (it != iteration && work / 10 == 0) {
                    // cached
                    totalIterations += cache[it][work];
                    continue;
                }

                // rules
                if (work == 0) {
                    worklist.push_back({1, it - 1});
                } else if (numberOfDigits(work) % 2 == 0) {
                    auto [left, right] = splitHalf(work);
                    worklist.push_back({left, it - 1});
                    worklist.push_back({right, it - 1});
                } else {
                    worklist.push_back({work * 2024, it - 1});
                }
            }

            itVec.push_back(totalIterations);
        }
        cache.push_back(itVec);
    }

    // oh boy
    long numberOfStones{};
    #pragma omp parallel for reduction(+ : numberOfStones)
    for (auto& stone: stones) {
        std::vector<std::pair<long, int>> worklist{};
        worklist.push_back({stone, ITERATIONS});

        while (!worklist.empty()) {
            auto [work, it] = worklist.back();
            worklist.pop_back();

            if (!it) {
                // done
                numberOfStones++;
                continue;
            }

            if (work / 10 == 0) {
                // cached
                numberOfStones += cache.at(it).at(work);
                continue;
            }

            // rules
            if (work == 0) {
                worklist.push_back({1, it - 1});
            } else if (numberOfDigits(work) % 2 == 0) {
                auto [left, right] = splitHalf(work);
                worklist.push_back({left, it - 1});
                worklist.push_back({right, it - 1});
            } else {
                worklist.push_back({work * 2024, it - 1});
            }
        }
    }

    std::cout << ITERATIONS << " iterations, total: " << numberOfStones << std::endl;
}