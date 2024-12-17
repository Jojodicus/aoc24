#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

using ull_t = unsigned long long;

struct State {
    unsigned pc;
    ull_t a;
    ull_t b;
    ull_t c;
};

ull_t combo(State& state, int operand) {
    switch (operand) {
        case 0:
        case 1:
        case 2:
        case 3:
            return operand;
        case 4:
            return state.a;
        case 5:
            return state.b;
        case 6:
            return state.c;
        default:
            assert(false);
    }
    return 0; // unreachable
}

// 0
int adv(State& state, int operand) {
    state.a >>= combo(state, operand);
    state.pc += 2;
    return 0;
}

// 1
int bxl(State& state, int operand) {
    state.b ^= operand;
    state.pc += 2;
    return 0;
}

// 2
int bst(State& state, int operand) {
    state.b = combo(state, operand) & 0b111;
    state.pc += 2;
    return 0;
}

// 3
int jnz(State& state, int operand) {
    if (!state.a) {
        state.pc += 2;
        return 0;
    }

    state.pc = operand;
    return 0;
}

// 4
int bxc(State& state, int operand) {
    (void) operand;
    state.b ^= state.c;
    state.pc += 2;
    return 0;
}

// 5
int out(State& state, int operand) {
    int output = combo(state, operand) & 0b111;
    state.pc += 2;
    return output;
}

// 6
int bdv(State& state, int operand) {
    state.b = state.a >> combo(state, operand);
    state.pc += 2;
    return 0;
}

// 7
int cdv(State& state, int operand) {
    state.c = state.a >> combo(state, operand);
    state.pc += 2;
    return 0;
}

std::vector<int> run(std::vector<int>& program, State state) {
    int (*operations[])(State&, int) = {
        adv, bxl, bst, jnz, bxc, out, bdv, cdv
    };

    std::vector<int> output{};

    while (state.pc < program.size()) {
        bool shouldOutput = program[state.pc] == 5;

        int opOutput = operations[program[state.pc]](state, program[state.pc + 1]);

        if (shouldOutput) {
            output.push_back(opOutput);
        }
    }

    return output;
}

bool runQuine(std::vector<int>& program, State state) {
    int (*operations[])(State&, int) = {
        adv, bxl, bst, jnz, bxc, out, bdv, cdv
    };

    // ull_t a = state.a;

    unsigned programIndex{};

    while (state.pc < program.size()) {
        bool shouldOutput = program[state.pc] == 5;

        int opOutput = operations[program[state.pc]](state, program[state.pc + 1]);

        if (shouldOutput) {
            if (programIndex >= program.size() || program[programIndex++] != opOutput) {
                // if (programIndex > 9) {
                //     std::cout << std::oct << a << std::dec << std::endl;
                // }
                return false;
            }
        }
    }

    return programIndex == program.size();
}

std::string toString(std::vector<int>& vec) {
    std::string stringOutput{};
    for (auto i : vec) {
        if (stringOutput != "") {
            stringOutput += ',';
        }
        stringOutput += '0' + i;
    }
    return stringOutput;
}

int main(int argc, char** argv) {
    // if argument is given, open file and redirect cin
    // otherwise, receive data via stdin
    std::ifstream file;
    if (argc > 1) {
        file = std::ifstream{argv[1]};
        std::cin.rdbuf(file.rdbuf());
    }

    State state{};

    std::string s; // dump
    std::cin >> s >> s;

    std::cin >> state.a;
    std::cin >> s >> s;
    std::cin >> state.b;
    std::cin >> s >> s;
    std::cin >> state.c;
    std::cin >> s;

    std::vector<int> program{};
    while (std::getline(std::cin, s, ',')) {
        int mem;

        std::istringstream sstream{s};
        sstream >> mem;
        program.push_back(mem);
    }

    std::vector<int> defaultOutput = run(program, state);

    std::cout << "part 1: " << toString(defaultOutput) << std::endl;

    // this is more or less hard-coded for my specific input:
    // length of program: 16
    // a will only ever be shifted right by 3
    // -> a needs 16 * 3 = 48 bits -> 134217728 numbers in search space

    // first 21 digits are kinda-known from earlier brute-force attempts (see commented code above)
    ull_t candidateLast1 = 03267275;
    ull_t candidateLast2 = 03267675;
    // depending on the input, one offset might be enough, or you need more

    for (ull_t a = 1ull << 47; a < 1ull << 48; a += 1 << 20) {
        State aState1 = {0, a + candidateLast1, state.b, state.c};
        if (runQuine(program, aState1)) {
            std::cout << "part 2: " << a + candidateLast1 << std::endl;
            break;
        }

        State aState2 = {0, a + candidateLast2, state.b, state.c};
        if (runQuine(program, aState2)) {
            std::cout << "part 2: " << a + candidateLast2 << std::endl;
            break;
        }
    }
}
