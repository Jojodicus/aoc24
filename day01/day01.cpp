#include <algorithm>
#include <execution>
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    // if argument is given, open file and redirect cin
    // otherwise, receive data via stdin
    std::ifstream file;
    if (argc > 1) {
        file = std::ifstream{argv[1]};
        std::cin.rdbuf(file.rdbuf());
    }

    std::vector<long> leftList{};
    std::vector<long> rightList{};

    // split input into left and right column-wise
    while (!std::cin.eof()) {
        long left, right;
        std::cin >> left >> right;
        leftList.push_back(left);
        rightList.push_back(right);
    }

    // sort
    // realistically, it would probably be faster to build a heap during parsing, then doing a heapsort with that
    std::sort(leftList.begin(), leftList.end());
    std::sort(rightList.begin(), rightList.end());

    unsigned long distance{};
    for (unsigned long i = 0; i < leftList.size(); ++i) {
        distance += std::abs(leftList[i] - rightList[i]);
    }

    std::cout << "part 1: " << distance << std::endl;

    unsigned long similarity{}, walkRight{};
    for (unsigned long i = 0; i < leftList.size(); ++i) {
        long left{leftList[i]};
        unsigned long nums{};

        // walk right index along
        while (walkRight < rightList.size() && rightList[walkRight] < left) {
            ++walkRight;
        }

        // count equal numbers
        while (walkRight < rightList.size() && rightList[walkRight] == left) {
            ++nums;
            ++walkRight;
        }

        similarity += left * nums;
    }

    std::cout << "part 2: " << similarity << std::endl;
}
