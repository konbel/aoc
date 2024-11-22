#include "../includes.h"
#include "day25.h"

#include <unordered_set>

int count_nodes(const map<string, std::unordered_set<string>> &graph, const std::unordered_set<string> &modules, const string &module) {
    int count = 0;

    for (const auto &node : graph.at(module)) {
        if (!modules.contains(node)) {
            count++;
        }
    }

    return count;
}

int count_all_nodes(const map<string, std::unordered_set<string>> &graph, const std::unordered_set<string> &modules, const std::unordered_set<string> &nodes) {
    int sum = 0;

    for (const auto &node : nodes) {
        sum += count_nodes(graph, modules, node);
    }

    return sum;
}

void day25::solve(const string& input) {
    if (std::ifstream file(input); file.is_open()) {
        map<string, std::unordered_set<string>> graph;
        std::unordered_set<string> all_modules;
        std::unordered_set<string> modules;

        string line;
        while (getline(file, line)) {
            string name = line.substr(0, 3);
            all_modules.insert(name);
            modules.insert(name);

            string cur;
            for (int i = 5; i < line.size(); i++) {
                if (line[i] == ' ') {
                    all_modules.insert(cur);
                    modules.insert(cur);

                    graph[cur].insert(name);
                    graph[name].insert(cur);

                    cur.clear();
                    i++;
                }

                cur += line[i];
            }

            all_modules.insert(cur);
            modules.insert(cur);

            graph[cur].insert(name);
            graph[name].insert(cur);
        }

        while (count_all_nodes(graph, modules, modules) != 3) {
            int max_count = 0;
            string max_node;

            for (const auto &node : modules) {
                int count = count_nodes(graph, modules, node);

                if (count >= max_count) {
                    max_count = count;
                    max_node = node;
                }
            }

            modules.erase(max_node);
        }

        std::unordered_set<string> complementary;
        for (const auto &node : all_modules) {
            if (!modules.contains(node)) {
                complementary.insert(node);
            }
        }

        std::cout << "Solution problem 1: " << modules.size() * complementary.size() << std::endl;

        file.close();
    } else std::cout << "Can't open file" << std::endl;
}