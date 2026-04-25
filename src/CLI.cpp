#include "CLI.h"
#include "FileEncryptor.h"
#include <iostream>
#include <string>

void CLI::runInteractive() {
    std::cout << "=== FileEncrypt v0.1 ===" << std::endl;
    std::cout << "1. Encrypt file" << std::endl;
    std::cout << "2. Decrypt file" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "> ";

    int choice;
    std::cin >> choice;

    if (choice == 0) {
        std::cout << "Goodbye." << std::endl;
        return;
    }

    std::string src, dst, password;
    std::cout << "Enter source file path: ";
    std::cin >> src;
    std::cout << "Enter output file path: ";
    std::cin >> dst;
    std::cout << "Enter password: ";
    std::cin >> password;

    FileEncryptor fe;
    bool ok = false;

    if (choice == 1) {
        ok = fe.encrypt(src, dst, password);
    } else if (choice == 2) {
        ok = fe.decrypt(src, dst, password);
    }

    if (ok)
        std::cout << "Operation completed (stub)." << std::endl;
    else
        std::cout << "Operation failed." << std::endl;
}

void CLI::runBatch(int argc, char* argv[]) {
    std::cout << "[FileEncryptor] Batch mode is not implemented yet." << std::endl;
    std::cout << "Usage: fileencrypt -e|-d <file> -p <password> [-o <output>]" << std::endl;
}