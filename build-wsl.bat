@echo off
REM Build script using WSL
echo Building TCP Chat Server using WSL...
wsl -d docker-desktop sh -c "cd /mnt/c/Users/rajko/tcp-chat-server && gcc -Wall -Wextra -pthread -o server server.c && gcc -Wall -Wextra -pthread -o client client.c"
if %ERRORLEVEL% EQU 0 (
    echo Build successful!
    echo Starting server...
    wsl -d docker-desktop sh -c "cd /mnt/c/Users/rajko/tcp-chat-server && ./server"
) else (
    echo Build failed!
)
