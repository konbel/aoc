//
// Created by Konstantin Kilbel on 20.12.23.
//

#include "../includes.h"
#include "day20.h"

#include <assert.h>
#include <__numeric/gcd_lcm.h>

int day20::pushButton(map<string, Module>& modules, const int& count) {
    int lowPulsesSent = 0;
    int highPulsesSent = 0;

    for (int i = 0; i < count; i++) {
        vector<Pulse> pulseQueue;

        pulseQueue.push_back(Pulse("button", "broadcaster", false));
        // cout << "button -low -> boardcaster" << endl;
        lowPulsesSent++;

        while (!pulseQueue.empty()) {
            Pulse pulse = pulseQueue[0];
            bool pulseToSend = pulse.pulse;

            if (!modules.contains(pulse.receiver)) {
                pulseQueue.erase(pulseQueue.begin(), pulseQueue.begin() + 1);
                continue;
            }
            Module& module = modules[pulse.receiver];

            if (module.type == FLIPFLOP) {
                if (pulse.pulse == false) {
                    module.isOn = !module.isOn;
                    if (module.isOn == true) pulseToSend = true;
                    else pulseToSend = false;
                } else {
                    pulseQueue.erase(pulseQueue.begin(), pulseQueue.begin() + 1);
                    continue;
                }
            } else if (module.type == CONJUNCTION) {
                module.memory[pulse.sender] = pulse.pulse;
                int highPulses = 0;
                for (auto [_, memoryPulse] : module.memory) {
                    if (memoryPulse == true) highPulses++;
                }

                if (highPulses == module.memory.size()) pulseToSend = false;
                else pulseToSend = true;
            }

            for (const string& destination : module.destinations) {
                if (pulseToSend == true) highPulsesSent++;
                else lowPulsesSent++;

                pulseQueue.push_back(Pulse(module.name, destination, pulseToSend));
                // cout << pulse.receiver << " -" << (pulseToSend ? "high" : "low") << " -> " << destination << endl;
            }

            pulseQueue.erase(pulseQueue.begin(), pulseQueue.begin() + 1);
        }
    }

    return highPulsesSent * lowPulsesSent;
}

long day20::lowestToRX(map<string, Module>& modules) {
    Module feed;
    for (const auto& pair : modules) {
        for (const string& destination : pair.second.destinations) {
            if (destination == "rx") {
                feed = pair.second;
                break;
            }
        }
    }

    map<string, int> cycleLengths;
    map<string, int> seen;
    for (const auto& pair : modules) {
        for (const string& destination : pair.second.destinations) {
            if (destination == feed.name) seen[pair.first] = 0;
        }
    }

    int buttonPresses = 0;
    while (true) {
        buttonPresses++;
        vector<Pulse> pulseQueue;

        pulseQueue.push_back(Pulse("button", "broadcaster", false));
        // cout << "button -low -> boardcaster" << endl;

        while (!pulseQueue.empty()) {
            Pulse pulse = pulseQueue[0];
            bool pulseToSend = pulse.pulse;

            if (!modules.contains(pulse.receiver)) {
                pulseQueue.erase(pulseQueue.begin(), pulseQueue.begin() + 1);
                continue;
            }
            Module& module = modules[pulse.receiver];

            if (pulse.receiver == feed.name && pulse.pulse == true) {
                seen[pulse.sender]++;

                if (!cycleLengths.contains(pulse.sender)) cycleLengths[pulse.sender] = buttonPresses;
                else assert(buttonPresses == seen[pulse.sender] * cycleLengths[pulse.sender]);

                int hasValue = 0;
                for (const auto& value : seen) {
                    if (value.second != 0) hasValue++;
                }

                if (hasValue == seen.size()) {
                    long x = 1;
                    for (const auto& cycleLength : cycleLengths) x = std::lcm(x, cycleLength.second);
                    return x;
                }
            }

            if (module.type == FLIPFLOP) {
                if (pulse.pulse == false) {
                    module.isOn = !module.isOn;
                    if (module.isOn == true) pulseToSend = true;
                    else pulseToSend = false;
                } else {
                    pulseQueue.erase(pulseQueue.begin(), pulseQueue.begin() + 1);
                    continue;
                }
            } else if (module.type == CONJUNCTION) {
                module.memory[pulse.sender] = pulse.pulse;
                int highPulses = 0;
                for (auto [_, memoryPulse] : module.memory) {
                    if (memoryPulse == true) highPulses++;
                }

                if (highPulses == module.memory.size()) pulseToSend = false;
                else pulseToSend = true;
            }

            for (const string& destination : module.destinations) {
                pulseQueue.push_back(Pulse(module.name, destination, pulseToSend));
                // cout << pulse.receiver << " -" << (pulseToSend ? "high" : "low") << " -> " << destination << endl;
            }

            pulseQueue.erase(pulseQueue.begin(), pulseQueue.begin() + 1);
        }
    }

}

void day20::solve(string input) {
    std::ifstream file(input);
    if (file.is_open()) {
        map<string, Module> modules;
        vector<string> cunjunctionModules;

        string line;
        while (getline(file, line)) {
            Module module;

            if (isalpha(line[0])) {
                module.name = "broadcaster";
                module.type = BROADCASTER;

                line.erase(0, 12);
            } else {
                if (line[0] == '%') {
                    module.type = FLIPFLOP;
                    module.isOn = false;
                } else if (line[0] == '&') module.type = CONJUNCTION;

                for (int i = 1; i < line.size(); i++) {
                    if (line[i] == ' ') {
                        line.erase(0, i + 1);
                        break;
                    }
                    module.name += line[i];
                }

                if (module.type == CONJUNCTION) cunjunctionModules.push_back(module.name);
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

            modules.insert(std::pair(module.name, module));
        }
        for (string c : cunjunctionModules) {
            for (std::pair<string, Module> pair : modules) {
                const Module& mod = pair.second;
                for (string dest : mod.destinations) {
                    if (dest == c) modules[c].memory[mod.name] = false;
                }
            }
        }

        map<string, Module> modulesP1 = modules;

        int resultP1 = pushButton(modulesP1, 1000);
        long resultP2 = lowestToRX(modules);

        cout << "Solution problem 1: " << resultP1 << endl;
        cout << "Solution problem 2: " << resultP2 << endl;

        file.close();
    } else printf("Can't open file");
}