#include <gtest/gtest.h>
#include "FileHeader.h"

// ============================================================
// Тесты pack()
// ============================================================

TEST(FileHeaderTest, Pack_CreatesCorrectSize) {
    FileHeader header;
    std::vector<uint8_t> salt(16, 0xAA);
    std::vector<uint8_t> iv(16, 0xBB);

    auto packed = header.pack(salt, iv);

    // Заголовок всегда 38 байт
    EXPECT_EQ(packed.size(), FileHeader::HEADER_SIZE);
}

TEST(FileHeaderTest, Pack_ContainsMagicBytes) {
    FileHeader header;
    std::vector<uint8_t> salt(16, 0x00);
    std::vector<uint8_t> iv(16, 0x00);

    auto packed = header.pack(salt, iv);

    // Первые 4 байта — "FENC"
    EXPECT_EQ(packed[0], 'F');
    EXPECT_EQ(packed[1], 'E');
    EXPECT_EQ(packed[2], 'N');
    EXPECT_EQ(packed[3], 'C');
}

TEST(FileHeaderTest, Pack_ContainsVersion) {
    FileHeader header;
    std::vector<uint8_t> salt(16, 0x00);
    std::vector<uint8_t> iv(16, 0x00);

    auto packed = header.pack(salt, iv);

    // Версия 0x0001 в little-endian
    EXPECT_EQ(packed[4], 0x01);
    EXPECT_EQ(packed[5], 0x00);
}

TEST(FileHeaderTest, Pack_PreservesSalt) {
    FileHeader header;
    std::vector<uint8_t> salt = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    std::vector<uint8_t> iv(16, 0x00);

    auto packed = header.pack(salt, iv);

    // Байты 6-21 — соль
    for (int i = 0; i < 16; i++) {
        EXPECT_EQ(packed[6 + i], salt[i]);
    }
}

TEST(FileHeaderTest, Pack_PreservesIV) {
    FileHeader header;
    std::vector<uint8_t> salt(16, 0x00);
    std::vector<uint8_t> iv = {15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};

    auto packed = header.pack(salt, iv);

    // Байты 22-37 — IV
    for (int i = 0; i < 16; i++) {
        EXPECT_EQ(packed[22 + i], iv[i]);
    }
}

TEST(FileHeaderTest, Pack_DifferentInputsGiveDifferentHeaders) {
    FileHeader header;
    std::vector<uint8_t> salt1(16, 0x11);
    std::vector<uint8_t> iv1(16, 0x22);
    std::vector<uint8_t> salt2(16, 0x33);
    std::vector<uint8_t> iv2(16, 0x44);

    auto packed1 = header.pack(salt1, iv1);
    auto packed2 = header.pack(salt2, iv2);

    // Заголовки должны отличаться
    EXPECT_NE(packed1, packed2);
}

// ============================================================
// Тесты isValid()
// ============================================================

TEST(FileHeaderTest, IsValid_RecognizesCorrectHeader) {
    FileHeader header;
    std::vector<uint8_t> salt(16, 0xAA);
    std::vector<uint8_t> iv(16, 0xBB);

    auto packed = header.pack(salt, iv);

    EXPECT_TRUE(FileHeader::isValid(packed));
}

TEST(FileHeaderTest, IsValid_RejectsTooSmallHeader) {
    std::vector<uint8_t> small(10, 0x00);

    EXPECT_FALSE(FileHeader::isValid(small));
}

TEST(FileHeaderTest, IsValid_RejectsEmptyHeader) {
    std::vector<uint8_t> empty;

    EXPECT_FALSE(FileHeader::isValid(empty));
}

TEST(FileHeaderTest, IsValid_RejectsWrongMagic) {
    FileHeader header;
    std::vector<uint8_t> salt(16, 0x00);
    std::vector<uint8_t> iv(16, 0x00);

    auto packed = header.pack(salt, iv);
    packed[0] = 'X'; // портим magic

    EXPECT_FALSE(FileHeader::isValid(packed));
}

TEST(FileHeaderTest, IsValid_RejectsAllZeros) {
    std::vector<uint8_t> zeros(38, 0x00);

    EXPECT_FALSE(FileHeader::isValid(zeros));
}

// ============================================================
// Тесты unpack()
// ============================================================

TEST(FileHeaderTest, Unpack_ExtractsCorrectSalt) {
    FileHeader header;
    std::vector<uint8_t> originalSalt = {10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160};
    std::vector<uint8_t> iv(16, 0xCC);

    auto packed = header.pack(originalSalt, iv);
    std::vector<uint8_t> extractedSalt, extractedIV;

    ASSERT_TRUE(header.unpack(packed, extractedSalt, extractedIV));
    EXPECT_EQ(extractedSalt, originalSalt);
}

TEST(FileHeaderTest, Unpack_ExtractsCorrectIV) {
    FileHeader header;
    std::vector<uint8_t> salt(16, 0xAA);
    std::vector<uint8_t> originalIV = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

    auto packed = header.pack(salt, originalIV);
    std::vector<uint8_t> extractedSalt, extractedIV;

    ASSERT_TRUE(header.unpack(packed, extractedSalt, extractedIV));
    EXPECT_EQ(extractedIV, originalIV);
}

TEST(FileHeaderTest, Unpack_RoundTripPreservesData) {
    FileHeader header;
    std::vector<uint8_t> salt(16, 0xDE);
    std::vector<uint8_t> iv(16, 0xAD);

    auto packed = header.pack(salt, iv);
    std::vector<uint8_t> extractedSalt, extractedIV;

    ASSERT_TRUE(header.unpack(packed, extractedSalt, extractedIV));
    EXPECT_EQ(salt, extractedSalt);
    EXPECT_EQ(iv, extractedIV);
}

TEST(FileHeaderTest, Unpack_ReturnsFalseOnInvalidHeader) {
    FileHeader header;
    std::vector<uint8_t> invalid(38, 0xFF);
    std::vector<uint8_t> salt, iv;

    EXPECT_FALSE(header.unpack(invalid, salt, iv));
}