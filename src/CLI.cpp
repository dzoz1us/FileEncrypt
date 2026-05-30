#include "CLI.h"
#include "FileEncryptor.h"
#include <iostream>
#include <limits>
#include <cstring>

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

bool CLI::runBatch(int argc, char* argv[]) {
    std::string mode, input, output, password;

    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-e") == 0 || std::strcmp(argv[i], "--encrypt") == 0) {
            mode = "encrypt";
            if (i + 1 < argc) input = argv[++i];
        }
        else if (std::strcmp(argv[i], "-d") == 0 || std::strcmp(argv[i], "--decrypt") == 0) {
            mode = "decrypt";
            if (i + 1 < argc) input = argv[++i];
        }
        else if (std::strcmp(argv[i], "-p") == 0 || std::strcmp(argv[i], "--password") == 0) {
            if (i + 1 < argc) password = argv[++i];
        }
        else if (std::strcmp(argv[i], "-o") == 0 || std::strcmp(argv[i], "--output") == 0) {
            if (i + 1 < argc) output = argv[++i];
        }
        else if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0) {
            std::cout << "Usage: fileencrypt [options]\n\n";
            std::cout << "Interactive mode (no arguments):\n";
            std::cout << "  fileencrypt\n\n";
            std::cout << "Batch mode:\n";
            std::cout << "  fileencrypt -e <file> -p <password> [-o <output>]\n";
            std::cout << "  fileencrypt -d <file> -p <password> [-o <output>]\n\n";
            std::cout << "Options:\n";
            std::cout << "  -e, --encrypt <file>   Encrypt specified file\n";
            std::cout << "  -d, --decrypt <file>   Decrypt specified file\n";
            std::cout << "  -p, --password <pwd>   Password for encryption/decryption\n";
            std::cout << "  -o, --output <file>    Output file path (optional)\n";
            std::cout << "  -h, --help             Show this help\n";
            return true;
        }
    }

    if (mode.empty()) {
        std::cerr << "Error: No mode specified. Use -e or -d.\n";
        std::cerr << "Use -h for help.\n";
        return false;
    }

    if (input.empty()) {
        std::cerr << "Error: No input file specified.\n";
        return false;
    }

    if (password.empty()) {
        std::cerr << "Error: No password specified. Use -p.\n";
        return false;
    }

    // Если output не указан, формируем автоматически
    if (output.empty()) {
        if (mode == "encrypt") {
            output = input + ".enc";
        } else {
            // Убираем .enc, если есть, иначе добавляем .dec
            if (input.size() > 4 && input.substr(input.size() - 4) == ".enc") {
                output = input.substr(0, input.size() - 4) + ".dec";
            } else {
                output = input + ".dec";
            }
        }
    }

    FileEncryptor fe;
    bool ok = false;

    if (mode == "encrypt") {
        ok = fe.encrypt(input, output, password);
    } else {
        ok = fe.decrypt(input, output, password);
    }

    return ok;
}