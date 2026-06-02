# 1. Сборка проекта
FROM ubuntu:22.04 AS builder

# Ставим всё необходимое для сборки
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libssl-dev \
    git \
    && rm -rf /var/lib/apt/lists/*

# Копируем ВСЕ исходники одной командой
WORKDIR /app
COPY . .

# Собираем проект (основная программа + тесты + сценарии)
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build

# 2. Тестирование
FROM builder AS tester

# Запускаем unit-тесты
RUN ctest --test-dir build --output-on-failure

# Запускаем тестовые сценарии
RUN ./build/scenario_encrypt_decrypt
RUN ./build/scenario_wrong_password
RUN ./build/scenario_corrupted_file

# Этап 3: Финальный образ (без временных файлов сборки)
FROM ubuntu:22.04 AS final

# Ставим только runtime-зависимости
RUN apt-get update && apt-get install -y \
    libssl3 \
    && rm -rf /var/lib/apt/lists/*

# Копируем только готовый исполняемый файл из этапа сборки
WORKDIR /app
COPY --from=builder /app/build/fileencrypt /app/fileencrypt

# Точка входа - программа
ENTRYPOINT ["/app/fileencrypt"]
# Аргументы по умолчанию (если не переданы - запустится интерактивный режим)
CMD []