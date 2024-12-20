#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <unordered_set>
#include <vector>

constexpr int cheatThreshold{100};

using position_t = std::pair<int, int>;

std::vector<position_t> computeLegalPath(position_t start, position_t finish, std::vector<std::vector<bool>>& walls) {
    // queue holds (y, x, [path])
    std::queue<std::tuple<int, int, std::vector<position_t>>> queue{};
    std::set<position_t> visited{};

    // start
    queue.push({start.first, start.second, {start}});

    while (!queue.empty()) {
        auto [r, c, path] = queue.front();
        queue.pop();

        // found finish path
        if (r == finish.first && c == finish.second) {
            return path;
        }

        if (visited.count({r, c})) {
            continue;
        }
        visited.insert({r, c});

        std::vector<position_t> extensions{
            {r - 1, c},
            {r + 1, c},
            {r, c - 1},
            {r, c + 1}
        };

        for (auto [next_r, next_c] : extensions) {
            if (visited.count({next_r, next_c}) || walls[next_r][next_c]) {
                continue;
            }

            auto next_path = path;
            next_path.push_back({next_r, next_c});
            queue.push({next_r, next_c, next_path});
        }
    }

    return {};
}

std::pair<std::vector<unsigned long>, std::vector<unsigned long>> computeCheats(std::vector<position_t> path) {
    // index: time saved, value: number of cheats
    std::vector<unsigned long> cheats1(path.size(), 0ul);
    std::vector<unsigned long> cheats2(path.size(), 0ul);

    for (int start{0}; start < static_cast<int>(path.size()); ++start) {
        for (int end{start}; end < static_cast<int>(path.size()); ++end) {
            // row or column differ by two: cheat possible
            if ((path[start].second == path[end].second && std::abs(path[start].first - path[end].first) == 2)
                || (path[start].first == path[end].first && std::abs(path[start].second - path[end].second) == 2)) {
                // saves time?
                int time_saved = end - start - 2;
                if (time_saved > 0) {
                    cheats1[time_saved]++;
                }
            }

            // distance <= 20: cheat possible
            int manhattanDistance{std::abs(path[start].first - path[end].first) + std::abs(path[start].second - path[end].second)};
            if (manhattanDistance <= 20) {
                // saves time?
                int time_saved = end - start - manhattanDistance;
                if (time_saved > 0) {
                    cheats2[time_saved]++;
                }
            }
        }
    }

    // for (int i{0}; i < static_cast<int>(cheats1.size()); ++i) {
    //     if (cheats1[i] || cheats2[i]) {
    //         std::cout << i << ": " << cheats1[i] << '\t' << cheats2[i] << std::endl;
    //     }
    // }

    return {cheats1, cheats2};
}

int main(int argc, char** argv) {
    // if argument is given, open file and redirect cin
    // otherwise, receive data via stdin
    std::ifstream file;
    if (argc > 1) {
        file = std::ifstream{argv[1]};
        std::cin.rdbuf(file.rdbuf());
    }

    std::vector<std::vector<bool>> walls{};
    position_t player{};
    position_t finish{};

    std::string line;
    int row{};
    int col{};
    while (std::getline(std::cin, line)) {
        std::istringstream linestream{line};

        col = 0;
        std::vector<bool> rowWall{};

        char c;
        while (linestream >> c) {
            rowWall.push_back(c == '#');

            if (c == 'S') {
                player = {row, col};
            }
            if (c == 'E') {
                finish = {row, col};
            }

            col++;
        }

        walls.push_back(rowWall);
        row++;
    }

    std::vector<position_t> legalPath = computeLegalPath(player, finish, walls);
    std::cout << "original fastest time: " << legalPath.size()-1 << " ps" << std::endl;

    auto [cheats1, cheats2] = computeCheats(legalPath);

    std::cout << "part 1: " << std::accumulate(cheats1.begin() + cheatThreshold, cheats1.end(), 0ul) << std::endl;
    std::cout << "part 2: " << std::accumulate(cheats2.begin() + cheatThreshold, cheats2.end(), 0ul) << std::endl;
}
