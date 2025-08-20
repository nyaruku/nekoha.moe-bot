namespace help {
    const static std::string name = "help";
    const static std::string alias = "h";
    const static std::string helpDecription = "Help command";
    const static std::string description = "**All Available Commands**\nPrefix: '" + config::bot::prefix + "'";
    const static bool ownerOnly = false;
    const static short argumentCount = 1;

    dpp::embed help() {
        return NULL;
    }

    dpp::embed cmd() {
        dpp::embed embed = dpp::embed().
            set_color(config::bot::embedColor).
            set_title("Help").
            set_description(description).
        set_timestamp(time(0));
        // generate fields based on registered commands array
        for (size_t i = 0; i < command_handler::registeredCommands.size(); i++) {
            embed.add_field(
                "",
                ""
            );
        }
        return embed;
    }
}