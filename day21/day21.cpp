#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

// 7 8 9
// 4 5 6
// 1 2 3
//   0 A

//   ^ A
// < v >

// found heuristically:
// we want as many consecutive presses of the same button as possible -> reduce travel time in lower depths
// <^ is better than ^< -> makes the << presses one depth lower consecutive
// ...
// yes, one could find this programmatically...
std::map<char, std::map<char, std::string>> bestSteps{
    {'A', {{'A', "A"}, {'0', "<A"}, {'1', "^<<A"}, {'2', "<^A"}, {'3', "^A"}, {'4', "^^<<A"}, {'5', "<^^A"}, {'6', "^^A"}, {'7', "^^^<<A"}, {'8', "<^^^A"}, {'9', "^^^A"}, {'<', "v<<A"}, {'v', "<vA"}, {'^', "<A"}, {'>', "vA"}}},
    {'0', {{'A', ">A"}, {'0', "A"}, {'1', "^<A"}, {'2', "^A"}, {'3', "^>A"}, {'4', "^^<A"}, {'5', "^^A"}, {'6', "^^>A"}, {'7', "^^^<A"}, {'8', "^^^A"}, {'9', "^^^>A"}}},
    {'1', {{'A', ">>vA"}, {'0', ">vA"}, {'1', "A"}, {'2', ">A"}, {'3', ">>A"}, {'4', "^A"}, {'5', "^>A"}, {'6', "^>>A"}, {'7', "^^A"}, {'8', "^^>A"}, {'9', "^^>>A"}}},
    {'2', {{'A', "v>A"}, {'0', "vA"}, {'1', "<A"}, {'2', "A"}, {'3', ">A"}, {'4', "<^A"}, {'5', "^A"}, {'6', "^>A"}, {'7', "<^^A"}, {'8', "^^A"}, {'9', "^^>A"}}},
    {'3', {{'A', "vA"}, {'0', "<vA"}, {'1', "<<A"}, {'2', "<A"}, {'3', "A"}, {'4', "<<^A"}, {'5', "<^A"}, {'6', "^A"}, {'7', "<<^^A"}, {'8', "<^^A"}, {'9', "^^A"}}},
    {'4', {{'A', ">>vvA"}, {'0', ">vvA"}, {'1', "vA"}, {'2', "v>A"}, {'3', "v>>A"}, {'4', "A"}, {'5', ">A"}, {'6', ">>A"}, {'7', "^A"}, {'8', "^>A"}, {'9', "^>>A"}}},
    {'5', {{'A', "vv>A"}, {'0', "vvA"}, {'1', "<vA"}, {'2', "vA"}, {'3', "v>A"}, {'4', "<A"}, {'5', "A"}, {'6', ">A"}, {'7', "<^A"}, {'8', "^A"}, {'9', "^>A"}}},
    {'6', {{'A', "vvA"}, {'0', "<vvA"}, {'1', "<<vA"}, {'2', "<vA"}, {'3', "vA"}, {'4', "<<A"}, {'5', "<A"}, {'6', "A"}, {'7', "<<^A"}, {'8', "<^A"}, {'9', "^A"}}},
    {'7', {{'A', ">>vvvA"}, {'0', ">vvvA"}, {'1', "vvA"}, {'2', "vv>A"}, {'3', "vv>>A"}, {'4', "vA"}, {'5', "v>A"}, {'6', "v>>A"}, {'7', "A"}, {'8', ">A"}, {'9', ">>A"}}},
    {'8', {{'A', "vvv>A"}, {'0', "vvvA"}, {'1', "<vvA"}, {'2', "vvA"}, {'3', "vv>A"}, {'4', "<vA"}, {'5', "vA"}, {'6', "v>A"}, {'7', "<A"}, {'8', "A"}, {'9', ">A"}}},
    {'9', {{'A', "vvvA"}, {'0', "<vvvA"}, {'1', "<<vvA"}, {'2', "<vvA"}, {'3', "vvA"}, {'4', "<<vA"}, {'5', "<vA"}, {'6', "vA"}, {'7', "<<A"}, {'8', "<A"}, {'9', "A"}}},
    {'<', {{'A', ">>^A"}, {'<', "A"}, {'v', ">A"}, {'^', ">^A"}, {'>', ">>A"}}},
    {'v', {{'A', "^>A"}, {'<', "<A"}, {'v', "A"}, {'^', "^A"}, {'>', ">A"}}},
    {'^', {{'A', ">A"}, {'<', "v<A"}, {'v', "vA"}, {'^', "A"}, {'>', "v>A"}}},
    {'>', {{'A', "^A"}, {'<', "<<A"}, {'v', "<A"}, {'^', "<^A"}, {'>', "A"}}}
};

std::map<std::pair<int, std::string>, unsigned long> cache{};

unsigned long findPath(std::string sequence, int depth) {
    if (depth == 0) {
        return sequence.size();
    }

    if (cache.count({depth, sequence})) {
        return cache.at({depth, sequence});
    }

    char drag{'A'}; // arm starts at A
    unsigned long pathLength{};
    for (char c : sequence) {
        pathLength += findPath(bestSteps.at(drag).at(c), depth - 1);
        drag = c;
    }

    cache.insert({{depth, sequence}, pathLength});
    return pathLength;
}

int main(int argc, char** argv) {
    // if argument is given, open file and redirect cin
    // otherwise, receive data via stdin
    std::ifstream file;
    if (argc > 1) {
        file = std::ifstream{argv[1]};
        std::cin.rdbuf(file.rdbuf());
    }

    unsigned long sumComplexity{};
    unsigned long sumComplexComplexity{};

    std::string sequence;
    while (std::cin >> sequence) {
        std::istringstream stream{sequence};
        unsigned numeric;
        stream >> numeric;

        unsigned long lengthP1 = findPath(sequence, 3);
        unsigned long lengthP2 = findPath(sequence, 26);
        unsigned long complexityP1 = numeric * lengthP1;
        unsigned long complexityP2 = numeric * lengthP2;

        std::cout << sequence << std::endl;
        std::cout << std::format("depth 3: {} * {} = {}", lengthP1, numeric, complexityP1) << std::endl;
        std::cout << std::format("depth 26: {} * {} = {}", lengthP2, numeric, complexityP2) << std::endl;
        std::cout << std::endl;

        sumComplexity += numeric * lengthP1;
        sumComplexComplexity += numeric * lengthP2;
    }

    std::cout << "part 1: " << sumComplexity << std::endl;
    std::cout << "part 2: " << sumComplexComplexity << std::endl;
}
