#pragma once

#include <cstdint>
#include <vector>

/**
 * @brief Формирование и разбор заголовка зашифрованного файла.
 *
 * Структура заголовка:
 *   Magic bytes:  4 байта  "FENC"
 *   Version:      2 байта  0x0001
 *   Salt:        16 байт
 *   IV:          16 байт
 *   Итого:       38 байт
 */
class FileHeader {
public:
    static constexpr uint32_t MAGIC = 0x434E4546; // "FENC" в little-endian
    static constexpr uint16_t VERSION = 0x0001;
    static constexpr size_t   HEADER_SIZE = 38;

    /**
     * @brief Упаковать соль и IV в бинарный заголовок.
     */
    std::vector<uint8_t> pack(const std::vector<uint8_t>& salt,
                              const std::vector<uint8_t>& iv);

    /**
     * @brief Распаковать заголовок, извлечь соль и IV.
     * @return true, если сигнатура и версия корректны
     */
    bool unpack(const std::vector<uint8_t>& rawHeader,
                std::vector<uint8_t>& salt,
                std::vector<uint8_t>& iv);

    /**
     * @brief Проверка корректности заголовка без извлечения данных.
     */
    static bool isValid(const std::vector<uint8_t>& rawHeader);
};