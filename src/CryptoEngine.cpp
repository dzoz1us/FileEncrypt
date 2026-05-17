#include "CryptoEngine.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <cstring>
#include <stdexcept>

struct CryptoEngine::Impl {
    EVP_CIPHER_CTX* ctx = nullptr;
    bool initialized = false;
    bool encryptMode = false;
};

CryptoEngine::CryptoEngine() : pImpl(new Impl) {}
CryptoEngine::~CryptoEngine() { cleanup(); delete pImpl; }

std::vector<uint8_t> CryptoEngine::generateSalt() {
    std::vector<uint8_t> salt(16);
    if (RAND_bytes(salt.data(), 16) != 1)
        throw std::runtime_error("Failed to generate salt");
    return salt;
}

std::vector<uint8_t> CryptoEngine::generateIV() {
    std::vector<uint8_t> iv(16);
    if (RAND_bytes(iv.data(), 16) != 1)
        throw std::runtime_error("Failed to generate IV");
    return iv;
}

std::vector<uint8_t> CryptoEngine::deriveKey(const std::string& password,
                                              const std::vector<uint8_t>& salt) {
    std::vector<uint8_t> key(32);
    if (PKCS5_PBKDF2_HMAC(password.c_str(), static_cast<int>(password.size()),
                           salt.data(), static_cast<int>(salt.size()),
                           100000, EVP_sha256(), 32, key.data()) != 1)
        throw std::runtime_error("Key derivation failed");
    return key;
}

bool CryptoEngine::initEncrypt(const std::vector<uint8_t>& key,
                                const std::vector<uint8_t>& iv) {
    if (pImpl->ctx) EVP_CIPHER_CTX_free(pImpl->ctx);
    pImpl->ctx = EVP_CIPHER_CTX_new();
    if (!pImpl->ctx) return false;
    if (EVP_EncryptInit_ex(pImpl->ctx, EVP_aes_256_cbc(), nullptr,
                           key.data(), iv.data()) != 1) return false;
    m_key = key;
    pImpl->initialized = true;
    pImpl->encryptMode = true;
    return true;
}

bool CryptoEngine::initDecrypt(const std::vector<uint8_t>& key,
                                const std::vector<uint8_t>& iv) {
    if (pImpl->ctx) EVP_CIPHER_CTX_free(pImpl->ctx);
    pImpl->ctx = EVP_CIPHER_CTX_new();
    if (!pImpl->ctx) return false;
    if (EVP_DecryptInit_ex(pImpl->ctx, EVP_aes_256_cbc(), nullptr,
                           key.data(), iv.data()) != 1) return false;
    m_key = key;
    pImpl->initialized = true;
    pImpl->encryptMode = false;
    return true;
}

bool CryptoEngine::update(const std::vector<uint8_t>& input,
                           std::vector<uint8_t>& output) {
    if (!pImpl->initialized || !pImpl->ctx) return false;
    output.resize(input.size() + EVP_MAX_BLOCK_LENGTH);
    int outLen = 0;
    int ok = pImpl->encryptMode
        ? EVP_EncryptUpdate(pImpl->ctx, output.data(), &outLen,
                            input.data(), static_cast<int>(input.size()))
        : EVP_DecryptUpdate(pImpl->ctx, output.data(), &outLen,
                            input.data(), static_cast<int>(input.size()));
    output.resize(outLen);
    return ok == 1;
}

bool CryptoEngine::finalize(std::vector<uint8_t>& output) {
    if (!pImpl->initialized || !pImpl->ctx) return false;
    EVP_CIPHER_CTX_set_padding(pImpl->ctx, 1);
    output.resize(EVP_MAX_BLOCK_LENGTH);
    int outLen = 0;
    int ok = pImpl->encryptMode
        ? EVP_EncryptFinal_ex(pImpl->ctx, output.data(), &outLen)
        : EVP_DecryptFinal_ex(pImpl->ctx, output.data(), &outLen);
    output.resize(outLen);
    pImpl->initialized = false;
    return ok == 1;
}

void CryptoEngine::cleanup() {
    if (pImpl->ctx) { EVP_CIPHER_CTX_free(pImpl->ctx); pImpl->ctx = nullptr; }
    std::memset(m_key.data(), 0, m_key.size());
    m_key.clear();
    pImpl->initialized = false;
}