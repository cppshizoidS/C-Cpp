#pragma once

#include <functional>
#include <string>

#include "fmt/format.h"
#include "tgbot/EventBroadcaster.h"
#include "tgbot/tgbot.h"


namespace tc {


class Router
{
private:
    TgBot::Bot& bot_;

public:
    Router(TgBot::Bot& bot) : bot_(bot) {};

public:
    using ReplyHandler = std::function<std::string (TgBot::Message::Ptr)>;

    void
    route(const std::string& command, ReplyHandler handler);
};


}

