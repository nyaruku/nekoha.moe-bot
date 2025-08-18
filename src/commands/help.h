namespace help {
    const static std::string command = "help";
    const static std::string alias = "h";
    const static std::string description = "**All Available Commands**\nPrefix: '" + config::bot::prefix + "'";

    dpp::embed cmd() {
        dpp::embed embed = dpp::embed().
            set_color(config::bot::embedColor).
            set_title("Help").
            set_description(description).
            
            
            add_field(
                "**help**",
                "This thingy."
            ).
        set_timestamp(time(0));
        return embed;
    }
}