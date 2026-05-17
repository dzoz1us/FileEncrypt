#include <gtest/gtest.h>
#include "CryptoEngine.h"

// ============================================================
// Тесты generateSalt()
// ============================================================

TEST(CryptoEngineTest, GenerateSalt_Returns16Bytes) {
    CryptoEngine engine;
    auto salt = engine.generateSalt();
    EXPECT_EQ(salt.size(), 16);
}

TEST(CryptoEngineTest, GenerateSalt_TwoCallsGiveDifferentResults) {
    CryptoEngine engine;
    auto salt1 = engine.generateSalt();
    auto salt2 = engine.generateSalt();
    EXPECT_NE(salt1, salt2); // Вероятность совпадения ничтожна
}

TEST(CryptoEngineTest, GenerateSalt_NotEmpty) {
    CryptoEngine engine;
    auto salt = engine.generateSalt();
    std::vector<uint8_t> zeros(16, 0x00);
    EXPECT_NE(salt, zeros); // Соль не должна быть нулевой
}

// ============================================================
// Тесты generateIV()
// ============================================================

TEST(CryptoEngineTest, GenerateIV_Returns16Bytes) {
    CryptoEngine engine;
    auto iv = engine.generateIV();
    EXPECT_EQ(iv.size(), 16);
}

TEST(CryptoEngineTest, GenerateIV_TwoCallsGiveDifferentResults) {
    CryptoEngine engine;
    auto iv1 = engine.generateIV();
    auto iv2 = engine.generateIV();
    EXPECT_NE(iv1, iv2);
}

// ============================================================
// Тесты deriveKey()
// ============================================================

TEST(CryptoEngineTest, DeriveKey_Returns32Bytes) {
    CryptoEngine engine;
    auto salt = engine.generateSalt();
    auto key = engine.deriveKey("password123", salt);
    EXPECT_EQ(key.size(), 32);
}

TEST(CryptoEngineTest, DeriveKey_SamePasswordSameSalt_ProducesSameKey) {
    CryptoEngine engine;
    std::vector<uint8_t> salt(16, 0x42);
    auto key1 = engine.deriveKey("mypassword", salt);
    auto key2 = engine.deriveKey("mypassword", salt);
    EXPECT_EQ(key1, key2);
}

TEST(CryptoEngineTest, DeriveKey_DifferentPassword_ProducesDifferentKey) {
    CryptoEngine engine;
    std::vector<uint8_t> salt(16, 0x42);
    auto key1 = engine.deriveKey("password1", salt);
    auto key2 = engine.deriveKey("password2", salt);
    EXPECT_NE(key1, key2);
}

TEST(CryptoEngineTest, DeriveKey_DifferentSalt_ProducesDifferentKey) {
    CryptoEngine engine;
    std::vector<uint8_t> salt1(16, 0x11);
    std::vector<uint8_t> salt2(16, 0x22);
    auto key1 = engine.deriveKey("samepassword", salt1);
    auto key2 = engine.deriveKey("samepassword", salt2);
    EXPECT_NE(key1, key2);
}

TEST(CryptoEngineTest, DeriveKey_EmptyPasswordWorks) {
    CryptoEngine engine;
    auto salt = engine.generateSalt();
    auto key = engine.deriveKey("", salt);
    EXPECT_EQ(key.size(), 32);
}

TEST(CryptoEngineTest, DeriveKey_LongPasswordWorks) {
    CryptoEngine engine;
    auto salt = engine.generateSalt();
    std::string longPwd(1000, 'x');
    auto key = engine.deriveKey(longPwd, salt);
    EXPECT_EQ(key.size(), 32);
}

// ============================================================
// Тесты initEncrypt / initDecrypt
// ============================================================

TEST(CryptoEngineTest, InitEncrypt_ReturnsTrue) {
    CryptoEngine engine;
    std::vector<uint8_t> key(32, 0x01);
    std::vector<uint8_t> iv(16, 0x02);
    EXPECT_TRUE(engine.initEncrypt(key, iv));
}

TEST(CryptoEngineTest, InitDecrypt_ReturnsTrue) {
    CryptoEngine engine;
    std::vector<uint8_t> key(32, 0x01);
    std::vector<uint8_t> iv(16, 0x02);
    EXPECT_TRUE(engine.initDecrypt(key, iv));
}

// ============================================================
// Тесты update() и finalize() — полный цикл
// ============================================================

TEST(CryptoEngineTest, EncryptDecrypt_RoundTrip_SmallData) {
    CryptoEngine enc, dec;
    std::vector<uint8_t> key(32, 0xAA);
    std::vector<uint8_t> iv(16, 0xBB);
    std::vector<uint8_t> plaintext = {'H','e','l','l','o',',',' ','W','o','r','l','d','!'};

    // Шифруем
    ASSERT_TRUE(enc.initEncrypt(key, iv));
    std::vector<uint8_t> ciphertext;
    ASSERT_TRUE(enc.update(plaintext, ciphertext));
    std::vector<uint8_t> finalEnc;
    ASSERT_TRUE(enc.finalize(finalEnc));
    ciphertext.insert(ciphertext.end(), finalEnc.begin(), finalEnc.end());

    // Расшифровываем
    ASSERT_TRUE(dec.initDecrypt(key, iv));
    std::vector<uint8_t> decrypted;
    ASSERT_TRUE(dec.update(ciphertext, decrypted));
    std::vector<uint8_t> finalDec;
    ASSERT_TRUE(dec.finalize(finalDec));
    decrypted.insert(decrypted.end(), finalDec.begin(), finalDec.end());

    EXPECT_EQ(decrypted, plaintext);
}

