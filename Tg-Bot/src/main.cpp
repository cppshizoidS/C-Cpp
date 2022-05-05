#include <cstdlib>
#include <ctime>
#include <exception>
#include <string>

#include "boost/algorithm/string/trim_all.hpp"
#include "boost/program_options.hpp"
#include "fmt/core.h"
#include "fmt/format.h"
#include "tgbot/tgbot.h"

#include "router.hpp"
#include "command_handlers.hpp"


int
main()
{
    using namespace fmt;
    namespace opt = boost::program_options;

    std::srand(std::time(nullptr));
    std::string token;

    opt::options_description desc;
    desc.add_options()
        (
            "token",
            opt::value(&token)->required(),
            "bot token"
        );
    opt::variables_map vm;
    opt::store(opt::parse_config_file("../toadcraft.conf", desc), vm);
    vm.notify();

    TgBot::Bot bot(token);
    tc::Router router(bot);

    router.route("roll", tc::onRoll);
    router.route("wannagf", tc::onWannaGf);
    router.route("wannabf", tc::onWannaBf);

    while (true) {
        print("=== start ===\n");

        try {
            TgBot::TgLongPoll long_poll(bot);

            while (true) {
                print("Long poll started\n");
                long_poll.start();
            }
        } catch (const std::exception& e) {
            print("error: {}\n", e.what());
        }
    }

    return 0;
}

