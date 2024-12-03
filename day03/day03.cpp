#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <sstream>
#include <vector>

int main(int argc, char** argv) {
    // if argument is given, open file and redirect cin
    // otherwise, receive data via stdin
    std::ifstream file;
    if (argc > 1) {
        file = std::ifstream{argv[1]};
        std::cin.rdbuf(file.rdbuf());
    }

    // read everything
    std::ostringstream oStream{};
    oStream << std::cin.rdbuf();
    std::string input{oStream.str()};

    // matches all of `mul(X,Y)`, with X and Y as subgroups
    // for the second part, we match `do()` and `don't()`
    std::regex expr{"mul\\((\\d*),(\\d*)\\)|do\\(\\)|don't\\(\\)"};
    auto iteratorBegin{std::sregex_iterator(input.begin(), input.end(), expr)};
    auto iteratorEnd{std::sregex_iterator()};

    // find and multiply
    long sum{0};
    bool mulEnabled{true};
    long sumWhileEnabled{0};
    for (std::sregex_iterator i{iteratorBegin}; i != iteratorEnd; ++i) {
        std::smatch match{*i};

        // `do()` or `don't()`?
        if (match.str() == "do()") {
            mulEnabled = true;
            continue;
        }
        if (match.str() == "don't()") {
            mulEnabled = false;
            continue;
        }

        // guaranteed to match `mul(X,Y)`
        long mult = std::accumulate(match.begin() + 1, match.end(), 1l, [](long acc, auto curr){
            return acc * std::stol(curr.str());
        });
        sum += mult;
        if (mulEnabled) {
            sumWhileEnabled += mult;
        }
    }

    std::cout << "part 1: " << sum << std::endl;
    std::cout << "part 2: " << sumWhileEnabled << std::endl;
}
