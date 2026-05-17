#pragma once
#include <string>

class FileEncryptor {
public:
    enum class Direction { Encrypt, Decrypt };

    bool encrypt(const std::string& srcPath,
                 const std::string& dstPath,
                 const std::string& password);
    bool decrypt(const std::string& srcPath,
                 const std::string& dstPath,
                 const std::string& password);

private:
    bool processFile(const std::string& srcPath,
                     const std::string& dstPath,
                     const std::string& password,
                     Direction direction);
    static constexpr size_t CHUNK_SIZE = 4096;
};