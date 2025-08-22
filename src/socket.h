namespace socket_io {
    std::thread websocketThread(sio::client& client, dpp::cluster& bot) {
        return std::thread([&]() {
            try {
                client.set_fail_listener([&]() {
                    std::cerr << "Socket connect error\n";
                });

                client.set_close_listener([&](sio::client::close_reason const& reason) {
                    std::cout << "Socket Client Disconnected\n";
                });

                client.socket(config::websocket::path)->on("connect", [&](sio::event&) {
                    std::cout << "Connected to website WebSocket\n";
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
                
                client.set_reconnect_attempts(-1);      // infinite retries
                client.set_reconnect_delay(2000);       // 2s before first retry
                client.set_reconnect_delay_max(5000);  // cap retry backoff at 5s
                client.connect(config::websocket::url, {{"token", config::websocket::secret}});
                
                while (ws_running) {
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                }

            } catch (const std::exception& e) {
                std::cout << "WebSocket exception: " << e.what() << std::endl;
                // maybe restart ws client after exception ??, idk
            }
        });
    }
}