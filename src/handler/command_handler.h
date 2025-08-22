namespace command_handler {
    // each command provides data for this struct 
    struct command {
        std::string cmdName;
        std::string cmdAlias;
        std::string cmdHelpDesc;
        bool isOwnerOnly;
        short cmdArgCount;
        dpp::embed (*helpEmbed)(dpp::cluster &bot, const dpp::message_create_t& event, std::vector<std::string>args);
        dpp::embed (*cmdEmbed)(dpp::cluster &bot, const dpp::message_create_t& event, std::vector<std::string>args);
        dpp::embed (*parseArgs)(dpp::cluster &bot, const dpp::message_create_t& event, std::vector<std::string>args);
    };

    extern const std::vector<command> registeredCommands;

    struct embedResult {
        dpp::embed embed;
    };

    inline embedResult commandParser(dpp::cluster &bot, const dpp::message_create_t& event, std::vector<std::string> args) {
        for(size_t i = 0; i < registeredCommands.size(); i++) {
            if(args[0] == registeredCommands[i].cmdName || args[0] == registeredCommands[i].cmdAlias) {
                return {registeredCommands[i].parseArgs(bot,event,args)};
            }
        }
        // No valid command found
        dpp::embed embed = dpp::embed().
        set_color(config::bot::embedColorErr).
        set_description(
            "```Command not found: " + args[0] + "```"
        );
        return {embed};
    }
}
