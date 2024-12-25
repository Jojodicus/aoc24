#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>


class Node {
public:
    std::string outputName;
    Node(std::string outputName) : outputName{outputName} {}
    virtual ~Node() {}
    virtual bool getOutputValue() = 0;
    virtual std::string getDotRepresentation() {
        return "";
    };
};

std::map<std::string, Node*> nodes{};

enum GateType {
    AND, OR, XOR
};

std::string gateToString(GateType gate) {
    switch (gate) {
        case AND:
            return "AND";
        case OR:
            return "OR";
        case XOR:
            return "XOR";
    }
    return "INV";
}

class Constant : public Node {
public:
    bool value;
    Constant(std::string outputName, bool value) : Node{outputName}, value{value} {}

    bool getOutputValue() {
        return value;
    }
};

class Gate : public Node {
public:
    GateType type;
    std::string in1, in2;
    Gate(std::string outputName, GateType type, std::string in1, std::string in2) : Node{outputName}, type(type), in1{in1}, in2{in2} {}

    bool getOutputValue() {
        bool val1 = nodes[in1]->getOutputValue();
        bool val2 = nodes[in2]->getOutputValue();

        switch (type) {
            case AND:
                return val1 && val2;
            case OR:
                return val1 || val2;
            case XOR:
                return val1 ^ val2;
        }

        return false;
    }

    std::string getDotRepresentation() {
        std::string gatename{};
        gatename += gateToString(type) + " " + outputName;
        // return std::format("\t{} -> {}\n\t{} -> {}\n\t{} -> {}\n", in1, gatename, in2, gatename, gatename, outputName);
        return std::format("\t{} [ label = \"{}\" ]\n\t{} -> {}\n\t{} -> {}\n", outputName, gatename, in1, outputName, in2, outputName);
    }
};

void genDotfile(std::string&& name) {
    std::ofstream out{name};
    out << "digraph G {\n";
    for (auto& [k, v] : nodes) {
        out << v->getDotRepresentation();
    }
    out << "}\n";
    out.close();
}

unsigned long buildValue(std::vector<std::string>& signals) {
    unsigned long result{};
    for (auto& z : signals) {
        result <<= 1;
        result |= nodes[z]->getOutputValue();
    }
    return result;
}

int main(int argc, char** argv) {
    // if argument is given, open file and redirect cin
    // otherwise, receive data via stdin
    std::ifstream file;
    if (argc > 1) {
        file = std::ifstream{argv[1]};
        std::cin.rdbuf(file.rdbuf());
    }

    std::string s;
    // start assignments
    std::vector<std::string> xSignals{};
    std::vector<std::string> ySignals{};
    while (std::getline(std::cin, s) && !s.empty()) {
        std::istringstream sstream{s};
        std::getline(sstream, s, ':'); // signal name
        int assign;
        sstream >> assign; // signal start value

        Node* signal = new Constant(s, assign);
        nodes.insert({s, signal});

        if (s.starts_with("x")) {
            xSignals.push_back(s);
        }
        if (s.starts_with("y")) {
            ySignals.push_back(s);
        }
    }

    std::vector<std::string> zSignals{};

    // expressions
    while (std::getline(std::cin, s)) {
        std::istringstream sstream{s};
        std::string sig1, op, sig2;
        sstream >> sig1 >> op >> sig2;
        sstream >> s >> s;

        GateType type;
        if (op == "AND") {
            type = GateType::AND;
        } else if (op == "OR") {
            type = GateType::OR;
        } else {
            type = GateType::XOR;
        }

        Node* expr = new Gate(s, type, sig1, sig2);
        nodes.insert({s, expr});
        if (s.starts_with("z")) {
            zSignals.push_back(s);
        }
    }

    // signal stuff
    std::sort(xSignals.begin(), xSignals.end(), std::greater<>());
    std::sort(ySignals.begin(), ySignals.end(), std::greater<>());
    std::sort(zSignals.begin(), zSignals.end(), std::greater<>());

    // build output
    unsigned long result = buildValue(zSignals);

    std::cout << "part 1: " << result << std::endl;

    genDotfile("day24p1.dot");

    // TODO: fix
    // for my solution, i have to find the patterns manually...
    std::vector<std::string> swapped {
        "z17", "wmp",
        "z39", "qsb",
        "z26", "gvm",
        "gjc", "qjj"
    };
    for (unsigned i{0}; i < swapped.size(); i += 2) {
        std::pair fix{swapped[i], swapped[i+1]};

        Node* first = nodes[fix.first];
        Node* second = nodes[fix.second];

        first->outputName = fix.second;
        second->outputName = fix.first;

        nodes[fix.first] = second;
        nodes[fix.second] = first;
    }

    genDotfile("day24p2.dot");

    // quick check
    std::cout << "part 2: ";
    unsigned long x = buildValue(xSignals);
    unsigned long y = buildValue(ySignals);
    result = buildValue(zSignals);
    if (x + y != result) {
        std::cout << "see dotfile" << std::endl;
    } else {
        // yay!
        std::sort(swapped.begin(), swapped.end());
        bool comma{};
        for (auto& e : swapped) {
            if (comma) {
                std::cout << ',';
            }
            comma = true;
            std::cout << e;
        }
        std::cout << std::endl;
    }
}
