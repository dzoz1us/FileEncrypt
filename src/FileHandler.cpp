#include "FileHandler.h"
#include <fstream>
#include <stdexcept>

struct FileHandler::Impl {
    std::fstream file;
    uint64_t fileSizeBytes = 0;
    bool writeMode = false;
};

FileHandler::FileHandler() : pImpl(new Impl) {}
FileHandler::~FileHandler() { close(); delete pImpl; }

bool FileHandler::openRead(const std::string& path) {
    pImpl->file.open(path, std::ios::in | std::ios::binary);
    if (!pImpl->file.is_open()) return false;
    pImpl->writeMode = false;
    // Определяем размер
    pImpl->file.seekg(0, std::ios::end);
    pImpl->fileSizeBytes = static_cast<uint64_t>(pImpl->file.tellg());
    pImpl->file.seekg(0, std::ios::beg);
    return true;
}

bool FileHandler::openWrite(const std::string& path) {
    pImpl->file.open(path, std::ios::out | std::ios::binary);
    pImpl->writeMode = true;
    return pImpl->file.is_open();
}

std::vector<uint8_t> FileHandler::readChunk(size_t size) {
    std::vector<uint8_t> buffer(size);
    pImpl->file.read(reinterpret_cast<char*>(buffer.data()), size);
    auto bytesRead = pImpl->file.gcount();
    if (bytesRead == 0) return {};
    buffer.resize(static_cast<size_t>(bytesRead));
    return buffer;
}

bool FileHandler::writeChunk(const std::vector<uint8_t>& data) {
    pImpl->file.write(reinterpret_cast<const char*>(data.data()), data.size());
    return pImpl->file.good();
}

uint64_t FileHandler::fileSize() const {
    return pImpl->fileSizeBytes;
}

bool FileHandler::isOpen() const {
    return pImpl->file.is_open();
}

void FileHandler::close() {
    if (pImpl->file.is_open())
        pImpl->file.close();
}