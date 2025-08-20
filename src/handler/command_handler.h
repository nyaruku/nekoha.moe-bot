namespace command_handler {
    // each command provides data for this struct 
    struct command {
        std::string name;
        std::string alias;
        std::string helpDecription;
        std::string description;
        bool ownerOnly;
        short argumentCount;
        dpp::embed (*help)();
        dpp::embed (*cmd)();
    };

    extern const std::vector<command> registeredCommands;

    static dpp::embed commandParser(const dpp::message_create_t& event, std::vector<std::string>msg) {
        for(size_t i = 0; i < registeredCommands.size(); i++) {

        }
        return NULL;
    }
}
