#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <vector>

using position = std::pair<int, int>;

bool isInBounds(int row, int col, int rows, int cols) {
    if (row < 0 || col < 0) {
        return false;
    }
    if (row >= rows || col >= cols) {
        return false;
    }
    return true;
}

int main(int argc, char** argv) {
    // if argument is given, open file and redirect cin
    // otherwise, receive data via stdin
    std::ifstream file;
    if (argc > 1) {
        file = std::ifstream{argv[1]};
        std::cin.rdbuf(file.rdbuf());
    }

    std::vector<std::vector<int>> topoMap{};
    std::vector<position> trailHeads{};

    std::string line;
    while (getline(std::cin, line)) {
        std::istringstream linestream{line};

        std::vector<int> row{};

        char height;
        while (linestream >> height) {
            if (height == '0') {
                // add trailheads (height 0) here to save some iterations
                trailHeads.push_back({topoMap.size(), row.size()});
            }
            row.push_back(height - '0');
        }

        topoMap.push_back(row);
    }

    unsigned long nRows{topoMap.size()};
    unsigned long nCols{topoMap.at(0).size()};

    int score{}; // part 1
    int scoreDistinct{}; // part 2
    for (auto headPos : trailHeads) {
        std::set<position> peaks{};

        // bfs
        std::queue<position> worklist{};
        worklist.push(headPos);

        while (!worklist.empty()) {
            position current{worklist.front()};
            worklist.pop();

            int height{topoMap[current.first][current.second]};

            // found peak
            if (height == 9) {
                scoreDistinct++; // part 2
                if (!peaks.count(current)) {
                    // part 1
                    score++;
                    peaks.insert(current);
                }
                continue;
            }

            //  1
            // 3X4
            //  2
            std::vector<position> cross {
                {current.first - 1, current.second},
                {current.first + 1, current.second},
                {current.first, current.second - 1},
                {current.first, current.second + 1}
            };
            for (auto [row, col] : cross) {
                if (isInBounds(row, col, nRows, nCols)) {
                    // check height
                    if (topoMap[row][col] == height + 1) {
                        worklist.push({row, col});
                    }
                }
            }
        }
    }

    std::cout << "part 1: " << score << std::endl;
    std::cout << "part 2: " << scoreDistinct << std::endl;
}
