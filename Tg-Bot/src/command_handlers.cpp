#include "command_handlers.hpp"

#include "boost/algorithm/string/trim_all.hpp"
#include "fmt/format.h"


std::string
tc::onRoll(TgBot::Message::Ptr msg)
{
    boost::trim_all(msg->text);

    int randomed       = std::rand() % 101;
    size_t space_index = msg->text.find(' ');
    std::string answer;

    if (randomed == 100)
        randomed = 146;

    if (space_index > 0 && space_index < msg->text.size()) {
        std::string thing = msg->text.substr(space_index + 1);

        answer = (
            msg->from->username == "nkysxx"
            ? fmt::format("Hot Cat says\n[{}]: {}%", thing, randomed)
            : fmt::format("Chance of\n[{}]: {}%", thing, randomed)
        );

        if (
            msg->text.find("кпоп") != std::string::npos
            || msg->text.find("kpop") != std::string::npos
        ) answer += "\n--------\nпочему рот в кпопе?";
        else if (msg->text.find("панк") != std::string::npos)
            answer += "\n--------\nпанки хой";
    } else
        answer = std::to_string(randomed);

    return answer;
}

std::string
tc::onWannaGf(TgBot::Message::Ptr msg)
{
    return
        "https://drive.google.com/drive/folders/"
        "1Pe2OAAG1FGmw5Zc29JCAWhCoIayuWi0F?usp=sharing";
}

std::string
tc::onWannaBf(TgBot::Message::Ptr msg)
{ return "t.me/secretdzen"; }

