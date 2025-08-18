#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <regex>
#include <vector>

// Socket IO Client
#include <socket_io_client/sio_client.h>
sio::client client;
std::atomic<bool> ws_running{true};
std::atomic<bool> ws_reconnect{true};

// Discord Bot Library
#include <dpp/dpp.h>

// Headers
#include "config.h"
#include "extension.h"
#include "db.h"
#include "bot.h"
#include "socket.h"
#include "commands.h"

int main() {

    // Blocked if one of those cant connect, but should never happen
    db::nekoha_moe::connect();
    db::osu_logger::connect();
    
    // Start WebSocket thread (connects & listens)
    auto ws_thread = socket_io::websocketThread(client, discord_bot::bot);

    // Blocking Discord Bot Thread
    discord_bot::init();
    discord_bot::bot.start(dpp::st_wait);

    // tell websocket thread to not reconnect anymore
    ws_reconnect = false;
    ws_running = false;

    // explicitly close websocket connection
    client.sync_close();
    ws_thread.join();

    return 0;
}