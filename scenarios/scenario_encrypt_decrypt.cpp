#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include "FileEncryptor.h"

/**
 * Scenario 1: Encrypt -> Decrypt -> Compare
 */

std::vector<uint8_t> readFile(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f.is_open()) return {};
    return std::vector<uint8_t>((std::istreambuf_iterator<char>(f)),
                                 std::istreambuf_iterator<char>());
}

void writeFile(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream f(path, std::ios::binary);
    f.write(reinterpret_cast<const char*>(data.data()), data.size());
}

int main() {
    std::cout << "========================================\n";
    std::cout << "  SCENARIO 1: Encrypt and Decrypt\n";
    std::cout << "========================================\n\n";

    std::string testData = "This is a test file for encryption.\n"
                           "It contains multiple lines.\n"
                           "Line 3: Hello, World!\n"
                           "Line 4: 1234567890!@#$%^&*()\n";
    std::vector<uint8_t> originalData(testData.begin(), testData.end());

    std::cout << "[1/4] Creating test file: scenario1_original.txt\n";
    writeFile("scenario1_original.txt", originalData);
    std::cout << "      Size: " << originalData.size() << " bytes\n\n";

    std::cout << "[2/4] Encrypting with password 'MySecretPassword123'...\n";
    FileEncryptor fe;
    bool encryptOk = fe.encrypt("scenario1_original.txt",
                                "scenario1_encrypted.enc",
                                "MySecretPassword123");
    if (!encryptOk) {
        std::cerr << "      ENCRYPTION FAILED!\n";
        return 1;
    }
    std::cout << "      Encryption successful.\n\n";

    std::cout << "[3/4] Decrypting with the SAME password...\n";
    bool decryptOk = fe.decrypt("scenario1_encrypted.enc",
                                "scenario1_decrypted.txt",
                                "MySecretPassword123");
    if (!decryptOk) {
        std::cerr << "      DECRYPTION FAILED!\n";
        return 1;
    }
    std::cout << "      Decryption successful.\n\n";

    std::cout << "[4/4] Comparing original and decrypted files...\n";
    auto decryptedData = readFile("scenario1_decrypted.txt");
    
    if (originalData == decryptedData) {
        std::cout << "      FILES MATCH!\n\n";
        std::cout << "========================================\n";
        std::cout << "  SCENARIO 1: PASSED\n";
        std::cout << "========================================\n";
    } else {
        std::cout << "      FILES DO NOT MATCH!\n\n";
        std::cout << "========================================\n";
        std::cout << "  SCENARIO 1: FAILED\n";
        std::cout << "========================================\n";
        return 1;
    }

    std::remove("scenario1_original.txt");
    std::remove("scenario1_encrypted.enc");
    std::remove("scenario1_decrypted.txt");

    return 0;
}