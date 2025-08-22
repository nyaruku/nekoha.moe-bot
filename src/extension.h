namespace extension {
    static inline std::string sanitizeDiscordMentions(const std::string& text) {
        std::string result = text;
        result = std::regex_replace(result, std::regex("@everyone"), "@\u200Beveryone"); // @everyone
        result = std::regex_replace(result, std::regex("@here"), "@\u200Bhere");         // @here
        result = std::regex_replace(result, std::regex("<@!?(\\d+)>"), "<@\u200B$1>");   // user mentions
        result = std::regex_replace(result, std::regex("<@&(\\d+)>"), "<@&\u200B$1>");   // role mentions
        result = std::regex_replace(result, std::regex("<#(\\d+)>"), "<#\u200B$1>");     // channel mentions
        return result;
    };
    
    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n\r\f\v");
        if (first == std::string::npos)
            return "";
        size_t last = str.find_last_not_of(" \t\n\r\f\v");
        return str.substr(first, last - first + 1);
    }

    std::string collapseSpaces(const std::string& input) {
        std::string result;
        result.reserve(input.size());
        bool in_space = false;

        for (unsigned char ch : input) {
            if (std::isspace(ch)) {
                if (!in_space) {
                    result.push_back(' ');
                    in_space = true;
                }
            } else {
                result.push_back(ch);
                in_space = false;
            }
        }
        return result;
    }

};