#include "main.hpp"
#include "tk.hpp"
#include "LegendWords.hpp"

using namespace std::chrono_literals;

#define GET_GOOD (LegendWords::get(LegendWords::type::GOOD))
#define GET_GOODMAN (LegendWords::get(LegendWords::type::GOODMAN))
#define GET_ERROR (LegendWords::get(LegendWords::type::ERR))

CoordsData g_CoordsData{ "coords_data.json" };

void infinity_polling(TgBot::Bot& bot, std::atomic_bool &run, std::atomic_bool &end) {
    try {
        TgBot::TgLongPoll longPoll(bot);
        while (run.load()) {
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        std::cerr << "\nERROR: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    printf("Infinity thread polling closed\n");
    end = true;
}

int main() {
    TgBot::Bot bot{ TGBOT_TOKEN };

    auto msgBeforeCmd = [&bot](TgBot::Message::Ptr msg, const std::string &tp, const char* desc) {
        bot.getApi().sendMessage(msg->chat->id, tp + "\n\n" + desc, false, msg->messageId, 0, "MarkdownV2");
    };
    
    auto getStrParam = [](std::string& t, std::vector<std::string> &outParam) -> bool {
        if (!(std::count(t.begin(), t.end(), '\"') & 1)) {
            size_t a{}, b;
            while (a = t.find('\"', a), a != std::string::npos) {
                std::string s{ t.substr(a + 1, (b = t.find('\"', a + 1)) - 2) };
                if (s.size()) {
                    outParam.push_back(std::move(s));
                }
                t.erase(a, b);
                a = b + 1;
            }
            return true;
        }
        return false;
    };

    bot.getEvents().onCommand({"add_coord"}, [&bot, &msgBeforeCmd, &getStrParam](TgBot::Message::Ptr msg) {
        float x, y, z;
        std::vector<std::string> vStrParam;

        msg->text.erase(0, 10);
        if (!getStrParam(msg->text, vStrParam) || vStrParam.size() != 1 
            || sscanf(msg->text.c_str(), "%f %f %f", &x, &y, &z) != 3) 
        {
            msgBeforeCmd(msg, GET_ERROR, R"(*Правильно:* /add\_coord _\[name, "в кавычках"\] \[x\] \[y\] \[z\]_)");
        } else {
            auto &mb = g_CoordsData.getMembData(msg->chat->id);
            mb[msg->from->id].push_back({std::move(vStrParam[0]), x, y, z});

            msgBeforeCmd(msg, GET_GOODMAN, "Координата успешно *добавлена*\\.");
        }
    });

    bot.getEvents().onCommand({"list_coord"}, [&bot](TgBot::Message::Ptr msg){
        std::ostringstream str{};
        str << "📜Список координат:\n\n";
        for (auto &[id, cr] : g_CoordsData.getMembData(msg->chat->id)) {
            auto memb = bot.getApi().getChatMember(msg->chat->id, id);

            str << "[" << memb->user->firstName << " " << memb->user->lastName << "](tg://user?id=" << memb->user->id << "):\n";
            for (size_t i{}; i < cr.size(); ++i) {
                auto& [name, x, y, z] = cr[i];
                str << i + 1 << "\\) *" << fix_tgstr(name) << "* \\(`" << x << " " << y << " " << z << "`\\)\n";
            }
            str << "\n";
        }
        bot.getApi().sendMessage(msg->chat->id, str.str(), false, msg->messageId, 0, "MarkdownV2");
    });

    bot.getEvents().onCommand({"delete_coord"}, [&bot, &msgBeforeCmd](TgBot::Message::Ptr msg) {
        size_t n;
        if (auto r = sscanf(msg->text.c_str() + 13, "%ld", &n);
            r != 1) 
        {
            msgBeforeCmd(msg, GET_ERROR, R"(*Правильно:* /delete\_coord _\[порядковый номер координаты\]_)");
        } else {
            auto &coords = g_CoordsData.getMembData(msg->chat->id)[msg->from->id];
        
            if (n <= coords.size()) {
                coords.erase(coords.begin() + n);
                msgBeforeCmd(msg, GET_GOOD, "Координата успешно *удалена*\\.");
            }
            else {
                msgBeforeCmd(msg, GET_ERROR, "Такой координаты нету\n||\\(порядковый номер больше размера самого списка\\)||");
            }
        }
    });

    // INFINITY POLLING //

    std::atomic_bool run{ true }, end{ false };
    std::thread infinity_thr{infinity_polling, std::ref(bot), std::ref(run), std::ref(end)};
    infinity_thr.detach();

    std::string str;
    do {
        std::cout << R"(Write "close" to close the program: )";
        std::cin >> str;
    } while (str != "close");
    run = false;
    while (!end.load()) { std::this_thread::sleep_for(500ms); }

    return 0;
}