cmake_minimum_required(VERSION 3.10)
project(FileEncrypt VERSION 0.1.0 LANGUAGES CXX)

# Устанавливаем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Список исходных файлов
set(SOURCES
    src/main.cpp
    src/CLI.cpp
    src/CryptoEngine.cpp
    src/FileHeader.cpp
    src/FileHandler.cpp
    src/FileEncryptor.cpp
)

# Исполняемый файл
add_executable(fileencrypt ${SOURCES})

# Подключаем заголовочные файлы
target_include_directories(fileencrypt PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src   # для CLI.h
)

# Опционально: подключаем OpenSSL (раскомментировать, когда библиотека установлена)
# find_package(OpenSSL REQUIRED)
# target_link_libraries(fileencrypt PRIVATE OpenSSL::Crypto)
# target_compile_definitions(fileencrypt PRIVATE HAS_OPENSSL)

# Выводим информацию о сборке
message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")