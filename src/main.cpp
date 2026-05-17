#include "CLI.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "FileEncrypt v0.2 — AES-256 File Encryption Tool\n";
    std::cout << "Build: " << __DATE__ << " " << __TIME__ << "\n";

    CLI cli;
    if (argc > 1)
        cli.runBatch(argc, argv);
    else
        cli.runInteractive();

    return 0;
}