#include "router.hpp"

#include "boost/algorithm/string/trim_all.hpp"
#include "fmt/format.h"
#include "tgbot/TgException.h"


void
tc::Router::route(const std::string& command, tc::Router::ReplyHandler handler)
{
    bot_.getEvents().onCommand(
        command,
        [this, handler](TgBot::Message::Ptr msg) {
            std::string answer = handler(msg);

            try {
                bot_.getApi().sendMessage(
                    msg->chat->id,
                    answer,
                    false,
                    msg->messageId
                );
            } catch (const TgBot::TgException& e) {
                bot_.getApi().sendMessage(msg->chat->id, answer);
            }
        }
    );
}

