cmake_minimum_required(VERSION 3.14)
project(FileEncrypt VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# === OpenSSL ===
find_package(OpenSSL REQUIRED)
message(STATUS "OpenSSL found: ${OPENSSL_VERSION}")

# === Основная программа ===
set(SOURCES
    src/main.cpp
    src/CLI.cpp
    src/CryptoEngine.cpp
    src/FileHeader.cpp
    src/FileHandler.cpp
    src/FileEncryptor.cpp
)

add_executable(fileencrypt ${SOURCES})

target_include_directories(fileencrypt PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${OPENSSL_INCLUDE_DIR}
)

target_link_libraries(fileencrypt PRIVATE OpenSSL::Crypto)
target_compile_definitions(fileencrypt PRIVATE HAS_OPENSSL)

# === Google Test ===
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# === Тесты ===
# Тесты для каждого модуля
add_executable(test_cryptoengine
    tests/test_cryptoengine.cpp
    src/CryptoEngine.cpp
)
target_include_directories(test_cryptoengine PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${OPENSSL_INCLUDE_DIR}
)
target_link_libraries(test_cryptoengine PRIVATE OpenSSL::Crypto GTest::gtest_main)

add_executable(test_fileheader
    tests/test_fileheader.cpp
    src/FileHeader.cpp
)
target_include_directories(test_fileheader PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(test_fileheader PRIVATE GTest::gtest_main)

add_executable(test_filehandler
    tests/test_filehandler.cpp
    src/FileHandler.cpp
)
target_include_directories(test_filehandler PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(test_filehandler PRIVATE GTest::gtest_main)

add_executable(test_fileencryptor
    tests/test_fileencryptor.cpp
    src/FileEncryptor.cpp
    src/CryptoEngine.cpp
    src/FileHeader.cpp
    src/FileHandler.cpp
)
target_include_directories(test_fileencryptor PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${OPENSSL_INCLUDE_DIR}
)
target_link_libraries(test_fileencryptor PRIVATE OpenSSL::Crypto GTest::gtest_main)

# Регистрируем тесты в CTest
include(GoogleTest)
gtest_discover_tests(test_cryptoengine)
gtest_discover_tests(test_fileheader)
gtest_discover_tests(test_filehandler)
gtest_discover_tests(test_fileencryptor)

# Тестовые сценарии (мини-программы)

# Сценарий 1: Зашифровать → Расшифровать → Сравнить
add_executable(scenario_encrypt_decrypt
    scenarios/scenario_encrypt_decrypt.cpp
    src/FileEncryptor.cpp
    src/CryptoEngine.cpp
    src/FileHeader.cpp
    src/FileHandler.cpp
)
target_include_directories(scenario_encrypt_decrypt PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${OPENSSL_INCLUDE_DIR}
)
target_link_libraries(scenario_encrypt_decrypt PRIVATE OpenSSL::Crypto)

# Сценарий 2: Неверный пароль
add_executable(scenario_wrong_password
    scenarios/scenario_wrong_password.cpp
    src/FileEncryptor.cpp
    src/CryptoEngine.cpp
    src/FileHeader.cpp
    src/FileHandler.cpp
)
target_include_directories(scenario_wrong_password PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${OPENSSL_INCLUDE_DIR}
)
target_link_libraries(scenario_wrong_password PRIVATE OpenSSL::Crypto)

# Сценарий 3: Повреждённый файл
add_executable(scenario_corrupted_file
    scenarios/scenario_corrupted_file.cpp
    src/FileEncryptor.cpp
    src/CryptoEngine.cpp
    src/FileHeader.cpp
    src/FileHandler.cpp
)
target_include_directories(scenario_corrupted_file PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${OPENSSL_INCLUDE_DIR}
)
target_link_libraries(scenario_corrupted_file PRIVATE OpenSSL::Crypto)