#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "FileEncryptor.h"

/**
 * Scenario 3: Corrupted File
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
    std::cout << "  SCENARIO 3: Corrupted File\n";
    std::cout << "========================================\n\n";

    std::cout << "[1/4] Creating and encrypting test file...\n";
    std::string originalContent = "Important document.\nSignature: John Doe.\n";
    std::vector<uint8_t> originalData(originalContent.begin(), originalContent.end());
    writeFile("scenario3_original.txt", originalData);

    FileEncryptor fe;
    if (!fe.encrypt("scenario3_original.txt", "scenario3_encrypted.enc", "SecurePassword")) {
        std::cerr << "      ENCRYPTION FAILED!\n";
        return 1;
    }
    std::cout << "      Encryption successful.\n\n";

    std::cout << "[2/4] Corrupting the encrypted file...\n";
    auto encryptedData = readFile("scenario3_encrypted.enc");
    if (encryptedData.size() < 50) {
        std::cerr << "      File too small to corrupt.\n";
        return 1;
    }
    size_t pos = 38 + (encryptedData.size() - 38) / 2;
    encryptedData[pos] ^= 0xFF;
    writeFile("scenario3_corrupted.enc", encryptedData);
    std::cout << "      Flipped byte at position " << pos << " (in ciphertext).\n\n";

    std::cout << "[3/4] Attempting to decrypt the CORRUPTED file...\n";
    bool decryptOk = fe.decrypt("scenario3_corrupted.enc",
                                "scenario3_decrypted.txt",
                                "SecurePassword");

    std::cout << "\n[4/4] Checking result...\n";

    if (!decryptOk) {
        std::cout << "      Decryption FAILED (as expected).\n";
        std::cout << "      Program detected corrupted data.\n\n";
        std::cout << "========================================\n";
        std::cout << "  SCENARIO 3: PASSED\n";
        std::cout << "========================================\n";
    } else {
        auto decryptedData = readFile("scenario3_decrypted.txt");
        if (decryptedData == originalData) {
            std::cerr << "      Decryption SUCCEEDED and data MATCHES original.\n";
            std::cerr << "      (This is unexpected for a corrupted file!)\n\n";
            std::cerr << "========================================\n";
            std::cerr << "  SCENARIO 3: FAILED\n";
            std::cerr << "========================================\n";
            std::remove("scenario3_original.txt");
            std::remove("scenario3_encrypted.enc");
            std::remove("scenario3_corrupted.enc");
            std::remove("scenario3_decrypted.txt");
            return 1;
        } else {
            std::cout << "      Decryption reported success, but data is CORRUPTED.\n";
            std::cout << "      Original size: " << originalData.size() << "\n";
            std::cout << "      Decrypted size: " << decryptedData.size() << "\n";
            std::cout << "      Data does NOT match original (as expected).\n\n";
            std::cout << "========================================\n";
            std::cout << "  SCENARIO 3: PASSED\n";
            std::cout << "========================================\n";
        }
    }

    std::remove("scenario3_original.txt");
    std::remove("scenario3_encrypted.enc");
    std::remove("scenario3_corrupted.enc");
    std::remove("scenario3_decrypted.txt");

    return 0;
}