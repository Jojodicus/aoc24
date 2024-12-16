#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <unordered_set>
#include <vector>

enum Facing {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

std::pair<int, int> delta[] = {
    {-1,  0}, // north
    { 0,  1}, // east
    { 1,  0}, // south
    { 0, -1}  // west
};

Facing rotate(Facing prev, int direction) {
    return static_cast<Facing>((prev + direction + 4) % 4);
}

struct State {
    int row;
    int col;
    Facing facing;
    unsigned cost;

    bool operator<(const State& other) const {
        // flipped, so max-queue will be min-queue
        return other.cost < cost;
    }

    bool operator==(const State& other) const {
        return row == other.row && col == other.col && facing == other.facing;
    }

    struct Hasher {
        size_t operator()(const State& state) const {
            auto intHasher = std::hash<int>();
            return intHasher(state.row) ^ intHasher(state.col) ^ intHasher(state.facing);
        }
    };
};

int main(int argc, char** argv) {
    // if argument is given, open file and redirect cin
    // otherwise, receive data via stdin
    std::ifstream file;
    if (argc > 1) {
        file = std::ifstream{argv[1]};
        std::cin.rdbuf(file.rdbuf());
    }

    std::vector<std::vector<bool>> walls{};
    std::pair<int, int> player{};
    std::pair<int, int> finish{};

    std::string line;
    int row{};
    int col{};
    while (std::getline(std::cin, line)) {
        std::istringstream linestream{line};

        col = 0;
        std::vector<bool> rowWall{};

        char c;
        while (linestream >> c) {
            rowWall.push_back(c == '#');

            if (c == 'S') {
                player = {row, col};
            }
            if (c == 'E') {
                finish = {row, col};
            }

            col++;
        }

        walls.push_back(rowWall);
        row++;
    }

    // uhm
    std::vector<std::vector<std::vector<std::vector<State>>>> seatBacktrack(row, std::vector(col, std::vector(4, std::vector<State>())));

    State endState{};

    std::priority_queue<State> queue{};
    std::unordered_set<State, State::Hasher> visited{};
    queue.push({player.first, player.second, EAST, 0});

    // bfs
    while (!queue.empty()) {
        State state{queue.top()};
        queue.pop();

        // prune
        if (visited.contains(state)) {
            continue;
        }
        visited.insert(state);

        if (state.row == finish.first && state.col == finish.second) {
            // found solution
            endState = state;
            break;
        }

        std::vector<State> extensions{
            {state.row, state.col, rotate(state.facing, -1), state.cost + 1000}, // anti-clockwise
            {state.row, state.col, rotate(state.facing, 1), state.cost + 1000} // clockwise
        };

        // try to do a step
        int stepRow = state.row + delta[state.facing].first;
        int stepCol = state.col + delta[state.facing].second;
        if (!walls[stepRow][stepCol]) {
            extensions.push_back({stepRow, stepCol, state.facing, state.cost + 1});
        }

        // try each extension
        for (auto& ex : extensions) {
            queue.push(ex);
            std::vector<State>& localSeatVec = seatBacktrack[ex.row][ex.col][ex.facing];
            if (localSeatVec.empty() || localSeatVec.at(0).cost == ex.cost) {
                // the states saved have their cost offset by one position
                // we just use the State struct to make things easier
                localSeatVec.push_back({state.row, state.col, state.facing, ex.cost});
            }
        }
    }

    // avoid infinite loop, starting pos has no parent
    seatBacktrack[player.first][player.second][EAST].clear();

    std::set<std::pair<int, int>> seatSet{};
    std::vector<State> workList{endState};
    while (!workList.empty()) {
        State s{workList.back()};
        workList.pop_back();

        seatSet.insert({s.row, s.col});

        for (auto& e : seatBacktrack[s.row][s.col][s.facing]) {
            workList.push_back(e);
        }
    }

    // viz
    for (unsigned r = 0; r < walls.size(); r++) {
        for (unsigned c = 0; c < walls[r].size(); c++) {
            if (walls[r][c]) {
                std::cout << '#';
            } else if (seatSet.count({r, c})) {
                std::cout << 'O';
            } else {
                std::cout << '.';
            }
        }
        std::cout << std::endl;
    }

    std::cout << "part 1: " << endState.cost << std::endl;
    std::cout << "part 2: " << seatSet.size() << std::endl;
}
