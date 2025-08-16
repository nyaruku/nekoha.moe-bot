#include <iostream>
#include <cstdint>
#include <string>
#include <thread>
#include <chrono>
#include <regex>

// D++ (C++ Discord Bot Library)
#include <dpp/dpp.h>

// Socket IO Client
#include <socket_io_client/sio_client.h>

// ormpp
#include <ormpp/dbng.hpp>
#include <ormpp/mysql.hpp>
#include <ormpp/connection_pool.hpp>

// Headers
#include "config.h"
#include "extension.h"
#include "commands.h"

ormpp::dbng<ormpp::mysql> mysql;

void mysqlConnect() {
    while (true) {
        std::cout << "Connecting to Database...\nHOST: " <<  config::mysql::host << "\nDB: " << config::mysql::database << "\n";
        if (mysql.connect(config::mysql::host, config::mysql::username, config::mysql::password, config::mysql::database)) {
            std::cout << "Connected to MySQL Server\n";
            break;
        };
        std::cout << "Failed to connect to MySQL Server...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}

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

std::atomic<bool> ws_running{true};
std::mutex ws_mutex;

std::thread websocketThread(sio::client& client, const sio::message::ptr& auth_msg, const std::map<std::string, std::string>& headers, dpp::cluster& bot) {
    return std::thread([&]() {
        try {
            client.set_fail_listener([&]() {
                std::cerr << "Socket connect error\n";
            });

            client.set_reconnect_attempts(-1);
            client.set_reconnect_delay(2000);

            client.socket(config::websocket::path)->on("connect", [&](sio::event&) {
                std::cout << "Connected to website WebSocket\n";
                {
                    std::lock_guard<std::mutex> lock(ws_mutex);
                    client.socket(config::websocket::path)->emit("auth", auth_msg);
                }
            });

            client.socket(config::websocket::path)->on("new_message", [&](sio::event& ev) {
                auto msg = ev.get_message();
                if (!msg) return;

                int discord = msg->get_map()["discord"]->get_int();

                if (discord == 1) {
                    return;
                }

                std::string username = msg->get_map()["username"]->get_string();
                std::string message = msg->get_map()["message"]->get_string();
                bot.message_create(
                    dpp::message(config::bot::websiteChannelId, 
                        "[" + extension::sanitizeDiscordMentions(username) + "] " + extension::sanitizeDiscordMentions(message))
                );
            });

            client.socket(config::websocket::path)->on("disconnect", [&](sio::event&) {
                std::cout << "Disconnected from website WebSocket\n";
            });

            client.connect(config::websocket::url, {}, headers);

            while (ws_running) {
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }

        } catch (const std::exception& e) {
            std::cerr << "WebSocket exception: " << e.what() << std::endl;
        }
    });
}

int main() {

    mysqlConnect();

    sio::client client;
    std::map<std::string, std::string> headers = {
        {"Authorization", "Bearer " + config::websocket::secret}
    };

    sio::message::ptr auth_msg = sio::object_message::create();
    auth_msg->get_map()["token"] = sio::string_message::create(config::websocket::secret);
    
    dpp::cluster bot(config::bot::token, dpp::i_default_intents | dpp::i_message_content);
    bot.on_log(dpp::utility::cout_logger());

    bot.on_ready([&bot](const dpp::ready_t& event) {
        std::cout << "Logged in as " << bot.me.username << "\n";
    });

    bot.on_message_create([&bot, &client](const dpp::message_create_t& event) {
        if (event.msg.author.is_bot()) return;

        if (event.msg.channel_id == config::bot::websiteChannelId) {
            uint64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                                     std::chrono::system_clock::now().time_since_epoch())
                                     .count();
            std::string username = event.msg.author.username;
            std::string content = event.msg.content.substr(0, 2000);
            std::string color = "#5865F2";

        try {
            // ORM insert (no raw SQL strings, no manual quoting)
            chat_record row {
                0,
                static_cast<long long>(timestamp),
                username,
                content,
                1,
                color
            };

            const int inserted = mysql.insert(row);
            if (inserted != 1) {
                throw std::runtime_error("insert failed");
            }

            // Correct usage: pass the same object and the template type
            auto newId = mysql.get_insert_id_after_insert<chat_record>(row);

            // Build message for WebSocket
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
    }});

    // Start WebSocket thread (connects & listens)
    auto ws_thread = websocketThread(client, auth_msg, headers, bot);

    // Blocking Discord Bot Thread
    bot.start(dpp::st_wait);
   
    ws_running = true;
    client.sync_close();
    ws_thread.join();
    return 0;
}
