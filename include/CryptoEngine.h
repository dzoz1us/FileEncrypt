#pragma once

#include <cstdint>
#include <string>
#include <vector>

/**
 * @brief Обёртка над OpenSSL EVP API.
 * Адаптирует C-интерфейс OpenSSL к идиоматичному C++.
 * Отвечает за: генерацию соли/IV, деривацию ключа через PBKDF2,
 * инициализацию контекста шифрования/дешифрования, поблочную обработку
 * и финализацию с паддингом.
 */
class CryptoEngine {
public:
    CryptoEngine();
    ~CryptoEngine();

    // Удаляем копирование — владеет уникальным контекстом OpenSSL
    CryptoEngine(const CryptoEngine&) = delete;
    CryptoEngine& operator=(const CryptoEngine&) = delete;

    /**
     * @brief Генерация криптостойкой случайной соли (16 байт).
     */
    std::vector<uint8_t> generateSalt();

    /**
     * @brief Генерация вектора инициализации (16 байт).
     */
    std::vector<uint8_t> generateIV();

    /**
     * @brief Деривация 256-битного ключа из пароля через PBKDF2-HMAC-SHA-256.
     * @param password Пользовательский пароль
     * @param salt Соль (16 байт)
     * @return Ключ (32 байта) либо пустой вектор при ошибке
     */
    std::vector<uint8_t> deriveKey(const std::string& password,
                                   const std::vector<uint8_t>& salt);

    /**
     * @brief Инициализация контекста для шифрования.
     */
    bool initEncrypt(const std::vector<uint8_t>& key,
                     const std::vector<uint8_t>& iv);

    /**
     * @brief Инициализация контекста для дешифрования.
     */
    bool initDecrypt(const std::vector<uint8_t>& key,
                     const std::vector<uint8_t>& iv);

    /**
     * @brief Поблочная обработка данных (шифрование или дешифрование).
     */
    bool update(const std::vector<uint8_t>& input,
                std::vector<uint8_t>& output);

    /**
     * @brief Финализация: добавляет/удаляет паддинг.
     */
    bool finalize(std::vector<uint8_t>& output);

    /**
     * @brief Очистка контекста и чувствительных данных в памяти.
     */
    void cleanup();

private:
    // Указатель на непрозрачную структуру OpenSSL (реализация скрыта)
    struct Impl;
    Impl* pImpl;
    std::vector<uint8_t> m_key; // временное хранение ключа до cleanup
};