#pragma once
#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>

class CryptoEngine {
public:
    CryptoEngine();
    ~CryptoEngine();

    CryptoEngine(const CryptoEngine&) = delete;
    CryptoEngine& operator=(const CryptoEngine&) = delete;

    std::vector<uint8_t> generateSalt();
    std::vector<uint8_t> generateIV();
    std::vector<uint8_t> deriveKey(const std::string& password,
                                   const std::vector<uint8_t>& salt);
    bool initEncrypt(const std::vector<uint8_t>& key,
                     const std::vector<uint8_t>& iv);
    bool initDecrypt(const std::vector<uint8_t>& key,
                     const std::vector<uint8_t>& iv);
    bool update(const std::vector<uint8_t>& input,
                std::vector<uint8_t>& output);
    bool finalize(std::vector<uint8_t>& output);
    void cleanup();

private:
    struct Impl;
    Impl* pImpl;
    std::vector<uint8_t> m_key;
};