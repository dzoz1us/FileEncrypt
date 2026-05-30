# FileEncrypt

Консольная утилита для шифрования и дешифрования файлов с использованием
алгоритма AES-256-CBC на базе библиотеки OpenSSL.

## Функциональность

- Шифрование произвольных файлов по паролю
- Дешифрование ранее зашифрованных файлов
- Интерактивный режим с пошаговым меню
- Пакетный режим через аргументы командной строки
- Потоковая обработка (файл не загружается в память целиком)

## Требования

- CMake >= 3.10
- Компилятор с поддержкой C++17 (GCC >= 8, Clang >= 7, MSVC >= 2017)
- OpenSSL >= 1.1.1 (опционально на этапе прототипа)

## Сборка

```bash

# Конфигурация (out-of-source build)
cmake -S . -B build

# Сборка
cmake --build build

## Тестирование

Проект использует Google Test для модульного тестирования. Тесты охватывают все основные классы: "FileHeader", "FileHandler", "CryptoEngine", "FileEncryptor"

FileHeader - проверка формирования и разбора заголовка зашифрованного файла
FileHandler - проверка файлового ввода-вывода
CryptoEngine - проверка криптографического движка
FileEncryptor - интеграционные тесты полного цикла

### Запуск тестов

# Запуск всех тестов через CTest
ctest --test-dir build --output-on-failure

# Запуск сценариев
1. .\build\scenario_corrupted_file.exe
2. .\build\scenario_encrypt_decrypt.exe
3. .\build\scenario_wrong_password.exe
```

### Docker

# 1. Dockerfile для автоматической сборки проекта
Собирает проект без ошибок
Команда docker build -t fileencrypt . завершается успешно

# 2. Возможность запуска тестов внутри контейнера
В Dockerfile есть этап tester, который запускает ctest и три сценария
При сборке тесты проходят, иначе сборка бы упала
docker build --target tester -t fileencrypt:test . 

# 3. Передача аргументов командой строки
CLI::runBatch() обрабатывает аргументы: -e, -d, -p, -o, -h
ENTRYPOINT ["/app/fileencrypt"] в Dockerfile обеспечивает передачу аргументов
CMD [] - без аргументов запускается интерактивный режим

Справка:
docker run --rm fileencrypt -h

Шифрование:
docker run --rm -v ${PWD}/data:/data -w /data fileencrypt -e test.txt -p mypass -o test.enc

Расшифровка:
docker run --rm -v ${PWD}/data:/data -w /data fileencrypt -d test.enc -p mypass -o test_dec.txt

Неверный пароль (с возвращением кода ошибки)
docker run --rm -v ${PWD}/data:/data -w /data fileencrypt -d test.enc -p wrongpass; echo "Exit code: $LASTEXITCODE"


# Работающий контейнер с проектом
Программа работает в двух режимах - пакетном и интерактивном

Интерактивный режим
docker run -it --rm -v ${PWD}/data:/data -w /data fileencrypt
Появляется меню, можно выбрать 1 или 2, ввести путь к файлу и пароль, получить результат

Пакетный режим
Создание файла
echo "Secret: 12345" > data/demo.txt
Шифровка: 
docker run --rm -v ${PWD}/data:/data -w /data fileencrypt -e demo.txt -p secret -o demo.enc
Расшифровка:
docker run --rm -v ${PWD}/data:/data -w /data fileencrypt -d demo.enc -p secret -o demo_dec.txt
