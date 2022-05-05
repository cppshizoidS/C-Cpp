#pragma once

#include <string>
#include <vector>

#include "tgbot/EventBroadcaster.h"


namespace tc {


struct Command;

using Commands = std::vector<Command>;


struct Command
{
    std::string_view on;
    const TgBot::EventBroadcaster::MessageListener& listener;
};


}

