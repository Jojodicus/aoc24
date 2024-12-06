#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>
#include <cassert>

struct Node {
    std::set<Node*> parents{};
};

class Graph {
    public:
    std::map<int, Node*> nodes;

    Graph() : nodes{} {
    }

    ~Graph() {
        for (auto [_, node] : nodes) {
            delete node;
        }
    }

    void addEdge(int a, int b) {
        // get or create A
        Node* nodeA;
        if (nodes.count(a)) {
            nodeA = nodes[a];
        } else {
            nodeA = new Node{};
            nodes[a] = nodeA;
        }

        // get or create B
        Node* nodeB;
        if (nodes.count(b)) {
            nodeB = nodes[b];
        } else {
            nodeB = new Node{};
            nodes[b] = nodeB;
        }

        // add edge
        nodeB->parents.insert(nodeA);
    }
};

class Ordering {
    public:
    Graph* graph;
    std::set<Node*> candidates;

    Ordering(Graph* graph) : graph{graph}, candidates{} {
        for (auto [_, node] : graph->nodes) {
            candidates.insert(node);
        }
    }

    void erasePred(Node* node) {
        if (candidates.erase(node)) {
            // was just removed, so remove potential parents as well
            for (Node* parent : node->parents) {
                // erasePred(parent);
                candidates.erase(parent);
            }
        }
    }

    bool advance(int value) {
        assert(graph->nodes.count(value)); // all pages should have rules
        Node* node = graph->nodes[value];
        if (!candidates.count(node)) {
            // wrong topological sorting
            return false;
        }

        // update candidates
        // go up the graph, nodes before our are no longer valid as following pages
        erasePred(node);
        return true;
    }

    std::vector<int> topSort(std::vector<int> booklet) {
        std::vector<int> theNewOrder{};

        // insertion sort
        for (int page : booklet) {
            Node* node = graph->nodes[page];

            int i;
            for (i = theNewOrder.size(); i > 0; i--) {
                Node* prevnode = graph->nodes[theNewOrder.at(i-1)];
                if (node->parents.count(prevnode)) {
                    break;
                }
            }
            theNewOrder.insert(theNewOrder.begin() + i, page);
        }

        return theNewOrder;
    }
};

int main(int argc, char** argv) {
    // if argument is given, open file and redirect cin
    // otherwise, receive data via stdin
    std::ifstream file;
    if (argc > 1) {
        file = std::ifstream{argv[1]};
        std::cin.rdbuf(file.rdbuf());
    }

    // parse graph
    Graph graph{};

    char _;
    std::string s;
    getline(std::cin, s);
    while (s != "") {
        int from, to;
        std::istringstream istream{s};
        istream >> from >> _ >> to; // AAA|BBB

        graph.addEdge(from, to);

        getline(std::cin, s);
    }

    unsigned long part1{};
    unsigned long part2{};
    while (getline(std::cin, s)) {
        int page;
        std::vector<int> booklet{};

        std::istringstream istream{s};
        while (istream >> page) {
            istream >> _; // delete comma if it's there
            booklet.push_back(page);
        }

        // go through topological sort
        Ordering ordering{&graph};
        bool valid{true};
        for (int page : booklet) {
            if (!ordering.advance(page)) {
                valid = false;
                break;
            }
        }

        if (valid) {
            // sum middle value of a valid booklet
            part1 += booklet[booklet.size() / 2];
        } else {
            // fix first, then sum
            std::vector<int> ordered = ordering.topSort(booklet);
            part2 += ordered[ordered.size() / 2];
        }
    }

    std::cout << "part 1: " << part1 << std::endl;
    std::cout << "part 2: " << part2 << std::endl;
}
