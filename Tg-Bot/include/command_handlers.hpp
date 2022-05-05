#pragma once

#include <string>

#include "tgbot/types/Message.h"


namespace tc {


std::string
onRoll(TgBot::Message::Ptr msg);

std::string
onRollMore(TgBot::Message::Ptr msg);

std::string
onWannaGf(TgBot::Message::Ptr);

std::string
onWannaBf(TgBot::Message::Ptr);


}

