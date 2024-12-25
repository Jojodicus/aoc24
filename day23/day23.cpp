#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

// https://www.geeksforgeeks.org/maximal-clique-problem-recursive-solution/
void bronKerbosch(
    std::set<std::string>&& R, std::set<std::string>&& P,
    std::set<std::string>&& X,
    std::map<std::string, std::set<std::string> >& graph,
    std::set<std::set<std::string> >& cliques)
{
    if (P.empty() && X.empty()) {
        cliques.insert(R);
        return;
    }

    while (!P.empty()) {
        std::string v = *P.begin();
        std::set<std::string> newR = R;
        newR.insert(v);
        std::set<std::string> newP;
        for (std::string p : P) {
            if (graph[v].find(p) != graph[v].end()) {
                newP.insert(p);
            }
        }
        std::set<std::string> newX;
        for (std::string x : X) {
            if (graph[v].find(x) != graph[v].end()) {
                newX.insert(x);
            }
        }
        bronKerbosch(move(newR), move(newP), move(newX),
                     graph, cliques);
        P.erase(v);
        X.insert(v);
    }
}

int main(int argc, char** argv) {
    // if argument is given, open file and redirect cin
    // otherwise, receive data via stdin
    std::ifstream file;
    if (argc > 1) {
        file = std::ifstream{argv[1]};
        std::cin.rdbuf(file.rdbuf());
    }

    std::map<std::string, std::set<std::string>> adjList{};

    std::string line;
    while (std::cin >> line) {
        std::istringstream linestream{line};

        std::string from, to;
        std::getline(linestream, from, '-');
        std::getline(linestream, to, '-');

        // fill adjacency list
        if (!adjList.contains(from)) {
            adjList[from] = {to};
        } else {
            adjList[from].insert(to);
        }
        if (!adjList.contains(to)) {
            adjList[to] = {from};
        } else {
            adjList[to].insert(from);
        }
    }

    std::set<std::set<std::string>> foundCliques{};

    // unoptimized POS
    for (auto& [k1, v1] : adjList) {
        if (!k1.starts_with("t")) {
            continue;
        }

        for (auto& [k2, v2] : adjList) {
            if (k1 == k2) {
                continue;
            }

            for (auto& [k3, v3] : adjList) {
                if (k1 == k3 || k2 == k3) {
                    continue;
                }

                // clique?
                if (v1.contains(k2) && v1.contains(k3) && v2.contains(k3)) {
                    // clique!
                    std::set<std::string> clique = {k1, k2, k3};
                    foundCliques.insert(clique);
                }
            }
        }
    }

    std::cout << "part 1: " << foundCliques.size() << std::endl;


    // all available nodes
    std::set<std::string> nodes{};
    for (auto& [k, v] : adjList) {
        nodes.insert(k);
    }

    // largest clique
    foundCliques.clear();
    bronKerbosch({}, std::move(nodes), {}, adjList, foundCliques);
    unsigned long maxCliqueSize{};
    std::set<std::string> maxClique{};
    for (const auto& clique : foundCliques) {
        if (clique.size() > maxCliqueSize) {
            maxCliqueSize = clique.size();
            maxClique = clique;
        }
    }

    // ready to print
    std::vector<std::string> nice{};
    for (auto& node : maxClique) {
        nice.push_back(node);
    }
    std::sort(nice.begin(), nice.end());

    bool comma{};
    std::string toPrint{};
    for (auto& node : nice) {
        if (comma) {
            toPrint += ',';
        }
        toPrint += node;
        comma = true;
    }

    std::cout << "part 2: " << toPrint << std::endl;
}
