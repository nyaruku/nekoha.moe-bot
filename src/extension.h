class extension {
    public:
        static inline std::string sanitizeDiscordMentions(const std::string& text) {
            std::string result = text;
            result = std::regex_replace(result, std::regex("@everyone"), "@\u200Beveryone"); // @everyone
            result = std::regex_replace(result, std::regex("@here"), "@\u200Bhere");         // @here
            result = std::regex_replace(result, std::regex("<@!?(\\d+)>"), "<@\u200B$1>");   // user mentions
            result = std::regex_replace(result, std::regex("<@&(\\d+)>"), "<@&\u200B$1>");   // role mentions
            result = std::regex_replace(result, std::regex("<#(\\d+)>"), "<#\u200B$1>");     // channel mentions
            return result;
        };
};