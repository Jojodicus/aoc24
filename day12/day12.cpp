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
            int rebate{};
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

                // surroundings
                std::vector<std::pair<unsigned long, unsigned long>> cross {
                    {workRow - 1, workCol},
                    {workRow + 1, workCol},
                    {workRow, workCol - 1},
                    {workRow, workCol + 1}
                };
                for (auto [crossRow, crossCol] : cross) {
                    // `0 - 1` wraps around, saving a few comparisons (not that it matters anyway...)
                    if (crossRow >= nRows || crossCol >= nCols) {
                        continue;
                    }
                    char crossValue{grid[crossRow][crossCol]};
                    if (crossValue == val) {
                        // found connected field
                        // no fence between them, executed twice for each fence for each direction
                        perimeter--;
                        worklist.push_back({crossRow, crossCol});

                        // part 2 stuff...
                        if (visited[crossRow][crossCol]) {
                            // avoid counting both directions
                            continue;
                        }
                        int diffR = workRow - crossRow;
                        int diffC = workCol - crossCol;
                        if (diffR) {
                            // check in column
                            if (workCol == 0 || (grid[workRow][workCol-1] != val && grid[crossRow][crossCol-1] != val)) {
                                // ....
                                // XVV.
                                // XVV.
                                // ....
                                rebate++;
                            }
                            if (workCol == nCols-1 || (grid[workRow][workCol+1] != val && grid[crossRow][crossCol+1] != val)) {
                                // ....
                                // .VVX
                                // .VVX
                                // ....
                                rebate++;
                            }
                        }
                        if (diffC) {
                            // check in row
                            if (workRow == 0 || (grid[workRow-1][workCol] != val && grid[crossRow-1][crossCol] != val)) {
                                // .XX.
                                // .VV.
                                // .VV.
                                // ....
                                rebate++;
                            }
                            if (workRow == nRows-1 || (grid[workRow+1][workCol] != val && grid[crossRow+1][crossCol] != val)) {
                                // ....
                                // .VV.
                                // .VV.
                                // .XX.
                                rebate++;
                            }
                        }
                    }
                }
            }

            totalPrice += area * perimeter;
            totalPriceDiscounted += area * (perimeter - rebate);
        }
    }

    std::cout << "part 1: " << totalPrice << std::endl;
    std::cout << "part 2: " << totalPriceDiscounted << std::endl;
}
