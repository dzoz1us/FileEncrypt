#pragma once
#include <string>

class CLI {
public:
    void runInteractive();
    bool runBatch(int argc, char* argv[]);
};