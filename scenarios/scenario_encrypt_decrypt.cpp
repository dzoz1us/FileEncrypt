#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "FileEncryptor.h"
#include <cstdint>
#include <windows.h>

/**
 * Сценарий 1: Полный цикл шифрования и расшифровки.
 * 
 * 1. Создаётся тестовый файл с известным содержимым.
 * 2. Файл шифруется с заданным паролем.
 * 3. Зашифрованный файл расшифровывается с тем же паролем.
 * 4. Результат сравнивается с исходным содержимым.
 * 5. Выводится вердикт: успех или провал.
 */

// Чтение файла в вектор байт
std::vector<uint8_t> readFile(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f.is_open()) return {};
    return std::vector<uint8_t>((std::istreambuf_iterator<char>(f)),
                                 std::istreambuf_iterator<char>());
}

// Запись вектора байт в файл
void writeFile(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream f(path, std::ios::binary);
    f.write(reinterpret_cast<const char*>(data.data()), data.size());
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "========================================\n";
    std::cout << "  SCENARIO 1: Encrypt and Decrypt\n";
    std::cout << "========================================\n\n";

    // Шаг 1: Создаём тестовый файл
    std::string testData = "This is a test file for encryption.\n"
                           "It contains multiple lines.\n"
                           "Line 3: Hello, World!\n"
                           "Line 4: 1234567890!@#$%^&*()\n";
    std::vector<uint8_t> originalData(testData.begin(), testData.end());

    std::cout << "[1/4] Creating test file: scenario1_original.txt\n";
    writeFile("scenario1_original.txt", originalData);
    std::cout << "      Size: " << originalData.size() << " bytes\n\n";

    // Шаг 2: Шифруем
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

    // Шаг 3: Расшифровываем
    std::cout << "[3/4] Decrypting with the SAME password...\n";
    bool decryptOk = fe.decrypt("scenario1_encrypted.enc",
                                "scenario1_decrypted.txt",
                                "MySecretPassword123");
    if (!decryptOk) {
        std::cerr << "      DECRYPTION FAILED!\n";
        return 1;
    }
    std::cout << "      Decryption successful.\n\n";

    // Шаг 4: Сравниваем
    std::cout << "[4/4] Comparing original and decrypted files...\n";
    auto decryptedData = readFile("scenario1_decrypted.txt");
    
    if (originalData == decryptedData) {
        std::cout << "      FILES MATCH!\n\n";
        std::cout << "========================================\n";
        std::cout << "  SCENARIO 1: PASSED\n";
        std::cout << "========================================\n";
    } else {
        std::cout << "      FILES DO NOT MATCH!\n";
        std::cout << "      Original size: " << originalData.size() << "\n";
        std::cout << "      Decrypted size: " << decryptedData.size() << "\n\n";
        std::cout << "========================================\n";
        std::cout << "  SCENARIO 1: FAILED\n";
        std::cout << "========================================\n";
        return 1;
    }

    // Очистка
    std::remove("scenario1_original.txt");
    std::remove("scenario1_encrypted.enc");
    std::remove("scenario1_decrypted.txt");

    return 0;
}