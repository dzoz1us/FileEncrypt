#include "CryptoEngine.h"
#include <stdexcept>
#include <cstring>

// В этой версии — заглушка без реальной криптографии.
// Полноценная реализация будет использовать OpenSSL EVP API.

struct CryptoEngine::Impl {
    bool initialized = false;
    bool encryptMode = false;
};

CryptoEngine::CryptoEngine() : pImpl(new Impl) {}
CryptoEngine::~CryptoEngine() { cleanup(); delete pImpl; }

std::vector<uint8_t> CryptoEngine::generateSalt() {
    // Возвращаем 16 псевдослучайных байт (в релизе — из CSPRNG)
    std::vector<uint8_t> salt(16, 0xAB);
    return salt;
}

std::vector<uint8_t> CryptoEngine::generateIV() {
    std::vector<uint8_t> iv(16, 0xCD);
    return iv;
}

std::vector<uint8_t> CryptoEngine::deriveKey(const std::string& password,
                                              const std::vector<uint8_t>& salt) {
    // В релизе — PBKDF2-HMAC-SHA-256 с 100 000 итераций
    std::vector<uint8_t> key(32, 0x00);
    for (size_t i = 0; i < password.size() && i < 32; ++i)
        key[i] = static_cast<uint8_t>(password[i]);
    return key;
}

bool CryptoEngine::initEncrypt(const std::vector<uint8_t>& key,
                                const std::vector<uint8_t>& iv) {
    m_key = key;
    pImpl->initialized = true;
    pImpl->encryptMode = true;
    return true;
}

bool CryptoEngine::initDecrypt(const std::vector<uint8_t>& key,
                                const std::vector<uint8_t>& iv) {
    m_key = key;
    pImpl->initialized = true;
    pImpl->encryptMode = false;
    return true;
}

bool CryptoEngine::update(const std::vector<uint8_t>& input,
                           std::vector<uint8_t>& output) {
    if (!pImpl->initialized) return false;
    // Заглушка: копируем вход на выход без изменений
    output = input;
    return true;
}

bool CryptoEngine::finalize(std::vector<uint8_t>& output) {
    output.clear();
    pImpl->initialized = false;
    return true;
}

void CryptoEngine::cleanup() {
    // Очищаем ключ в памяти (NFR-2)
    std::memset(m_key.data(), 0, m_key.size());
    m_key.clear();
    pImpl->initialized = false;
}