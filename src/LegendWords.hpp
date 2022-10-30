#pragma once

#include <string>

class LegendWords {
public:
    // Характер фраз
    enum class type {
        ERR, // Ошибка
        GOOD, // Ок
        GOODMAN // Молодец
    };

    /**
     * @brief Получить рандомную фразочку от Елизоветы В.
     * 
     * @param tp Характер фразы
     * @return std::string Строка с фразой
     */
    static std::string get(type tp);
};