#include "command_handler.h"
#include "../commands/help.h"

namespace command_handler {

    const std::vector<command> registeredCommands = {
        {help::name, help::alias, help::helpDescription, help::description, help::ownerOnly, help::argumentCount, help::help, help::cmd}
    };
}