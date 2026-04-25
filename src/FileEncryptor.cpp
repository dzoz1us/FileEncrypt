#include "FileEncryptor.h"
#include "CryptoEngine.h"
#include "FileHeader.h"
#include "FileHandler.h"
#include <iostream>

bool FileEncryptor::encrypt(const std::string& srcPath,
                             const std::string& dstPath,
                             const std::string& password) {
    return processFile(srcPath, dstPath, password, Direction::Encrypt);
}

bool FileEncryptor::decrypt(const std::string& srcPath,
                             const std::string& dstPath,
                             const std::string& password) {
    return processFile(srcPath, dstPath, password, Direction::Decrypt);
}

bool FileEncryptor::processFile(const std::string& srcPath,
                                 const std::string& dstPath,
                                 const std::string& password,
                                 Direction direction) {
    // Шаблонный метод — каркас алгоритма.
    // В данной промежуточной версии реализация упрощена.
    // Полноценная версия будет включать все шаги, описанные в SRS.

    CryptoEngine engine;
    FileHeader header;
    FileHandler srcFile, dstFile;

    // Генерируем или будем извлекать метаданные
    auto salt = engine.generateSalt();
    auto iv = engine.generateIV();
    auto key = engine.deriveKey(password, salt);

    if (direction == Direction::Encrypt) {
        // В полной версии: упаковка заголовка, поблочное шифрование
        std::cout << "[FileEncryptor] Encrypting... (stub)" << std::endl;
    } else {
        // В полной версии: чтение заголовка, поблочное дешифрование
        std::cout << "[FileEncryptor] Decrypting... (stub)" << std::endl;
    }

    engine.cleanup();
    return true;
}