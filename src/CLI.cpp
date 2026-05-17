#include "CLI.h"
#include "FileEncryptor.h"
#include <iostream>
#include <limits>

void CLI::runInteractive() {
    while (true) {
        std::cout << "\n=== FileEncrypt v0.2 ===\n";
        std::cout << "1. Encrypt file\n";
        std::cout << "2. Decrypt file\n";
        std::cout << "0. Exit\n";
        std::cout << "> ";

        int choice;
        std::cin >> choice;

        if (choice == 0) {
            std::cout << "Goodbye.\n";
            break;
        }

        if (choice != 1 && choice != 2) {
            std::cout << "Invalid choice. Try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::string src, dst, pwd;
        std::cout << "Enter source file path: ";
        std::cin >> src;
        std::cout << "Enter output file path: ";
        std::cin >> dst;
        std::cout << "Enter password: ";
        std::cin >> pwd;

        FileEncryptor fe;
        bool ok = false;

        if (choice == 1) {
            std::cout << "Encrypting...\n";
            ok = fe.encrypt(src, dst, pwd);
        } else {
            std::cout << "Decrypting...\n";
            ok = fe.decrypt(src, dst, pwd);
        }

        if (ok)
            std::cout << "Operation completed successfully.\n";
        else
            std::cout << "Operation failed.\n";

        std::cout << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
}

void CLI::runBatch(int argc, char* argv[]) {
    std::cout << "Batch mode not implemented yet.\n";
    std::cout << "Usage: fileencrypt -e|-d <file> -p <password> [-o <output>]\n";
}