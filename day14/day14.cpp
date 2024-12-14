#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

constexpr int STEPS{100};
constexpr int WIDTH{101};
constexpr int HEIGHT{103};

void printGrid(std::vector<std::vector<int>>& grid) {
    for (auto& l : grid) {
        for (auto v : l) {
            if (!v) {
                std::cout << '.';
            } else {
                std::cout << v;
            }
        }
        std::cout << std::endl;
    }
}

// helper for positive modulo
auto mod(auto n, auto m) {
    return (n % m + m) % m;
}

int getQuadrant(long posY, long posX, long velY, long velX, int steps, std::vector<std::vector<int>>& grid) {
    posY = mod(posY + steps * velY, HEIGHT);
    posX = mod(posX + steps * velX, WIDTH);

    grid[posY][posX]++;

    // on a middle line
    if (posY == HEIGHT/2 || posX == WIDTH/2) {
        return 0;
    }

    if (posY < HEIGHT/2) {
        if (posX < WIDTH/2) {
            return 2; // upper left
        } else {
            return 1; // upper right
        }
    } else {
        if (posX < WIDTH/2) {
            return 3; // lower left
        } else {
            return 4; // lower right
        }
    }
}

unsigned long getSafetyScoreAfter(std::vector<std::tuple<long, long, long, long>> robots, int steps, bool print = false) {
    std::vector<std::vector<int>> grid{HEIGHT, std::vector<int>(WIDTH)};

    // 0 -> middle line, won't count
    unsigned long quadrantCounts[5]{};

    for (auto [posY, posX, velY, velX] : robots) {
        quadrantCounts[getQuadrant(posY, posX, velY, velX, steps, grid)]++;
    }

    unsigned long safetyScore{
        std::accumulate(std::begin(quadrantCounts)+1, std::end(quadrantCounts), 1ull,
            [](unsigned long acc, unsigned long e){
                return acc * e;
            }
        )
    };

    if (print) {
        printGrid(grid);
    }

    return safetyScore;
}

int main(int argc, char** argv) {
    // if argument is given, open file and redirect cin
    // otherwise, receive data via stdin
    std::ifstream file;
    if (argc > 1) {
        file = std::ifstream{argv[1]};
        std::cin.rdbuf(file.rdbuf());
    }

    std::vector<std::tuple<long, long, long, long>> robots{};

    std::string _s;
    char _c;
    while (!std::cin.eof()) {
        int posY, posX;
        int velY, velX;

        std::cin >> _c >> _c; // "p="
        std::cin >> posX;
        std::cin >> _c; // ','
        std::cin >> posY;
        std::cin >> _c >> _c; // "v="
        std::cin >> velX;
        std::cin >> _c; // ','
        std::cin >> velY;

        robots.push_back({posY, posX, velY, velX});
    }

    unsigned long minScore{-1ull}, minSteps{};
    for (int steps{}; steps < std::lcm(HEIGHT, WIDTH); ++steps) {
        unsigned long newScore = getSafetyScoreAfter(robots, steps);
        if (newScore < minScore) {
            minScore = newScore;
            minSteps = steps;
        }
    }
    getSafetyScoreAfter(robots, minSteps, true); // print

    std::cout << "part 1: " << getSafetyScoreAfter(robots, STEPS) << std::endl;
    std::cout << "part 2: " << minSteps << std::endl;
}
