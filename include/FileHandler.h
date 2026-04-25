#pragma once

#include <cstdint>
#include <string>
#include <vector>

/**
 * @brief Абстракция над файловым вводом-выводом.
 * Поблочное чтение и запись бинарных файлов.
 */
class FileHandler {
public:
    FileHandler();
    ~FileHandler();

    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;

    /**
     * @brief Открыть файл на чтение.
     */
    bool openRead(const std::string& path);

    /**
     * @brief Открыть файл на запись.
     */
    bool openWrite(const std::string& path);

    /**
     * @brief Прочитать очередной блок данных указанного размера.
     * @param size Размер блока в байтах
     * @return Прочитанные данные или пустой вектор при EOF/ошибке
     */
    std::vector<uint8_t> readChunk(size_t size);

    /**
     * @brief Записать блок данных в файл.
     */
    bool writeChunk(const std::vector<uint8_t>& data);

    /**
     * @brief Получить размер открытого файла в байтах.
     */
    uint64_t fileSize() const;

    /**
     * @brief Проверить, открыт ли файл.
     */
    bool isOpen() const;

    /**
     * @brief Закрыть файл.
     */
    void close();

private:
    struct Impl;
    Impl* pImpl;
};