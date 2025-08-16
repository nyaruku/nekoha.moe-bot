// ormpp (database connector)
#include <ormpp/dbng.hpp>
#include <ormpp/mysql.hpp>
#include <ormpp/connection_pool.hpp>

namespace db {
    namespace nekoha_moe {
        ormpp::dbng<ormpp::mysql> mysql;
        struct chat_record {
            uint64_t id;
            int64_t timestamp;
            std::string username;
            std::string message;
            int discord;
            std::string color;

            // Ensure table name is exactly "chat"
            static constexpr std::string_view get_alias_struct_name(chat_record*) {
                return "chat";
            }
        };

        REGISTER_AUTO_KEY(chat_record, id)
        YLT_REFL(chat_record, id, timestamp, username, message, discord, color)
        
        void connect() {
            while (true) {
                std::cout << "Connecting to Database...\nHOST: " <<  config::mysql::host << "\nDB: " << config::mysql::nekoha_moe::database << "\n";
                if (mysql.connect(config::mysql::host, config::mysql::username, config::mysql::password, config::mysql::nekoha_moe::database)) {
                    std::cout << "Connected to MySQL Server\n";
                    break;
                };
                std::cout << "Failed to connect to MySQL Server...\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            }
        }
    }

    namespace osu_logger {
        ormpp::dbng<ormpp::mysql> mysql;
        void connect() {
            while (true) {
                std::cout << "Connecting to Database...\nHOST: " <<  config::mysql::host << "\nDB: " << config::mysql::osu_logger::database << "\n";
                if (mysql.connect(config::mysql::host, config::mysql::username, config::mysql::password, config::mysql::osu_logger::database)) {
                    std::cout << "Connected to MySQL Server\n";
                    break;
                };
                std::cout << "Failed to connect to MySQL Server...\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            }
        }
    }
}