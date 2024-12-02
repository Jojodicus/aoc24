#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

bool isReportSafe(std::vector<int>& report) {
    assert(report.size() > 1);

    int drag = report.at(0);
    int direction = report.at(1) - drag;
    for (unsigned i{1}; i < report.size(); ++i) {
        int current{report[i]};

        // check direction
        int currentDirection{current - drag};
        if (direction * currentDirection <= 0) {
            // not monotonic -> unsafe
            return false;
        }

        // check distance of levels
        int distance{std::abs(currentDirection)};
        if (distance < 1 || 3 < distance) {
            // too big -> unsafe
            return false;
        }

        drag = current;
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

    unsigned long safeReports{};
    unsigned long safeReportsDampened{};
    std::vector<int> report{};

    for (std::string line; std::getline(std::cin, line);) {
        report.clear();

        // build vector
        int num;
        std::stringstream lineStream{line};
        while (lineStream >> num) {
            report.push_back(num);
        }

        bool safe = isReportSafe(report);
        if (safe) {
            safeReports++;
            safeReportsDampened++;
        } else {
            // remove one number at a time and check safety again
            // yes, this is inefficient, but idc
            for (unsigned i{0}; i < report.size(); ++i) {
                // adjusted report
                std::vector dampenedReport{report};
                dampenedReport.erase(dampenedReport.begin() + i);
                if (isReportSafe(dampenedReport)) {
                    safeReportsDampened++;
                    break;
                }
            }
        }
    }

    std::cout << "part 1: " << safeReports << std::endl;
    std::cout << "part 1: " << safeReportsDampened << std::endl;
}
