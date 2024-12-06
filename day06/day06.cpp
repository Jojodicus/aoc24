#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <unordered_set>
#include <vector>

using position = std::pair<long, long>;
using direction = std::pair<int, int>;

void turnRight(direction& dir) {
    // -1  0 ->  0  1
    //  1  0 ->  0 -1
    //  0 -1 -> -1  0
    //  0  1 ->  1  0
    int oldY = dir.first;
    dir.first = dir.second;
    dir.second = -oldY;
}

bool guardInBounds(position& pos, long sizeY, long sizeX) {
    if (pos.first < 0 || pos.second < 0) {
        return false;
    }
    if (pos.first >= sizeY || pos.second >= sizeX) {
        return false;
    }
    return true;
}

position step(position pos, direction dir) {
    return {pos.first + dir.first, pos.second + dir.second};
}

struct megaHasher3000 {
    long operator()(const std::pair<position, direction>& p) const {
        long hash{p.first.first};
        hash ^= p.first.second << 8;
        hash ^= p.second.first << 16;
        hash ^= p.second.second << 24;
        return hash;
    }
};

// return: ifLoop, numStepsTaken
std::pair<bool, long> walk(std::vector<std::vector<bool>>& obstructionMatrix, position guardPos, direction guardDir, position extraObstacle = {-1, -1}) {
    std::unordered_set<std::pair<position, direction>, megaHasher3000> visited{};

    unsigned long uniqueSteps{};
    long sizeY{static_cast<long>(obstructionMatrix.size())};
    long sizeX{static_cast<long>(obstructionMatrix.at(0).size())};
    while (guardInBounds(guardPos, sizeY, sizeX)) {
        if (visited.count({guardPos, guardDir})) {
            return {true, uniqueSteps};
        }

        uniqueSteps++;
        visited.insert({guardPos, guardDir});

        position newPosition{step(guardPos, guardDir)};
        while (guardInBounds(newPosition, sizeY, sizeX) && (obstructionMatrix[newPosition.first][newPosition.second] || extraObstacle == newPosition)) {
            turnRight(guardDir);
            newPosition = step(guardPos, guardDir);
        }
        guardPos = newPosition;
    }

    return {false, uniqueSteps};
}

int main(int argc, char** argv) {
    // if argument is given, open file and redirect cin
    // otherwise, receive data via stdin
    std::ifstream file;
    if (argc > 1) {
        file = std::ifstream{argv[1]};
        std::cin.rdbuf(file.rdbuf());
    }

    std::vector<std::vector<bool>> obstructionMatrix{};
    position guardStartPos{};
    direction guardStartDir{-1, 0}; // up

    std::vector<std::vector<bool>> visitedMatrix{};

    std::string s;
    while (std::getline(std::cin, s)) {
        std::vector<bool> row{};
        std::vector<bool> rowVisited{};

        std::istringstream istream{s};
        char c;
        while (istream >> c) {
            rowVisited.push_back(false);
            switch (c) {
                case '#':
                    // box
                    row.push_back(true);
                    break;
                case '^':
                    guardStartPos = {static_cast<long>(obstructionMatrix.size()), static_cast<long>(row.size())};
                    [[fallthrough]];
                default:
                    // no box
                    row.push_back(false);
                    break;
            }
        }

        obstructionMatrix.push_back(row);
        visitedMatrix.push_back(rowVisited);
    }

    // walk
    unsigned long uniqueSteps{};
    position guardPos{guardStartPos};
    direction guardDir{guardStartDir}; // up
    long sizeY{static_cast<long>(obstructionMatrix.size())};
    long sizeX{static_cast<long>(obstructionMatrix.at(0).size())};
    while (guardInBounds(guardPos, sizeY, sizeX)) {
        uniqueSteps += !visitedMatrix[guardPos.first][guardPos.second];
        visitedMatrix[guardPos.first][guardPos.second] = true;

        position newPosition{step(guardPos, guardDir)};
        if (guardInBounds(newPosition, sizeY, sizeX) && obstructionMatrix[newPosition.first][newPosition.second]) {
            turnRight(guardDir);
            newPosition = step(guardPos, guardDir);
        }
        guardPos = newPosition;
    }


    unsigned long resultInLoop{0};
    #pragma omp parallel for reduction(+ : resultInLoop)
    for (int r = 0; r < sizeY; ++r) {
        for (int c{0}; c < sizeX; ++c) {
            if (!visitedMatrix[r][c] || guardStartPos == std::pair<long, long>{r, c}) {
                continue;
            }

            auto [loop, _] = walk(obstructionMatrix, guardStartPos, guardStartDir, {r, c});
            resultInLoop += loop;
        }
    }

    std::cout << "part 1: " << uniqueSteps << std::endl;
    std::cout << "part 2: " << resultInLoop << std::endl;
}
