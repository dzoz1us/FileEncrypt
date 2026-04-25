#include "CLI.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "FileEncrypt v0.1 — AES-256 File Encryption Tool" << std::endl;
    std::cout << "Build: " << __DATE__ << " " << __TIME__ << std::endl;

    CLI cli;

    if (argc > 1) {
        cli.runBatch(argc, argv);
    } else {
        cli.runInteractive();
    }

    return 0;
}