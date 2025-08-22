#include "handler/command_handler.h"

namespace discord_bot {
    dpp::cluster bot(config::bot::token, dpp::i_default_intents | dpp::i_message_content);

    void init() {
        bot.on_log(dpp::utility::cout_logger());

            bot.on_ready([](const dpp::ready_t& event) {
            std::cout << "Logged in as " << bot.me.username << "\n";
            bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_game, config::bot::prefix + "help"));
        });

        bot.on_message_create([](const dpp::message_create_t& event) {
            // Also prevents that bot messages will be send over socket
            if (event.msg.author.is_bot()) return;            

            if (event.msg.channel_id == config::bot::websiteChannelId) {
                uint64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
                ).count();
                std::string username = event.msg.author.username;
                std::string content = event.msg.content.substr(0, 2000);
                std::string color = "#5865F2";

                try {
                    db::nekoha_moe::chat_record row {
                        0
                        ,static_cast<long long>(timestamp)
                        ,username
                        ,content
                        ,1
                        ,color
                    };

                    // SQL Insert and returns id of row
                    uint64_t newId = db::nekoha_moe::mysql.get_insert_id_after_insert<db::nekoha_moe::chat_record>(row);
                    
                    sio::message::ptr newMessage = sio::object_message::create();
                    newMessage->get_map()["id"] = sio::int_message::create(newId);
                    newMessage->get_map()["timestamp"] = sio::int_message::create(timestamp);
                    newMessage->get_map()["username"] = sio::string_message::create(username);
                    newMessage->get_map()["message"] = sio::string_message::create(content);
                    newMessage->get_map()["discord"] = sio::int_message::create(1);
                    newMessage->get_map()["color"] = sio::string_message::create(color);

                    client.socket(config::websocket::path)->emit("new_message", newMessage);

                    std::cout << "Sent message to WebSocket\n";  
                } 
                catch (const std::exception& e) {
                    std::cerr << "DB insert error: " << e.what() << std::endl;
                }
            } else {
                std::string originalMessage = event.msg.content;
                originalMessage = extension::collapseSpaces(originalMessage);
                originalMessage = extension::trim(originalMessage);
                std::vector<std::string> args;
                std::istringstream iss(originalMessage);
                std::string token;
                while (iss >> token) {
                    args.push_back(token);
                }
                command_handler::embedResult parsedCommand = command_handler::commandParser(bot, event, args);
                if(parsedCommand.valid == false){
                    
                }
                //dpp::message msg(response);
                //event.reply(msg, true);
            }
        });
    }
}