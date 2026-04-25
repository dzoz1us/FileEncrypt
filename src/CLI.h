#pragma once

/**
 * @brief Интерфейс командной строки.
 * Обрабатывает интерактивное меню и аргументы командной строки.
 */
class CLI {
public:
    /**
     * @brief Запуск в интерактивном режиме (без аргументов).
     */
    void runInteractive();

    /**
     * @brief Запуск в пакетном режиме (с аргументами).
     * @param argc Количество аргументов
     * @param argv Массив аргументов
     */
    void runBatch(int argc, char* argv[]);
};