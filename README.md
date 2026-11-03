# Networked Tic-Tac-Toe (C++, Winsock)

A two-player tic-tac-toe game played over TCP. A single server process hosts many games at once, and players connect from a console client.

*Originally written Jul–Sep 2021. Uploaded to GitHub in Nov 2026 from my old PC; the code is unchanged except for removing an IP address from a comment.*

## How it works
- **Server (`code/win32_server.cpp`)**
  - Listens on TCP port 27015.
  - Uses `WSAPoll` on a single thread to service up to 10 client sockets.
  - Tracks up to 100 games and assigns IDs to players and games.
  - A player can create a game or join one by its ID.
- **Protocol (`code/Shared.h`)**
  - A small binary protocol. The first byte of each message is a code, such as `CREATE_GAME`, `JOIN_GAME`, `YOUR_TURN`, `MOVE_OK`, `INVALID_MOVE` or `GAMEOVER`.
  - Some messages carry packed structs with the game and player IDs.
- **Game rules (`code/Client_TicTacToe.cpp`)**
  - `PlayMove` checks that the move is legal and detects a win. Draws aren't handled yet; that's still a TODO.
  - The server runs this check on every move, so a client can't cheat. It then tells the next player that it's their turn.
- **Client (`code/win32_client.cpp`):** console UI that connects to the server and sends moves. The server address is `TEST_IP` near the top of the file.

## Build (Windows, MSVC)
Open a "x64 Native Tools Command Prompt for VS" and run:
```
cd code
build.bat
```
This creates `build\win32_server.exe` and `build\win32_client.exe`. Start the server first, then start two clients.

`code/TCPClient.py` is a small Python test client I used to check the server's socket handling.

## Concepts practiced
- Winsock TCP sockets and multiplexing with `WSAPoll`
- Designing a binary message protocol
- Checking moves on the server and keeping turns in sync between two clients
- Keeping game and player state in fixed-size arrays

## Known limitations / TODOs (from my notes in the code)
- No reconnect or rejoin, no chat, and games are not saved to disk.
- The client connects to an address set at compile time.
