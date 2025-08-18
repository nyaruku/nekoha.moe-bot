#include "commands/help.h"

namespace commands {
    struct command {
        std::string command;
        std::string alias;
        std::string description;
        bool ownerOnly;
        short argumentCount;
        dpp::embed help; // null if no help available
        dpp::embed cmd;
    };

    const std::vector<command> registeredCommands = {
        {config::help::command, config::help::alias, config::help::description, false, 1, null, help::cmd}
    };
}