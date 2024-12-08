#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

using position = std::pair<int, int>;

bool isInBounds(position pos, int rows, int cols) {
    if (pos.first < 0 || pos.second < 0) {
        return false;
    }
    if (pos.first >= rows || pos.second >= cols) {
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

    std::map<char, std::vector<position>*> antennae{};

    std::string line;
    int row{};
    int col{};
    while (getline(std::cin, line)) {
        col = 0;

        std::istringstream linestream{line};
        char pos;
        while (linestream >> pos) {
            if (pos == '.') {
                col++;
                continue;
            }

            // scuffed, but i'm too lazy to change
            std::vector<position>* posVector;
            auto lookup = antennae.find(pos);
            if (lookup != antennae.end()) {
                posVector = lookup->second;
            } else {
                posVector = new std::vector<position>{};
                antennae.insert({pos, posVector});
            }

            posVector->push_back({row, col});
            col++;
        }
        row++;
    }

    // row and col hold grid bounds now

    std::set<position> antinodes{};
    std::set<position> antinodesRepeating{};

    for (auto& [type, positions] : antennae) {
        for (position antenna1 : *positions) {
            for (position antenna2 : *positions) {
                if (antenna1 == antenna2) {
                    // antenna positions count towards part 2
                    antinodesRepeating.insert(antenna1);
                    continue;
                }

                // ...1.....2... -> negative D
                // ...2.....1... -> positive D
                int deltaY = antenna1.first - antenna2.first;
                int deltaX = antenna1.second - antenna2.second;

                // A..1.....2...
                // ...2.....1..A
                position newPos{antenna1.first + deltaY, antenna1.second + deltaX};
                if (isInBounds(newPos, row, col)) {
                    antinodes.insert(newPos);
                }

                // walk along antenna line
                while (isInBounds(newPos, row, col)) {
                    antinodesRepeating.insert(newPos);
                    newPos = {newPos.first + deltaY, newPos.second + deltaX};
                }
            }
        }
        delete positions;
    }

    std::cout << "part 1: " << antinodes.size() << std::endl;
    std::cout << "part 2: " << antinodesRepeating.size() << std::endl;
}
