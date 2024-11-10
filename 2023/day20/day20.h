//
// Created by Konstantin Kilbel on 20.12.23.
//

#ifndef DAY20_H
#define DAY20_H

namespace day20 {
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

    int pushButton(map<string, Module>& modules, const int& count);
    long lowestToRX(map<string, Module>& modules);
    void solve(string input);
}

#endif //DAY20_H