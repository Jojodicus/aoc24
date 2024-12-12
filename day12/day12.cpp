#include <fstream>
#include <iostream>
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

    std::vector<std::vector<char>> grid{};
    std::vector<std::vector<bool>> visited{};

    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream linestream{line};

        std::vector<char> gridline{};
        std::vector<bool> visitline{};

        char field;
        while (linestream >> field) {
            gridline.push_back(field);
            visitline.push_back(false);
        }

        grid.push_back(gridline);
        visited.push_back(visitline);
    }

    unsigned long nRows{grid.size()};
    unsigned long nCols{grid.at(0).size()};

    unsigned long totalPrice{};
    unsigned long totalPriceDiscounted{};

    for (unsigned long row{0}; row < nRows; ++row) {
        for (unsigned long col{0}; col < nCols; ++col) {
            if (visited[row][col]) {
                continue;
            }

            // flood-fill
            char val{grid[row][col]};
            int area{};
            int perimeter{};
            int perimeterDiscounted{};
            std::vector<std::tuple<unsigned long, unsigned long>> worklist{{{row, col}}};
            while (!worklist.empty()) {
                auto [workRow, workCol] = worklist.back();
                worklist.pop_back();

                if (visited[workRow][workCol]) {
                    continue;
                }

                visited[workRow][workCol] = true;
                area++;
                perimeter += 4;
                perimeterDiscounted += 4;

                // surroundings
                std::vector<std::pair<unsigned long, unsigned long>> cross {
                    {workRow - 1, workCol},
                    {workRow + 1, workCol},
                    {workRow, workCol - 1},
                    {workRow, workCol + 1}
                };
                for (auto [crossRow, crossCol] : cross) {
                    // `0 - 1` would wrap around
                    if (crossRow >= nRows || crossCol >= nCols) {
                        continue;
                    }
                    char crossValue{grid[crossRow][crossCol]};
                    if (crossValue == val) {
                        // found connected field
                        perimeter--; // no fence between them, executed twice for each fence for each direction
                        perimeterDiscounted--;
                        worklist.push_back({crossRow, crossCol});

                        // part 2 stuff...
                        if (visited[crossRow][crossCol]) {
                            continue;
                        }
                        int diffR = workRow - crossRow;
                        int diffC = workCol - crossCol;
                        if (diffR) {
                            // check in column
                            if (workCol == 0 || (grid[workRow][workCol-1] != val && grid[crossRow][crossRow-1] != val)) {
                                // std::printf("1 %c: %ld,%ld %ld,%ld\n", val, workRow, workCol, crossRow, crossCol);
                                perimeterDiscounted--;
                            }
                            if (workCol == nCols-1 || (grid[workRow][workCol+1] != val && grid[crossRow][crossRow+1] != val)) {
                                // std::printf("2 %c: %ld,%ld %ld,%ld\n", val, workRow, workCol, crossRow, crossCol);
                                perimeterDiscounted--;
                            }
                        }
                        if (diffC) {
                            // check in row
                            if (workRow == 0 || (grid[workRow-1][workCol] != val && grid[crossRow-1][crossCol] != val)) {
                                // std::printf("3 %c: %ld,%ld %ld,%ld\n", val, workRow, workCol, crossRow, crossCol);
                                perimeterDiscounted--;
                            }
                            if (workRow == nRows-1 || (grid[workRow+1][workCol] != val && grid[crossRow+1][crossCol] != val)) {
                                // std::printf("4 %c: %ld,%ld %ld,%ld\n", val, workRow, workCol, crossRow, crossCol);
                                perimeterDiscounted--;
                            }
                        }
                    }
                }
            }

            std::cout << val << " - a: " << area << ", p: " << perimeterDiscounted << std::endl;

            totalPrice += area * perimeter;
            totalPriceDiscounted += area * perimeterDiscounted;
        }
    }

    std::cout << "part 1: " << totalPrice << std::endl;
    std::cout << "part 2: " << totalPriceDiscounted << std::endl;
}
