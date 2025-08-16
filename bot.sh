#!/bin/bash

echo "Checking for existing bot instances..."
pkill -x "nekoha.moe-bot"

while true; do
    echo "Starting Bot..."
    ./out/build/x86-debug-linux/nekoha.moe-bot
    echo "Bot crashed. Restarting..."
    sleep 1
done