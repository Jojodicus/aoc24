#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <tuple>
#include <vector>

// [0, 70]
constexpr int rows{71};
constexpr int cols{71};

constexpr int falling{1024};

std::pair<int, int> readPos() {
    int x, y;
    char _;
    std::cin >> x >> _ >> y;
    return {x, y};
}

std::vector<std::pair<int, int>> getExtensions(int r, int c, std::vector<std::vector<bool>>& corruptions) {
    std::vector<std::pair<int, int>> result{};
    if (r - 1 >= 0 && !corruptions[r - 1][c]) {
        result.push_back({r - 1, c});
    }
    if (c - 1 >= 0 && !corruptions[r][c - 1]) {
        result.push_back({r, c - 1});
    }
    if (r + 1 < rows && !corruptions[r + 1][c]) {
        result.push_back({r + 1, c});
    }
    if (c + 1 < cols && !corruptions[r][c + 1]) {
        result.push_back({r, c + 1});
    }
    return result;
}

std::set<std::pair<int, int>> getPath(std::vector<std::vector<bool>>& corruptions) {
    // queue holds (y, x, [path])
    std::queue<std::tuple<int, int, std::set<std::pair<int, int>>>> queue{};
    std::set<std::pair<int, int>> visited{};

    // start
    queue.push({0, 0, {}});

    while (!queue.empty()) {
        auto [r, c, path] = queue.front();
        queue.pop();

        // found finish path
        if (r == rows-1 && c == cols-1) {
            return path;
        }

        if (visited.count({r, c})) {
            continue;
        }
        visited.insert({r, c});

        for (auto [next_r, next_c] : getExtensions(r, c, corruptions)) {
            if (visited.count({next_r, next_c})) {
                continue;
            }

            auto next_path = path;
            next_path.insert({next_r, next_c});
            queue.push({next_r, next_c, next_path});
        }
    }

    return {};
}

int main(int argc, char** argv) {
    // if argument is given, open file and redirect cin
    // otherwise, receive data via stdin
    std::ifstream file;
    if (argc > 1) {
        file = std::ifstream{argv[1]};
        std::cin.rdbuf(file.rdbuf());
    }

    std::vector<std::vector<bool>> corruptions{rows, std::vector(cols, false)};

    for (int i{0}; i < falling; ++i) {
        auto corr = readPos();
        corruptions[corr.first][corr.second] = true;
    }

    std::set<std::pair<int, int>> path = getPath(corruptions);
    std::cout << "part 1: " << path.size() << std::endl;

    while (std::cin) {
        auto corr = readPos();
        corruptions[corr.first][corr.second] = true;

        if (!path.count(corr)) {
            // corruption not on path -> save recomputing
            continue;
        }

        // corruption on path
        path = getPath(corruptions);
        if (path.empty()) {
            // armageddon
            std::cout << "part 2: " << corr.first << ',' << corr.second << std::endl;
            break;
        }
    }
}
