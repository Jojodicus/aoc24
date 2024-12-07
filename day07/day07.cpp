#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>

auto concatInt(auto a, auto b) {
    for (auto shrinkingB{b}; shrinkingB != 0; shrinkingB /= 10) {
        a *= 10;
    }
    return a + b;
}

int main(int argc, char** argv) {
    // if argument is given, open file and redirect cin
    // otherwise, receive data via stdin
    std::ifstream file;
    if (argc > 1) {
        file = std::ifstream{argv[1]};
        std::cin.rdbuf(file.rdbuf());
    }

    long accValidResults{};
    long accValidResultsConcat{};

    std::string s;
    char _;
    while (std::getline(std::cin, s)) {
        std::istringstream linestream{s};

        long result;
        linestream >> result;
        linestream >> _; // `:`

        long currentValue;
        linestream >> currentValue;

        std::unordered_set<long> runValues{{currentValue}}; // part 1
        std::unordered_set<long> runValuesConcat{{currentValue}}; // part 2
        while (linestream >> currentValue) {
            std::unordered_set<long> updatedRunValues{};
            std::unordered_set<long> updatedRunValuesConcat{};
            // part 1
            for (long value : runValues) {
                long add = value + currentValue;
                long mul = value * currentValue;
                if (add <= result) { // once above result, the runValue is busted as it can't decrease
                    updatedRunValues.insert(add);
                }
                if (mul <= result) {
                    updatedRunValues.insert(mul);
                }
            }
            // part 2
            for (long value : runValuesConcat) {
                long add = value + currentValue;
                long mul = value * currentValue;
                long cat = concatInt(value, currentValue);
                if (add <= result) {
                    updatedRunValuesConcat.insert(add);
                }
                if (mul <= result) {
                    updatedRunValuesConcat.insert(mul);
                }
                if (cat <= result) {
                    updatedRunValuesConcat.insert(cat);
                }
            }
            runValues = updatedRunValues;
            runValuesConcat = updatedRunValuesConcat;
        }

        if (runValues.count(result)) {
            accValidResults += result;
        }
        if (runValuesConcat.count(result)) {
            accValidResultsConcat += result;
        }
    }

    std::cout << "part 1: " << accValidResults << std::endl;
    std::cout << "part 2: " << accValidResultsConcat << std::endl;
}
