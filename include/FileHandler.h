#pragma once
#include <cstdint>
#include <string>
#include <vector>

class FileHandler {
public:
    FileHandler();
    ~FileHandler();
    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;

    bool openRead(const std::string& path);
    bool openWrite(const std::string& path);
    std::vector<uint8_t> readChunk(size_t size);
    bool writeChunk(const std::vector<uint8_t>& data);
    uint64_t fileSize() const;
    bool isOpen() const;
    void close();

private:
    struct Impl;
    Impl* pImpl;
};