#include <iostream>
#include <string>
#include <thread>
#include <chrono>

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
#include "commands.h"

ormpp::dbng<ormpp::mysql> mysql;

void mysqlConnect() {
    while (true) {
        std::cout << "Connecting to Database, HOST: " <<  config::mysql::host << " | DB: " << config::mysql::database << "\n";
        if (mysql.connect(config::mysql::host, config::mysql::username, config::mysql::password ,config::mysql::database)) {
            std::cout << "Connected to MySQL Server\n";
            break;
        };
        std::cout << "Failed to connect to MySQL Server...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}

int main() {

    mysqlConnect();

    sio::client client;
    std::map<std::string, std::string> headers = {
        {"Authorization", "Bearer " + config::websocket::secret}
    };

    sio::message::ptr auth_msg = sio::object_message::create();
    auth_msg->get_map()["token"] = sio::string_message::create(config::websocket::secret);

    auto ws_thread = std::thread([&]() {
        try {
            client.set_fail_listener([&]() {
                std::cerr << "Socket connect error" << std::endl;
            });

            client.set_reconnect_attempts(-1);
            client.set_reconnect_delay(5000);
            
            client.socket(config::websocket::path)->on("connect", [&](sio::event&) {
                std::cout << "Connected to website WebSocket" << std::endl;
                client.socket(config::websocket::path)->emit("auth", auth_msg);
            });

            client.socket(config::websocket::path)->on("new_message", [&](sio::event& ev) {
                auto msg = ev.get_message();
                if (!msg) {
                    return;  
                } 
                std::string username = msg->get_map()["username"]->get_string();
                std::string message = msg->get_map()["message"]->get_string();
                int discord = msg->get_map()["discord"]->get_int();
                if (discord == 0) {
                    std::cout << "[" << username << "] " << message << std::endl;
                }
            });

            client.socket(config::websocket::path)->on("disconnect", [&](sio::event&) {
                std::cout << "Disconnected from website WebSocket" << std::endl;
            });

            client.connect(config::websocket::url, {}, headers);

            while (true) {
                std::this_thread::sleep_for(std::chrono::hours(1));
            } 

        } catch (const std::exception& e) {
            std::cerr << "WebSocket exception: " << e.what() << std::endl;
        }
    });

    // Discord bot runs in main thread
    dpp::cluster bot(config::bot::token, dpp::i_default_intents | dpp::i_message_content);
    bot.on_log(dpp::utility::cout_logger());

    bot.on_message_create([&bot](const dpp::message_create_t& event) {
        if (event.msg.content.find("bad word") != std::string::npos) {
            //event.reply("That is not allowed here.", true);
        }
    });

    bot.start(dpp::st_wait);
    ws_thread.join();

    return 0;
}
