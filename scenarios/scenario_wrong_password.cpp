#include <iostream>
#include <fstream>
#include <cstdint>
#include "FileEncryptor.h"

/**
 * Scenario 2: Wrong Password
 */

void writeFile(const std::string& path, const std::string& content) {
    std::ofstream f(path, std::ios::binary);
    f.write(content.data(), content.size());
}

bool fileExists(const std::string& path) {
    std::ifstream f(path);
    return f.good();
}

int main() {
    std::cout << "========================================\n";
    std::cout << "  SCENARIO 2: Wrong Password\n";
    std::cout << "========================================\n\n";

    std::cout << "[1/3] Creating test file: scenario2_original.txt\n";
    writeFile("scenario2_original.txt", "SECRET DATA: Credit card = 1234-5678-9012-3456");
    std::cout << "      File created.\n\n";

    std::cout << "[2/3] Encrypting with password 'CorrectPassword'...\n";
    FileEncryptor fe;
    bool encryptOk = fe.encrypt("scenario2_original.txt",
                                "scenario2_encrypted.enc",
                                "CorrectPassword");
    if (!encryptOk) {
        std::cerr << "      ENCRYPTION FAILED!\n";
        return 1;
    }
    std::cout << "      Encryption successful.\n\n";

    std::cout << "[3/3] Attempting decryption with password 'WrongPassword'...\n";
    bool decryptOk = fe.decrypt("scenario2_encrypted.enc",
                                "scenario2_decrypted.txt",
                                "WrongPassword");

    if (!decryptOk) {
        std::cout << "      Decryption FAILED (as expected).\n";
        if (!fileExists("scenario2_decrypted.txt")) {
            std::cout << "      No garbage file created (good).\n\n";
        }
        std::cout << "========================================\n";
        std::cout << "  SCENARIO 2: PASSED\n";
        std::cout << "========================================\n";
        std::cout << "  Program correctly rejected wrong password.\n";
    } else {
        std::cerr << "      Decryption SUCCEEDED (UNEXPECTED!)\n\n";
        std::cerr << "========================================\n";
        std::cerr << "  SCENARIO 2: FAILED\n";
        std::cerr << "========================================\n";
        std::remove("scenario2_decrypted.txt");
        std::remove("scenario2_original.txt");
        std::remove("scenario2_encrypted.enc");
        return 1;
    }

    std::remove("scenario2_original.txt");
    std::remove("scenario2_encrypted.enc");

    return 0;
}