#pragma once

#include <cstdint>
#include <string>

/**
 * @brief Фасад системы шифрования файлов.
 * Предоставляет унифицированный интерфейс для шифрования и дешифрования,
 * скрывая взаимодействие CryptoEngine, FileHeader и FileHandler.
 */
class FileEncryptor {
public:
    /**
     * @brief Режим работы.
     */
    enum class Direction { Encrypt, Decrypt };

    /**
     * @brief Зашифровать файл.
     * @param srcPath  Путь к исходному файлу
     * @param dstPath  Путь к выходному (зашифрованному) файлу
     * @param password Пароль
     * @return true в случае успеха
     */
    bool encrypt(const std::string& srcPath,
                 const std::string& dstPath,
                 const std::string& password);

    /**
     * @brief Расшифровать файл.
     * @param srcPath  Путь к зашифрованному файлу
     * @param dstPath  Путь к выходному (расшифрованному) файлу
     * @param password Пароль
     * @return true в случае успеха
     */
    bool decrypt(const std::string& srcPath,
                 const std::string& dstPath,
                 const std::string& password);

private:
    /**
     * @brief Шаблонный метод — общий каркас для encrypt и decrypt.
     */
    bool processFile(const std::string& srcPath,
                     const std::string& dstPath,
                     const std::string& password,
                     Direction direction);

    static constexpr size_t CHUNK_SIZE = 4096;
};