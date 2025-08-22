namespace ping {
    // net utils
    #include "../network/ping.h"

    const std::string name = "ping";
    const std::string alias = "p";
    const std::string helpDescription = "Measure the latency between discord or host.";
    const bool ownerOnly = false;
    const short argumentCount = 2;

    dpp::embed help(dpp::cluster &bot, const dpp::message_create_t& event, std::vector<std::string>args) {            
        dpp::embed embed = dpp::embed().
        set_title("Help for ping").
        set_color(config::bot::embedColor);
        std::string description =
            "`" + config::bot::prefix + name + "`\n"
            + "```Measure discord's latency```\n"
            + "`" + config::bot::prefix + name + " {host}`\n"
            + "```To measure the latency between a host.```\n";
        embed.set_description(description);
        return embed;
    }
    
    dpp::embed cmd(dpp::cluster &bot, const dpp::message_create_t& event, std::vector<std::string>args) {
        std::string description;

        if (args.size() == 1) {
            description = "```" + network::ping::latency("discord.com") + "```";
        } else {
            description = "```" + network::ping::latency(args[1]) + "```";
        }

        dpp::embed embed = dpp::embed().
        set_color(config::bot::embedColor);                
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
        return cmd(bot, event, args);
    }
}