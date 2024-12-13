#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>

// the comments will be using the first example for concrete documentation

// stolen from learncpp.com
bool approxEq(double a, double b, double epsilon = 1e-8) {
    return (std::abs(a - b) <= (std::max(std::abs(a), std::abs(b)) * epsilon));
}

long costOf(long ax, long ay, long bx, long by, long px, long py) {
    double d_ax{static_cast<double>(ax)}; // 94
    double d_ay{static_cast<double>(ay)}; // 34
    double d_bx{static_cast<double>(bx)}; // 22
    double d_by{static_cast<double>(by)}; // 67
    double d_px{static_cast<double>(px)}; // 8400
    double d_py{static_cast<double>(py)}; // 5400

    // --> 94*A + 22*B = 8400
    //     34*A + 67*B = 5400

    // check for unique solution. task is worded such that non-unique exist, but in practice there are none
    if (approxEq(d_ax/d_ay, d_bx/d_by) && approxEq(d_bx/d_by, d_px/d_py)) {
        assert(false);
    }

    // Gauss:
    // 94*A + 22*B = 8400
    //      + ((94/34) * 67 - 22)*B = (94/34) * 5400 - 8400
    // -> B = ((94/34) * 5400 - 8400) / ((94/34) * 67 - 22) = 40
    double d_B = ((d_ax * d_py/d_ay) - d_px) / ((d_ax * d_by/d_ay)- d_bx);
    long B = round(d_B);
    if (!approxEq(d_B, B)) {
        // no integer solution
        return 0;
    }

    // 94*A + 22*40 = 8400
    // -> A = (8400 - 22*40) / 94
    long A = (px - bx*B) / ax; // if not an integer -> not a solution

    // check
    if (A*ax + B*bx != px || A*ay + B*by != py) {
        return 0;
    }

    // pay up buddy
    return 3*A + B;
}

int main(int argc, char** argv) {
    // if argument is given, open file and redirect cin
    // otherwise, receive data via stdin
    std::ifstream file;
    if (argc > 1) {
        file = std::ifstream{argv[1]};
        std::cin.rdbuf(file.rdbuf());
    }

    unsigned long cost{};
    unsigned long costCorrected{};

    std::string _s;
    char _c;
    while (!std::cin.eof()) {
        long ax, ay, bx, by, px, py;

        std::cin >> _s; // "Button"
        std::cin >> _s; // "A:"
        std::cin >> _c >> _c; // "X+"
        std::cin >> ax; // 94
        std::cin >> _c >> _c >> _c; // ", Y+"
        std::cin >> ay; // 34

        std::cin >> _s; // "Button"
        std::cin >> _s; // "B:"
        std::cin >> _c >> _c; // "X+"
        std::cin >> bx; // 22
        std::cin >> _c >> _c >> _c; // ", Y+"
        std::cin >> by; // 67

        std::cin >> _s; // "Prize:"
        std::cin >> _c >> _c; // "X="
        std::cin >> px; // 8400
        std::cin >> _c >> _c >> _c; // ", Y="
        std::cin >> py; // 5400

        constexpr long adjustment{10000000000000};
        cost += costOf(ax, ay, bx, by, px, py);
        costCorrected += costOf(ax, ay, bx, by, px + adjustment, py + adjustment);
    }

    std::cout << "part 1: " << cost << std::endl;
    std::cout << "part 2: " << costCorrected << std::endl;
}
