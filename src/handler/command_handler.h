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

    enum callbackReason {
        noValidPrefix = 0
        ,noValidCommand = 1
    };

    std::string callbackReasonString(callbackReason reason, std::string info) {
        switch(reason) {
            case callbackReason::noValidCommand: return "Command doesn't exist: ";
        }
        return "";
    }

    struct embedResult {
        dpp::embed embed;
        bool valid;
        callbackReason reason;
        std::string info = "";
    };

    inline embedResult commandParser(dpp::cluster &bot, const dpp::message_create_t& event, std::vector<std::string>args) {
        if(args[0][0] != config::bot::prefix[0]) {            
            // no callback, since message is not starting with prefix
            return {dpp::embed(), false, callbackReason::noValidPrefix};
        }

        // remove prefx
        args[0].erase(args[0].begin());

        for(size_t i = 0; i < registeredCommands.size(); i++) {
            if(args[0] == registeredCommands[i].cmdName || args[0] == registeredCommands[i].cmdAlias) {
                return {registeredCommands[i].parseArgs(bot,event,args), true};
            }
        }

        // No valid command found, return with assumed command
        return {dpp::embed(), false, callbackReason::noValidCommand, args[0]};
    }
}
