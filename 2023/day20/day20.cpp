#include "../includes.h"
#include "day20.h"

#include <cassert>
#include <__numeric/gcd_lcm.h>

enum ModuleType {
    BROADCASTER,
    FLIPFLOP,
    CONJUNCTION
};

struct Module {
    string name;
    ModuleType type;
    vector<string> destinations;
    std::unordered_map<string, bool> memory;
    bool isOn;
};

struct Pulse {
    Pulse(const string& sender, const string& receiver, const bool& pulse) {
        this->sender = sender;
        this->receiver = receiver;
        this->pulse = pulse;
    }

    string sender;
    string receiver;
    bool pulse;
};

static int push_button(map<string, Module>& modules, const int& count) {
    int low_pulses_sent = 0;
    int high_pulses_sent = 0;

    for (int i = 0; i < count; i++) {
        vector<Pulse> pulse_queue;

        pulse_queue.push_back(Pulse("button", "broadcaster", false));
        low_pulses_sent++;

        while (!pulse_queue.empty()) {
            Pulse pulse = pulse_queue[0];
            bool pulse_to_send = pulse.pulse;

            if (!modules.contains(pulse.receiver)) {
                pulse_queue.erase(pulse_queue.begin(), pulse_queue.begin() + 1);
                continue;
            }
            Module& module = modules[pulse.receiver];

            if (module.type == ModuleType::FLIPFLOP) {
                if (pulse.pulse == false) {
                    module.isOn = !module.isOn;
                    if (module.isOn == true) pulse_to_send = true;
                    else pulse_to_send = false;
                } else {
                    pulse_queue.erase(pulse_queue.begin(), pulse_queue.begin() + 1);
                    continue;
                }
            } else if (module.type == ModuleType::CONJUNCTION) {
                module.memory[pulse.sender] = pulse.pulse;
                int high_pulses = 0;
                for (auto [_, memory_pulse] : module.memory) {
                    if (memory_pulse == true) high_pulses++;
                }

                if (high_pulses == module.memory.size()) pulse_to_send = false;
                else pulse_to_send = true;
            }

            for (const string& destination : module.destinations) {
                if (pulse_to_send == true) high_pulses_sent++;
                else low_pulses_sent++;

                pulse_queue.push_back(Pulse(module.name, destination, pulse_to_send));
                // std::cout << pulse.receiver << " -" << (pulse_to_send ? "high" : "low") << " -> " << destination << std::endl;
            }

            pulse_queue.erase(pulse_queue.begin(), pulse_queue.begin() + 1);
        }
    }

    return high_pulses_sent * low_pulses_sent;
}

static long lowest_to_rx(map<string, Module>& modules) {
    Module feed;
    for (const auto& pair : modules) {
        for (const string& destination : pair.second.destinations) {
            if (destination == "rx") {
                feed = pair.second;
                break;
            }
        }
    }

    map<string, int> cycle_lengths;
    map<string, int> seen;
    for (const auto& pair : modules) {
        for (const string& destination : pair.second.destinations) {
            if (destination == feed.name) seen[pair.first] = 0;
        }
    }

    int button_presses = 0;
    while (true) {
        button_presses++;
        vector<Pulse> pulse_queue;

        pulse_queue.push_back(Pulse("button", "broadcaster", false));
        // std::cout << "button -low -> boardcaster" << std::endl;

        while (!pulse_queue.empty()) {
            Pulse pulse = pulse_queue[0];
            bool pulse_to_send = pulse.pulse;

            if (!modules.contains(pulse.receiver)) {
                pulse_queue.erase(pulse_queue.begin(), pulse_queue.begin() + 1);
                continue;
            }
            Module& module = modules[pulse.receiver];

            if (pulse.receiver == feed.name && pulse.pulse == true) {
                seen[pulse.sender]++;

                if (!cycle_lengths.contains(pulse.sender)) cycle_lengths[pulse.sender] = button_presses;
                else assert(button_presses == seen[pulse.sender] * cycle_lengths[pulse.sender]);

                int has_value = 0;
                for (const auto& value : seen) {
                    if (value.second != 0) has_value++;
                }

                if (has_value == seen.size()) {
                    long x = 1;
                    for (const auto& cycle_length : cycle_lengths) x = std::lcm(x, cycle_length.second);
                    return x;
                }
            }

            if (module.type == ModuleType::FLIPFLOP) {
                if (pulse.pulse == false) {
                    module.isOn = !module.isOn;
                    if (module.isOn == true) pulse_to_send = true;
                    else pulse_to_send = false;
                } else {
                    pulse_queue.erase(pulse_queue.begin(), pulse_queue.begin() + 1);
                    continue;
                }
            } else if (module.type == ModuleType::CONJUNCTION) {
                module.memory[pulse.sender] = pulse.pulse;
                int high_pulses = 0;
                for (auto [_, memory_pulse] : module.memory) {
                    if (memory_pulse == true) high_pulses++;
                }

                if (high_pulses == module.memory.size()) pulse_to_send = false;
                else pulse_to_send = true;
            }

            for (const string& destination : module.destinations) {
                pulse_queue.push_back(Pulse(module.name, destination, pulse_to_send));
                // std::cout << pulse.receiver << " -" << (pulse_to_send ? "high" : "low") << " -> " << destination << std::endl;
            }

            pulse_queue.erase(pulse_queue.begin(), pulse_queue.begin() + 1);
        }
    }

}

void day20::solve(const string &input) {
    if (std::ifstream file(input); file.is_open()) {
        map<string, Module> modules;
        vector<string> conjunction_modules;

        string line;
        while (getline(file, line)) {
            Module module;

            if (isalpha(line[0])) {
                module.name = "broadcaster";
                module.type = BROADCASTER;

                line.erase(0, 12);
            } else {
                if (line[0] == '%') {
                    module.type = ModuleType::FLIPFLOP;
                    module.isOn = false;
                } else if (line[0] == '&') module.type = CONJUNCTION;

                for (int i = 1; i < line.size(); i++) {
                    if (line[i] == ' ') {
                        line.erase(0, i + 1);
                        break;
                    }
                    module.name += line[i];
                }

                if (module.type == CONJUNCTION) conjunction_modules.push_back(module.name);
            }

            line.erase(0, 3);
            line += ',';

            vector<string> destinations;
            string destination;
            for (int i = 0; i < line.size(); i++) {
                if (line[i] == ',') {
                    destinations.push_back(destination);
                    destination.clear();
                    i++;
                    continue;
                }
                destination += line[i];
            }
            module.destinations = destinations;

            modules.insert(pair(module.name, module));
        }
        for (string c : conjunction_modules) {
            for (pair<string, Module> pair : modules) {
                const Module& mod = pair.second;
                for (string dest : mod.destinations) {
                    if (dest == c) modules[c].memory[mod.name] = false;
                }
            }
        }

        map<string, Module> modules_p1 = modules;

        int result_p1 = push_button(modules_p1, 1000);
        long result_p2 = lowest_to_rx(modules);

        std::cout << "Solution problem 1: " << result_p1 << std::endl;
        std::cout << "Solution problem 2: " << result_p2 << std::endl;

        file.close();
    } else printf("Can't open file");
}