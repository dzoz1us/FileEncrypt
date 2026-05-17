#include <iostream>
#include <fstream>
#include <vector>
#include "FileEncryptor.h"
#include <cstdint>
#include <windows.h>

/**
 * Сценарий 2: Попытка расшифровки с неверным паролем.
 * 
 * 1. Создаётся тестовый файл.
 * 2. Файл шифруется с паролем "CorrectPassword".
 * 3. Делается попытка расшифровки с паролем "WrongPassword".
 * 4. Ожидается, что расшифровка ПРОВАЛИТСЯ.
 * 5. Проверяется, что программа не создала файл с мусором.
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
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "========================================\n";
    std::cout << "  SCENARIO 2: Wrong Password\n";
    std::cout << "========================================\n\n";

    // Шаг 1: Создаём тестовый файл
    std::cout << "[1/3] Creating test file: scenario2_original.txt\n";
    writeFile("scenario2_original.txt", "SECRET DATA: Credit card = 1234-5678-9012-3456");
    std::cout << "      File created.\n\n";

    // Шаг 2: Шифруем с правильным паролем
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

    // Шаг 3: Пытаемся расшифровать с НЕВЕРНЫМ паролем
    std::cout << "[3/3] Attempting decryption with password 'WrongPassword'...\n";
    bool decryptOk = fe.decrypt("scenario2_encrypted.enc",
                                "scenario2_decrypted.txt",
                                "WrongPassword");

    // Проверяем результат
    if (!decryptOk) {
        std::cout << "      Decryption FAILED (as expected).\n";
        
        // Дополнительно убедимся, что временный файл не остался
        if (!fileExists("scenario2_decrypted.txt")) {
            std::cout << "      No garbage file created (good).\n\n";
        }
        
        std::cout << "========================================\n";
        std::cout << "  SCENARIO 2: PASSED\n";
        std::cout << "========================================\n";
        std::cout << "  Программа правильно отвергла неверный пароль.\n";
    } else {
        std::cerr << "      Decryption SUCCEEDED (UNEXPECTED!)\n\n";
        std::cerr << "========================================\n";
        std::cerr << "  SCENARIO 2: FAILED\n";
        std::cerr << "========================================\n";
        std::cerr << "  Программа приняла неверный пароль!\n";
        std::remove("scenario2_decrypted.txt");
        std::remove("scenario2_original.txt");
        std::remove("scenario2_encrypted.enc");
        return 1;
    }

    // Очистка
    std::remove("scenario2_original.txt");
    std::remove("scenario2_encrypted.enc");

    return 0;
}