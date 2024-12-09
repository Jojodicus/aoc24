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

    std::vector<int> disk{};

    int id{};
    bool isFile{true};
    char read;
    int lengthOnDisk;
    while (std::cin >> read) {
        lengthOnDisk = read - '0';
        if (isFile) {
            for (int i{0}; i < lengthOnDisk; ++i) {
                disk.push_back(id);
            }
            id++;
        } else {
            for (int i{0}; i < lengthOnDisk; ++i) {
                disk.push_back(-1);
            }
        }

        isFile = !isFile;
    }

    std::vector<int> disk2{disk};

    // defragment
    for (int i{static_cast<int>(disk.size()) - 1}; i >= 0; --i) {
        for (int j{0}; j < i; ++j) {
            if (disk[j] == -1) {
                disk[j] = disk[i];
                disk[i] = -1;
            }
        }
    }

    // defragment2
    for (int i{static_cast<int>(disk2.size()) - 1}; i >= 0; --i) {
        int lengthBackElement{};
        id = disk2[i];
        while (disk2[i] == id) {
            lengthBackElement++;
            i--;
        }
        i++;

        // find space up front
        for (int j{0}; j < i; ++j) {
            if (disk2[j] == -1) {
                // is space enough?
                int freeSpace{};
                for (int k{j}; k < i && freeSpace < lengthBackElement; ++k) {
                    if (disk2[k] != -1) {
                        break;
                    }
                    freeSpace++;
                }

                if (freeSpace >= lengthBackElement) {
                    // copy over
                    for (int l{0}; l < lengthBackElement; ++l) {
                        disk2[j + l] = disk2[i + l];
                        disk2[i + l] = -1;
                    }
                }
            }
        }
    }

    // checksum
    unsigned long checksum{};
    for (unsigned i{0}; i < disk.size(); ++i) {
        if (disk[i] == -1) {
            continue;
        }

        checksum += i * disk[i];
    }
    unsigned long checksum2{};
    for (unsigned i{0}; i < disk2.size(); ++i) {
        if (disk2[i] == -1) {
            continue;
        }

        checksum2 += i * disk2[i];
    }

    std::cout << "part 1: " << checksum << std::endl;
    std::cout << "part 2: " << checksum2 << std::endl;
}