TEST(CryptoEngineTest, EncryptDecrypt_RoundTrip_EmptyData) {
    CryptoEngine enc, dec;
    std::vector<uint8_t> key(32, 0xAA);
    std::vector<uint8_t> iv(16, 0xBB);
    std::vector<uint8_t> plaintext;

    ASSERT_TRUE(enc.initEncrypt(key, iv));
    std::vector<uint8_t> ciphertext;
    ASSERT_TRUE(enc.update(plaintext, ciphertext));
    std::vector<uint8_t> finalEnc;
    ASSERT_TRUE(enc.finalize(finalEnc));
    ciphertext.insert(ciphertext.end(), finalEnc.begin(), finalEnc.end());

    ASSERT_TRUE(dec.initDecrypt(key, iv));
    std::vector<uint8_t> decrypted;
    ASSERT_TRUE(dec.update(ciphertext, decrypted));
    std::vector<uint8_t> finalDec;
    ASSERT_TRUE(dec.finalize(finalDec));
    decrypted.insert(decrypted.end(), finalDec.begin(), finalDec.end());

    EXPECT_EQ(decrypted, plaintext);
}

TEST(CryptoEngineTest, EncryptDecrypt_RoundTrip_ExactBlockSize) {
    CryptoEngine enc, dec;
    std::vector<uint8_t> key(32, 0xAA);
    std::vector<uint8_t> iv(16, 0xBB);
    std::vector<uint8_t> plaintext(16, 0x42); // Ровно 16 байт

    ASSERT_TRUE(enc.initEncrypt(key, iv));
    std::vector<uint8_t> ciphertext;
    ASSERT_TRUE(enc.update(plaintext, ciphertext));
    std::vector<uint8_t> finalEnc;
    ASSERT_TRUE(enc.finalize(finalEnc));
    ciphertext.insert(ciphertext.end(), finalEnc.begin(), finalEnc.end());

    ASSERT_TRUE(dec.initDecrypt(key, iv));
    std::vector<uint8_t> decrypted;
    ASSERT_TRUE(dec.update(ciphertext, decrypted));
    std::vector<uint8_t> finalDec;
    ASSERT_TRUE(dec.finalize(finalDec));
    decrypted.insert(decrypted.end(), finalDec.begin(), finalDec.end());

    EXPECT_EQ(decrypted, plaintext);
}

TEST(CryptoEngineTest, EncryptDecrypt_RoundTrip_LargeData) {
    CryptoEngine enc, dec;
    std::vector<uint8_t> key(32, 0xAA);
    std::vector<uint8_t> iv(16, 0xBB);
    std::vector<uint8_t> plaintext(4096, 0x55); // 4 КБ

    ASSERT_TRUE(enc.initEncrypt(key, iv));
    std::vector<uint8_t> ciphertext;
    ASSERT_TRUE(enc.update(plaintext, ciphertext));
    std::vector<uint8_t> finalEnc;
    ASSERT_TRUE(enc.finalize(finalEnc));
    ciphertext.insert(ciphertext.end(), finalEnc.begin(), finalEnc.end());

    ASSERT_TRUE(dec.initDecrypt(key, iv));
    std::vector<uint8_t> decrypted;
    ASSERT_TRUE(dec.update(ciphertext, decrypted));
    std::vector<uint8_t> finalDec;
    ASSERT_TRUE(dec.finalize(finalDec));
    decrypted.insert(decrypted.end(), finalDec.begin(), finalDec.end());

    EXPECT_EQ(decrypted, plaintext);
}

TEST(CryptoEngineTest, Decrypt_WrongKey_FailsFinalize) {
    CryptoEngine enc, dec;
    std::vector<uint8_t> key1(32, 0xAA);
    std::vector<uint8_t> key2(32, 0xBB);
    std::vector<uint8_t> iv(16, 0xCC);
    std::vector<uint8_t> plaintext = {'T','e','s','t'};

    ASSERT_TRUE(enc.initEncrypt(key1, iv));
    std::vector<uint8_t> ciphertext;
    ASSERT_TRUE(enc.update(plaintext, ciphertext));
    std::vector<uint8_t> finalEnc;
    ASSERT_TRUE(enc.finalize(finalEnc));
    ciphertext.insert(ciphertext.end(), finalEnc.begin(), finalEnc.end());

    ASSERT_TRUE(dec.initDecrypt(key2, iv)); // ДРУГОЙ ключ
    std::vector<uint8_t> decrypted;
    ASSERT_TRUE(dec.update(ciphertext, decrypted));
    std::vector<uint8_t> finalDec;
    // Финализация должна провалиться из-за неверного паддинга
    EXPECT_FALSE(dec.finalize(finalDec));
}

TEST(CryptoEngineTest, Update_BeforeInit_Fails) {
    CryptoEngine engine;
    std::vector<uint8_t> input(16, 0x00);
    std::vector<uint8_t> output;
    EXPECT_FALSE(engine.update(input, output));
}

TEST(CryptoEngineTest, Finalize_BeforeInit_Fails) {
    CryptoEngine engine;
    std::vector<uint8_t> output;
    EXPECT_FALSE(engine.finalize(output));
}

TEST(CryptoEngineTest, Cleanup_AllowsReuse) {
    CryptoEngine engine;
    std::vector<uint8_t> key(32, 0x01);
    std::vector<uint8_t> iv(16, 0x02);

    // Первый цикл
    ASSERT_TRUE(engine.initEncrypt(key, iv));
    std::vector<uint8_t> dummy1, dummy2;
    engine.update({1,2,3}, dummy1);
    engine.finalize(dummy2);
    engine.cleanup();

    // Второй цикл — должно работать после cleanup
    ASSERT_TRUE(engine.initDecrypt(key, iv));
    std::vector<uint8_t> dummy3, dummy4;
    engine.update({1,2,3,4,5}, dummy3);
    engine.finalize(dummy4);
}