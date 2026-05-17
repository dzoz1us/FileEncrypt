#include <gtest/gtest.h>
#include "FileHandler.h"
#include <fstream>
#include <cstdio>

// Вспомогательная функция: создать временный файл с данными
std::string createTempFile(const std::string& name, const std::vector<uint8_t>& data) {
    std::ofstream f(name, std::ios::binary);
    f.write(reinterpret_cast<const char*>(data.data()), data.size());
    f.close();
    return name;
}

// ============================================================
// Тесты openRead()
// ============================================================

TEST(FileHandlerTest, OpenRead_ExistingFile) {
    createTempFile("test_read.dat", {1,2,3,4,5});
    FileHandler fh;
    EXPECT_TRUE(fh.openRead("test_read.dat"));
    fh.close();
    std::remove("test_read.dat");
}

TEST(FileHandlerTest, OpenRead_NonExistingFile) {
    FileHandler fh;
    EXPECT_FALSE(fh.openRead("this_file_does_not_exist.xyz"));
}

TEST(FileHandlerTest, OpenRead_IsOpenReturnsTrue) {
    createTempFile("test_open.dat", {0x00});
    FileHandler fh;
    fh.openRead("test_open.dat");
    EXPECT_TRUE(fh.isOpen());
    fh.close();
    std::remove("test_open.dat");
}

// ============================================================
// Тесты openWrite()
// ============================================================

TEST(FileHandlerTest, OpenWrite_CreatesFile) {
    FileHandler fh;
    EXPECT_TRUE(fh.openWrite("test_write.dat"));
    EXPECT_TRUE(fh.isOpen());
    fh.close();
    // Проверяем, что файл создан
    std::ifstream check("test_write.dat");
    EXPECT_TRUE(check.good());
    check.close();
    std::remove("test_write.dat");
}

// ============================================================
// Тесты readChunk()
// ============================================================

TEST(FileHandlerTest, ReadChunk_ReturnsCorrectData) {
    std::vector<uint8_t> original = {10,20,30,40,50,60,70,80,90,100};
    createTempFile("test_chunk.dat", original);

    FileHandler fh;
    fh.openRead("test_chunk.dat");
    auto chunk = fh.readChunk(5);
    EXPECT_EQ(chunk.size(), 5);
    EXPECT_EQ(chunk[0], 10);
    EXPECT_EQ(chunk[4], 50);
    fh.close();
    std::remove("test_chunk.dat");
}

TEST(FileHandlerTest, ReadChunk_ReadsEntireFile) {
    std::vector<uint8_t> original = {1,2,3};
    createTempFile("test_full.dat", original);

    FileHandler fh;
    fh.openRead("test_full.dat");
    auto chunk = fh.readChunk(100); // запрашиваем больше, чем есть
    EXPECT_EQ(chunk.size(), 3);
    EXPECT_EQ(chunk, original);
    fh.close();
    std::remove("test_full.dat");
}

TEST(FileHandlerTest, ReadChunk_EndOfFile_ReturnsEmpty) {
    std::vector<uint8_t> original = {1,2,3};
    createTempFile("test_eof.dat", original);

    FileHandler fh;
    fh.openRead("test_eof.dat");
    fh.readChunk(3); // читаем всё
    auto empty = fh.readChunk(10); // читаем дальше
    EXPECT_TRUE(empty.empty());
    fh.close();
    std::remove("test_eof.dat");
}

TEST(FileHandlerTest, ReadChunk_EmptyFile_ReturnsEmpty) {
    createTempFile("test_empty.dat", {});
    FileHandler fh;
    fh.openRead("test_empty.dat");
    auto chunk = fh.readChunk(10);
    EXPECT_TRUE(chunk.empty());
    fh.close();
    std::remove("test_empty.dat");
}

// ============================================================
// Тесты writeChunk()
// ============================================================

TEST(FileHandlerTest, WriteChunk_WritesCorrectData) {
    FileHandler fh;
    fh.openWrite("test_writechunk.dat");
    fh.writeChunk({1,2,3,4,5});
    fh.close();

    // Проверяем содержимое
    std::ifstream check("test_writechunk.dat", std::ios::binary);
    std::vector<uint8_t> contents((std::istreambuf_iterator<char>(check)),
                                   std::istreambuf_iterator<char>());
    std::vector<uint8_t> expected = {1,2,3,4,5};
    EXPECT_EQ(contents, expected);
    check.close();
    std::remove("test_writechunk.dat");
}

TEST(FileHandlerTest, WriteChunk_MultipleWrites) {
    FileHandler fh;
    fh.openWrite("test_multi.dat");
    fh.writeChunk({1,2,3});
    fh.writeChunk({4,5,6});
    fh.close();

    std::ifstream check("test_multi.dat", std::ios::binary);
    std::vector<uint8_t> contents((std::istreambuf_iterator<char>(check)),
                                   std::istreambuf_iterator<char>());
    std::vector<uint8_t> expected = {1,2,3,4,5,6};
    EXPECT_EQ(contents, expected);
    check.close();
    std::remove("test_multi.dat");
}

// ============================================================
// Тесты fileSize()
// ============================================================

TEST(FileHandlerTest, FileSize_ReturnsCorrectValue) {
    std::vector<uint8_t> data(100, 0x42);
    createTempFile("test_size.dat", data);

    FileHandler fh;
    fh.openRead("test_size.dat");
    EXPECT_EQ(fh.fileSize(), 100);
    fh.close();
    std::remove("test_size.dat");
}

// ============================================================
// Тесты close()
// ============================================================

TEST(FileHandlerTest, Close_IsOpenReturnsFalse) {
    createTempFile("test_close.dat", {0x00});
    FileHandler fh;
    fh.openRead("test_close.dat");
    fh.close();
    EXPECT_FALSE(fh.isOpen());
    std::remove("test_close.dat");
}