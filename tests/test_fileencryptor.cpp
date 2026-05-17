#include <gtest/gtest.h>
#include "FileEncryptor.h"
#include <fstream>
#include <cstdio>

// Вспомогательная функция: прочитать файл в вектор
std::vector<uint8_t> readFile(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    return std::vector<uint8_t>((std::istreambuf_iterator<char>(f)),
                                 std::istreambuf_iterator<char>());
}

// Вспомогательная функция: записать вектор в файл
void writeFile(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream f(path, std::ios::binary);
    f.write(reinterpret_cast<const char*>(data.data()), data.size());
}

// ============================================================
// Интеграционные тесты
// ============================================================

TEST(FileEncryptorTest, EncryptDecrypt_RoundTrip_TextFile) {
    writeFile("roundtrip_test.txt", {'H','e','l','l','o',' ','W','o','r','l','d'});

    FileEncryptor fe;
    ASSERT_TRUE(fe.encrypt("roundtrip_test.txt", "roundtrip_test.enc", "password123"));
    ASSERT_TRUE(fe.decrypt("roundtrip_test.enc", "roundtrip_test_dec.txt", "password123"));

    auto original = readFile("roundtrip_test.txt");
    auto decrypted = readFile("roundtrip_test_dec.txt");
    EXPECT_EQ(original, decrypted);

    std::remove("roundtrip_test.txt");
    std::remove("roundtrip_test.enc");
    std::remove("roundtrip_test_dec.txt");
}

TEST(FileEncryptorTest, EncryptDecrypt_RoundTrip_BinaryData) {
    std::vector<uint8_t> binary(256);
    for (int i = 0; i < 256; i++) binary[i] = static_cast<uint8_t>(i);
    writeFile("binary_test.dat", binary);

    FileEncryptor fe;
    ASSERT_TRUE(fe.encrypt("binary_test.dat", "binary_test.enc", "key12345"));
    ASSERT_TRUE(fe.decrypt("binary_test.enc", "binary_test_dec.dat", "key12345"));

    auto decrypted = readFile("binary_test_dec.dat");
    EXPECT_EQ(binary, decrypted);

    std::remove("binary_test.dat");
    std::remove("binary_test.enc");
    std::remove("binary_test_dec.dat");
}

TEST(FileEncryptorTest, EncryptDecrypt_RoundTrip_EmptyFile) {
    writeFile("empty_test.txt", {});

    FileEncryptor fe;
    ASSERT_TRUE(fe.encrypt("empty_test.txt", "empty_test.enc", "pass"));
    ASSERT_TRUE(fe.decrypt("empty_test.enc", "empty_test_dec.txt", "pass"));

    auto decrypted = readFile("empty_test_dec.txt");
    EXPECT_TRUE(decrypted.empty());

    std::remove("empty_test.txt");
    std::remove("empty_test.enc");
    std::remove("empty_test_dec.txt");
}

TEST(FileEncryptorTest, Decrypt_WrongPassword_Fails) {
    writeFile("wrongpw_test.txt", {'S','e','c','r','e','t'});

    FileEncryptor fe;
    ASSERT_TRUE(fe.encrypt("wrongpw_test.txt", "wrongpw_test.enc", "correct"));

    // Расшифровка с НЕВЕРНЫМ паролем
    EXPECT_FALSE(fe.decrypt("wrongpw_test.enc", "wrongpw_test_dec.txt", "WRONG"));

    std::remove("wrongpw_test.txt");
    std::remove("wrongpw_test.enc");
}

TEST(FileEncryptorTest, Decrypt_CorruptedFile_Fails) {
    writeFile("corrupted_test.txt", {'D','a','t','a'});

    FileEncryptor fe;
    ASSERT_TRUE(fe.encrypt("corrupted_test.txt", "corrupted_test.enc", "pass"));

    // Портим зашифрованный файл (меняем байт в данных)
    auto data = readFile("corrupted_test.enc");
    if (data.size() > 50) data[50] ^= 0xFF; // инвертируем байт
    writeFile("corrupted_test.enc", data);

    EXPECT_FALSE(fe.decrypt("corrupted_test.enc", "corrupted_test_dec.txt", "pass"));

    std::remove("corrupted_test.txt");
    std::remove("corrupted_test.enc");
}

TEST(FileEncryptorTest, Decrypt_InvalidFile_Fails) {
    writeFile("invalid_test.enc", {'N','O','T','_','F','E','N','C',0,0,0,0,0,0,0,0,0,0});

    FileEncryptor fe;
    EXPECT_FALSE(fe.decrypt("invalid_test.enc", "invalid_test_dec.txt", "pass"));

    std::remove("invalid_test.enc");
}

TEST(FileEncryptorTest, Encrypt_SourceNotFound_Fails) {
    FileEncryptor fe;
    EXPECT_FALSE(fe.encrypt("nonexistent_file.xyz", "output.enc", "pass"));
}