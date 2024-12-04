#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>
#include <vector>

using xmatrix = std::vector<std::vector<char>>;

void printM(const xmatrix& m) {
    for (auto l : m) {
        for (auto c : l) {
            std::cout << c;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

long findXmas(const xmatrix& matrix) {
    long count{0};
    std::regex xmas{"XMAS"};
    std::regex samx{"SAMX"};

    for (auto line : matrix) {
        // match and count
        std::string sline{std::string_view{line.data()}};
        auto startXmax{std::sregex_iterator(sline.begin(), sline.end(), xmas)};
        auto startSamx{std::sregex_iterator(sline.begin(), sline.end(), samx)};
        auto end{std::sregex_iterator()};

        count += std::distance(startXmax, end);
        count += std::distance(startSamx, end);
    }

    return count;
}

xmatrix transpose(const xmatrix& matrix) {
    xmatrix mTrans{};
    for (unsigned i{0}; i < matrix.at(0).size(); ++i) {
        std::vector<char> push{};
        for (unsigned j{0}; j < matrix.size(); ++j) {
            push.push_back(matrix[j][i]);
        }
        mTrans.push_back(push);
    }
    return mTrans;
}

xmatrix flip(const xmatrix& matrix) {
    xmatrix flipped{};
    for (auto x : matrix) {
        std::vector reversed{x};
        std::reverse(reversed.begin(), reversed.end());
        flipped.push_back(reversed);
    }
    return flipped;
}

xmatrix halfRotate(const xmatrix& matrix) {
    xmatrix mRot{};

    // wavefront
    unsigned long rowSize{matrix.at(0).size()};
    for (unsigned i{0}; i < matrix.size() + rowSize; ++i) {
        std::vector<char> push{};

        long r{i};
        unsigned c{0};
        while (c < rowSize) {
            if (0 <= r && r < static_cast<long>(matrix.size()) && c < rowSize) {
                push.push_back(matrix[r][c]);
            } else {
                push.push_back('Z');
            }
            r--;
            c++;
        }

        mRot.push_back(push);
    }

    return mRot;
}

int main(int argc, char** argv) {
    // if argument is given, open file and redirect cin
    // otherwise, receive data via stdin
    std::ifstream file;
    if (argc > 1) {
        file = std::ifstream{argv[1]};
        std::cin.rdbuf(file.rdbuf());
    }

    // we always assume a rectangular grid

    // build input matrix
    xmatrix mNormal{};
    std::string line;
    while (std::cin >> line) {
        std::vector<char> push{};
        for (char c : line) {
            push.push_back(c);
        }
        mNormal.push_back(push);
    }

    xmatrix mTrans{transpose(mNormal)};

    long count{0};
    count += findXmas(mNormal);
    count += findXmas(mTrans);
    count += findXmas(halfRotate(mNormal));
    count += findXmas(halfRotate(flip(mNormal)));

    // scuffed
    long count2{0};
    for (auto [row, line] : std::views::enumerate(mNormal)) {
        for (auto [col, elem] : std::views::enumerate(line)) {
            if (elem == 'A'
                && row > 0 && row < static_cast<long>(mNormal.size()) - 1
                && col > 0 && col < static_cast<long>(line.size()) - 1) {
                char ul{mNormal[row-1][col-1]};
                char ur{mNormal[row-1][col+1]};
                char ll{mNormal[row+1][col-1]};
                char lr{mNormal[row+1][col+1]};

                auto isMS = [](char a){return a == 'M' || a == 'S';};
                if (isMS(ul) && isMS(lr) && ul != lr && isMS(ur) && isMS(ll) && ll != ur) {
                    count2++;
                }
            }
        }
    }

    std::cout << "part 1: " << count << std::endl;
    std::cout << "part 2: " << count2 << std::endl;
}
