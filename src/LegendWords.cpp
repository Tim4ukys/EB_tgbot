#include "LegendWords.hpp"
#include <random>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(*arr))

static std::string Error[] {
    "❗️ А\\-А\\-АХОВАЯ СИТУАЦИЯ ❗️",
    "😶 У нас аншлаг 🤐",
    "😳 Три звоночка от кого не ждали 🙅‍♂️",
    "‼️ Аварийная ситуация ‼️",
    "💂‍♀️ Полёт орла не увенчался успехом 🫣"
};

static std::string Good[] {
    "Добро 🤝 👍",
    "💯 Стопроцентное попадание 🎯"
};

static std::string GoodMan[] {
    "📚 Настоящий лицеист 🤓",
    "🎸 Super star 🤟",
    "Антон Павлович Чехов гордился бы вами 😏"
};

static int random(int max) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist{0, max};
    return dist(mt);
}

std::string LegendWords::get(type tp) {
    switch (tp)
    {
    case type::GOOD:
        return Good[random(ARRAY_SIZE(Good)-1)];
    case type::GOODMAN:
        return GoodMan[random(ARRAY_SIZE(GoodMan)-1)];
    case type::ERR:
    default:
        return Error[random(ARRAY_SIZE(Error)-1)];
    } 
}