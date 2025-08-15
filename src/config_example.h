class config {
public:
    struct bot {
        static inline std::string token = "";
        static inline std::string prefix = ",";
        static inline std::string websiteChannelId = "";
    };
    
    struct mysql {
        static inline std::string host = "";
        static inline std::string database = "";
        static inline std::string username = "";
        static inline std::string password = "";
    };

    struct websocket {
        static inline std::string url = "";
        static inline std::string path = "";
        static inline std::string secret = "";
    };
};