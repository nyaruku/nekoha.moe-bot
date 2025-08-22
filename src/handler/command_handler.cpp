#include <string>
#include <vector>
#include <dpp/dpp.h>

#include "../config.h"

#include "command_handler.h"
#include "../commands/help.h"
#include "../commands/ping.h"

namespace command_handler {
    
    const std::vector<command> registeredCommands = {
        {help::name, help::alias, help::helpDescription, help::ownerOnly, help::argumentCount, help::help, help::cmd, help::parseArgs}
        ,{ping::name, ping::alias, ping::helpDescription, ping::ownerOnly, ping::argumentCount, ping::help, ping::cmd, ping::parseArgs}
    };
}