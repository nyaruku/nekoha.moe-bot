namespace help {
    const std::string name = "help";
    const std::string alias = "h";
    const std::string helpDescription = "Help command";
    const bool ownerOnly = false;
    const short argumentCount = 2;
    
    dpp::embed help(dpp::cluster &bot, const dpp::message_create_t& event, std::vector<std::string>args) {
        // check for command to help
        if (args[1] != name && args[1] != alias) {
            for (size_t i = 0; i < command_handler::registeredCommands.size(); i++) {
                if (args[1] == command_handler::registeredCommands[i].cmdName || args[1] == command_handler::registeredCommands[i].cmdAlias) {
                    return command_handler::registeredCommands[i].helpEmbed(bot, event, args);
                }
            }
        }
            
        dpp::embed embed = dpp::embed().
        set_color(config::bot::embedColor).
        set_title("Help").
        set_description(
            "`" + config::bot::prefix + name + " {command}`\n"
            + "```To get specific help about a command.```"
        );
        return embed;
    }
    
    dpp::embed cmd(dpp::cluster &bot, const dpp::message_create_t& event, std::vector<std::string>args) {
        std::string description = "**All available commands**\n";
        
        dpp::embed embed = dpp::embed().
        set_color(config::bot::embedColor).
        set_title("Help");
        
        description
        += "`" + config::bot::prefix + command_handler::registeredCommands[0].cmdName + "`"
        + " or `" + config::bot::prefix + command_handler::registeredCommands[0].cmdAlias + "`\n" 
        + "```" + command_handler::registeredCommands[0].cmdHelpDesc + "```";
        
        for (size_t i = 1; i < command_handler::registeredCommands.size(); i++) {
            description 
            += "\n`" + config::bot::prefix + command_handler::registeredCommands[i].cmdName + "`"
            + " or `" + config::bot::prefix + command_handler::registeredCommands[i].cmdAlias + "`\n" 
            + "```" + command_handler::registeredCommands[i].cmdHelpDesc + "```";
        }
        embed.set_description(description);
        return embed;
    }

    dpp::embed parseArgs(dpp::cluster &bot, const dpp::message_create_t& event, std::vector<std::string>args) {
        if (args.size() > argumentCount) {
            dpp::embed embed = dpp::embed().
            set_color(config::bot::embedColorErr).
            set_description(
                "```To many arguments```"
            );
            return embed;
        }

        if (args.size() == 1)
            return cmd(bot, event, args);
        if (args.size() == 2)
            return help(bot, event, args);
    }
}