#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

using bool_grid = std::vector<std::vector<bool>>;

enum Extending {
    NO_BOX = 0,
    RIGHT = 1,
    LEFT = 2
};

void printGrid(bool_grid& walls, bool_grid& boxes, std::pair<unsigned, unsigned>& pos) {
    for (unsigned r{}; r < boxes.size(); ++r) {
        for (unsigned c{}; c < boxes[r].size(); ++c) {
            assert(!(walls[r][c] && boxes[r][c]));

            if (walls[r][c]) {
                std::cout << '#';
                continue;
            }

            if (boxes[r][c]) {
                std::cout << 'O';
                continue;
            }

            if (pos.first == r && pos.second == c) {
                std::cout << '@';
                continue;
            }

            std::cout << '.';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void printGridChungus(bool_grid& walls, std::vector<std::vector<Extending>>& boxes, std::pair<unsigned, unsigned>& pos) {
    for (unsigned r{}; r < boxes.size(); ++r) {
        for (unsigned c{}; c < boxes[r].size(); ++c) {
            assert(!(walls[r][c] && boxes[r][c]));

            if (walls[r][c]) {
                std::cout << '#';
                continue;
            }

            if (boxes[r][c] == RIGHT) {
                std::cout << '[';
                continue;
            }
            if (boxes[r][c] == LEFT) {
                std::cout << ']';
                continue;
            }

            if (pos.first == r && pos.second == c) {
                std::cout << '@';
                continue;
            }

            std::cout << '.';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool move(bool_grid& walls, bool_grid& boxes, std::pair<unsigned, unsigned>& pos, int dY, int dX) {
    unsigned freeY{pos.first + dY};
    unsigned freeX{pos.second + dX};

    while (boxes[freeY][freeX]) {
        freeY += dY;
        freeX += dX;
    }

    if (walls[freeY][freeX]) {
        return false;
    }

    // move player
    pos = {pos.first + dY, pos.second + dX};
    // move potential row of boxes
    boxes[freeY][freeX] = boxes[pos.first][pos.second];
    boxes[pos.first][pos.second] = false;
    return true;
}

bool moveChungus(bool_grid& walls, std::vector<std::vector<Extending>>& boxes, std::pair<unsigned, unsigned>& pos, int dY, int dX) {
    std::vector<std::pair<unsigned, unsigned>> free{};

    std::vector<std::pair<unsigned, unsigned>> worklist{{pos.first + dY, pos.second + dX}};
    while (!worklist.empty()) {
        auto [r, c] = worklist.back();
        worklist.pop_back();

        if (walls[r][c]) {
            return false;
        }

        free.push_back({r, c});

        if (boxes[r][c] == NO_BOX) {
            continue;
        }

        // box is in the way

        worklist.push_back({r + dY, c + dX});

        if (dY) {
            switch (boxes[r][c]) {
                case RIGHT:
                    worklist.push_back({r + dY, c + dX + 1});
                    break;
                case LEFT:
                    worklist.push_back({r + dY, c + dX - 1});
                    break;
                default:
                    assert(false);
            }
        }
    }

    // move boxes, starting from front and ending at the player
    std::reverse(free.begin(), free.end());
    std::set<std::pair<unsigned, unsigned>> did{};
    for (auto [r, c] : free) {
        // scuffed
        if (did.count({r, c})) {
            continue;
        }
        did.insert({r, c});

        boxes[r][c] = boxes[r - dY][c - dX];
        boxes[r - dY][c - dX] = NO_BOX;
    }
    // player
    pos = {pos.first + dY, pos.second + dX};

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

    bool_grid walls{};
    bool_grid boxes{};
    std::pair<unsigned, unsigned> player{};

    bool_grid wallsChungus{};
    std::vector<std::vector<Extending>> boxesChungus{};
    std::pair<unsigned, unsigned> playerSmol{};

    std::string line;
    unsigned row{};
    char c;
    while (std::getline(std::cin, line)) {
        if (line == "") {
            break;
        }
        unsigned col{};
        std::vector<bool> rowWalls{};
        std::vector<bool> rowBoxes{};

        std::vector<bool> rowWallsChungus{};
        std::vector<Extending> rowBoxesChungus{};

        std::istringstream linestream{line};
        while (linestream >> c) {
            rowWalls.push_back(c == '#');
            rowBoxes.push_back(c == 'O');

            rowWallsChungus.push_back(c == '#');
            rowWallsChungus.push_back(c == '#');
            rowBoxesChungus.push_back(static_cast<Extending>(c == 'O')); // RIGHT
            rowBoxesChungus.push_back(static_cast<Extending>(2 * (c == 'O'))); // LEFT

            if (c == '@') {
                player = {row, col};
                playerSmol = {row, 2 * col};
            }

            col++;
        }

        walls.push_back(rowWalls);
        boxes.push_back(rowBoxes);
        wallsChungus.push_back(rowWallsChungus);
        boxesChungus.push_back(rowBoxesChungus);
        row++;
    }

    printGrid(walls, boxes, player);
    printGridChungus(wallsChungus, boxesChungus, playerSmol);

    while (std::cin >> c) {
        switch (c) {
            case '<':
                move(walls, boxes, player, 0, -1);
                moveChungus(wallsChungus, boxesChungus, playerSmol, 0, -1);
                break;
            case '>':
                move(walls, boxes, player, 0, 1);
                moveChungus(wallsChungus, boxesChungus, playerSmol, 0, 1);
                break;
            case '^':
                move(walls, boxes, player, -1, 0);
                moveChungus(wallsChungus, boxesChungus, playerSmol, -1, 0);
                break;
            case 'v':
                move(walls, boxes, player, 1, 0);
                moveChungus(wallsChungus, boxesChungus, playerSmol, 1, 0);
                break;
        }
    }

    printGrid(walls, boxes, player);
    printGridChungus(wallsChungus, boxesChungus, playerSmol);

    unsigned long gpsSum{};
    unsigned long gpsSumChungus{};
    for (unsigned r{}; r < boxes.size(); ++r) {
        for (unsigned c{}; c < boxesChungus[r].size(); ++c) {
            if (c < boxes[r].size() && boxes[r][c]) {
                gpsSum += 100 * r + c;
            }
            if (boxesChungus[r][c] == RIGHT) { // left half of box
                gpsSumChungus += 100 * r + c;
            }
        }
    }

    std::cout << "part 1: " << gpsSum << std::endl;
    std::cout << "part 1: " << gpsSumChungus << std::endl;
}
