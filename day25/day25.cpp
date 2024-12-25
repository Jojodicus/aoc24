#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

using keyway_t = std::vector<int>;

constexpr int KEYWAY_HEIGHT{5};

int main(int argc, char** argv) {
    // if argument is given, open file and redirect cin
    // otherwise, receive data via stdin
    std::ifstream file;
    if (argc > 1) {
        file = std::ifstream{argv[1]};
        std::cin.rdbuf(file.rdbuf());
    }

    std::set<keyway_t> locks{};
    std::set<keyway_t> keys{};

    std::string s;
    while (std::cin >> s) {
        std::set<keyway_t>* relevant_set;

        if (s.starts_with("#")) {
            // lock
            relevant_set = &locks;
        } else {
            // key
            relevant_set = &keys;
        }

        // build keyway
        keyway_t keyway(5, 0);
        for (int i{0}; i < KEYWAY_HEIGHT; ++i) {
            std::cin >> s;
            int runIndex{};
            for (char c : s) {
                keyway[runIndex++] += c == '#';
            }
        }

        // discard last row
        std::cin >> s;

        relevant_set->insert(keyway);
    }

    unsigned long matches{};
    for (auto& lock : locks) {
        for (auto& key : keys) {
            // fits?
            bool fits{true};
            for (unsigned i{0}; i < lock.size(); ++i) {
                if (lock[i] + key[i] > KEYWAY_HEIGHT) {
                    fits = false;
                    break;
                }
            }

            matches += fits;
        }
    }

    std::cout << "part 1: " << matches << std::endl;
}
