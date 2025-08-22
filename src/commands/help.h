namespace help {
    const std::string name = "help";
    const std::string alias = "h";
    const std::string helpDescription = "Help command";
    const bool ownerOnly = false;
    const short argumentCount = 1;
    
    dpp::embed help(dpp::cluster &bot, const dpp::message_create_t& event, std::vector<std::string>args) {
        dpp::embed embed = dpp::embed().
        set_color(config::bot::embedColor).
        set_description(
            "`" + config::bot::prefix + name + " {command}`\n"
            + "To get specific help about a command."
        ).
        set_timestamp(time(0));
        return embed;
    }
    
    dpp::embed cmd(dpp::cluster &bot, const dpp::message_create_t& event, std::vector<std::string>args) {
        std::string description = "**All available commands**\n";
        
        dpp::embed embed = dpp::embed().
        set_color(config::bot::embedColor).
        set_title("Help");
        
        description
        += "`" + command_handler::registeredCommands[0].cmdName + "`"
        + " or `" + command_handler::registeredCommands[0].cmdAlias + "`" 
        + " `" + command_handler::registeredCommands[0].cmdHelpDesc + "`";
        
        for (size_t i = 1; i < command_handler::registeredCommands.size(); i++) {
            description 
            += "\n`" + command_handler::registeredCommands[i].cmdName + "`"
            + " or `" + command_handler::registeredCommands[i].cmdAlias + "`" 
            + " `" + command_handler::registeredCommands[i].cmdHelpDesc + "`";
        }
        
        embed.set_description(description);
        embed.set_timestamp(time(0));
        return embed;
    }

    dpp::embed parseArgs(dpp::cluster &bot, const dpp::message_create_t& event, std::vector<std::string>args) {
        if (args.size() > argumentCount+1) {
            
        }
    }
}