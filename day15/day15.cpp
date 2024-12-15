#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using bool_grid = std::vector<std::vector<bool>>;

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

        std::istringstream linestream{line};
        while (linestream >> c) {
            rowWalls.push_back(c == '#');
            rowBoxes.push_back(c == 'O');

            if (c == '@') {
                player = {row, col};
            }

            col++;
        }

        walls.push_back(rowWalls);
        boxes.push_back(rowBoxes);
        row++;
    }

    printGrid(walls, boxes, player);

    while (std::cin >> c) {
        switch (c) {
            case '<':
                move(walls, boxes, player, 0, -1);
                break;
            case '>':
                move(walls, boxes, player, 0, 1);
                break;
            case '^':
                move(walls, boxes, player, -1, 0);
                break;
            case 'v':
                move(walls, boxes, player, 1, 0);
                break;
        }
    }

    printGrid(walls, boxes, player);

    unsigned long gpsSum{};
    for (unsigned r{}; r < boxes.size(); ++r) {
        for (unsigned c{}; c < boxes[r].size(); ++c) {
            if (boxes[r][c]) {
                gpsSum += 100 * r + c;
            }
        }
    }

    std::cout << "part 1: " << gpsSum << std::endl;
}
