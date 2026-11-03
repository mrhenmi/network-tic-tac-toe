#include <winsock2.h>
#include <ws2tcpip.h>
#include "Client_TicTacToe.cpp"
#include "Shared.h"
#include "Shared.cpp"
#include "utility.cpp"


#define DEFAULT_BUFLEN 512
#define DEFAULT_WAIT_TIME 1000
#define MAX_PLAYERS 10
#define MAX_GAMES_PER_PLAYER 10
#define MAX_GAMES 100
#define DEFAULT_NUMBER_OF_SOCKETS 10
#define DEFAULT_PORT "27015"



struct game_data
{
    uint64_t GameId;
    // NOTE(mark): bitmask of if the players where connect the last time we checked
    char PlayersConnected;
    uint64_t PlayerOneId;
    uint64_t PlayerTwoId;
    board_postions game;
    // NOTE(mark): not sure about these part
    
};

struct player_data {
    // TODO(mark): not sure if i want to player id in here?
    // NOTE(mark): set SocketIndex to zero when player disconnects 
    // set SocketIndex to what index of SocketArray the player connected with
    int SocketIndex;
    bool32 GamesFull;
    uint64_t PlayerId;
    uint64_t GameId[MAX_GAMES_PER_PLAYER];
};


// TODO(mark): figure out if we want the player id sent with most message or just look them up?
// i think look them up
// NOTE(mark): winsock example 
// https://docs.microsoft.com/en-us/windows/win32/winsock/getting-started-with-winsock

// NOTE(mark): socket error codes
// https://docs.microsoft.com/en-us/windows/win32/winsock/windows-sockets-error-codes-2

// NOTE(mark): select and wsapool for better handling
// https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-select

// NOTE(mark): maybe need a work on the structs becuase both the client and serve need the gameid 
// but the server need to know who's turn it is using player id but the client should only see player 2 for example
// other case may show up to so think about how to handle this

// NOTE(mark): When starting again Start Here
// TODO(mark): seperate join code and gameid
// TODO(mark): Make join easy to type in for a human so probaly a-z and 0-9
// use code either 3 (36^4 = 46656) or 4 (36^4 = 1679616) long NOTE(mark): make the join code length a define
// TODO(mark): Work on client
// TODO(mark): get client to handle multi games 
// TODO(mark): figure out how the client should keep tack of which move is for which game
// TODO(mark): figure out how the chat box should look 
// TODO(mark): add the ablilty to chat in the client
// TODO(mark): think about adding password, playerid and usernames?

// TODO(mark): think about refactoring and file name and what should be in each file

// NOTE(mark): For later
// TODO(mark): when i come back refactor wher code goes move it around
// build a test client that does nothing but player a lot of player and games and plays them
// to test how much the computer can run. then decided to have games and players store on disk
// don't have to max the computer but check how many games and player in 1gb of ram and how much cpu use
// then work out how to seprate the gameid/playerid so you can have an array in ram of player and 
// array of player stored on disk
// TODO(mark): redo the player creation maybe
// TODO(mark): figure out how i want to rejoin a game and update the client
// TODO(mark): reload / sync game state

// TODO(mark): add forfeit as a move to the game
// TODO(mark): add delete of the game data when both party have been to the game is over


// NOTE(mark): Features For Later
// TODO(mark): add quick play 
// TODO(mark): currently using totalgame and total player to make id don't do that later
// TODO(mark): add saving and loading
// TODO(mark): look in to the differnt ways to recv data over the socket
// TODO(mark): simd GetPlayerId and GetGameId
// TODO(mark); multi-thread 
// TODO(mark): do i want the join codes to be the gameid? probly not
// TODO(mark): add chat system
// TODO(mark): make struct to save games on the server when you run out of memmory
// it should have game id and board_postions
// etc.

// NOTE(mark): Client list



// TODO (mark): not sure if want to return the index or game_data
uint64_t GetGameIndexById(game_data *Games, uint64_t GameId)
{
    uint64_t Result = 0;
    for (int GameIndex = 0;
         GameIndex < MAX_GAMES;
         GameIndex++)
    {
        if (Games[GameIndex].GameId == GameId)
        {
            Result = GameIndex;
            return Result;
        }
    }
    return Result;
}

char GetPlayerSpot(uint64_t PlayerId, game_data *data)
{
    char Result = 0;
    if (data->PlayerOneId == PlayerId)
    {
        Result = PLAYERONEVALUE;
    }
    else if (data->PlayerTwoId == PlayerId)
    {
        Result = PLAYERTWOVALUE;
    }
    else 
    {
        Result = 0xff;
    }
    return Result;
}

uint64_t GetPlayerIndexById(player_data *players, uint64_t PlayerId)
{
    uint64_t Result = 0;
    for (int PlayerIndex = 0;
         PlayerIndex < MAX_PLAYERS;
         PlayerIndex++)
    {
        if (players[PlayerIndex].PlayerId == PlayerId)
        {
            Result = PlayerIndex;
            return Result;
        }
    }
    return Result;
}

int GetPlayerSocketIndexById(player_data *players, uint64_t PlayerId)
{
    int Result = 0;
    for (int PlayerIndex = 0;
         PlayerIndex < MAX_PLAYERS;
         PlayerIndex++)
    {
        if (players[PlayerIndex].PlayerId == PlayerId)
        {
            Result = players[PlayerIndex].SocketIndex;
            return Result;
        }
    }
    return Result;
}

uint64_t GetPlayerIdBySocketIndex(player_data *players, int SocketIndex)
{
    uint64_t Result = 0;
    for (int PlayerIndex = 0;
         PlayerIndex < MAX_PLAYERS;
         PlayerIndex++)
    {
        if (players[PlayerIndex].SocketIndex == SocketIndex)
        {
            Result = players[PlayerIndex].PlayerId;
            return Result;
        }
    }
    return Result;
}

inline bool32
SendGameState(SOCKET Socket, board_postions *GameState)
{
    return SendMessage(Socket, (char *)GameState, sizeof(board_postions));
}


WSADATA wsaData;


int main()
{
    // NOTE(mark): temp game setup
    
    bool32 LegalMove = false;
    // NOTE(mark): value check
    Assert((MAX_PLAYERS * MAX_GAMES_PER_PLAYER) <= MAX_GAMES);
    
    int iResult;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }
    
    // setup address
    
    
    struct addrinfo *result = NULL, hints;
    
    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    
    // Resolve the local address and port to be used by the server
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }
    
    // Create a SOCKET for the server to listen for client connections
    
    WSAPOLLFD SocketArray[DEFAULT_NUMBER_OF_SOCKETS] = {}; 
    SocketArray[0].fd = INVALID_SOCKET;
    SocketArray[0].events = POLLRDNORM;
    
    //SOCKET ListenSocket = INVALID_SOCKET;
    SocketArray[0].fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (SocketArray[0].fd == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    
    // Setup the TCP listening socket
    iResult = bind(SocketArray[0].fd, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        
        
        
        closesocket(SocketArray[0].fd);
        WSACleanup();
        return 1;
    }
    freeaddrinfo(result);
    
    // Listen to socket
    if ( listen(SocketArray[0].fd, SOMAXCONN ) == SOCKET_ERROR ) {
        printf( "Listen failed with error: %ld\n", WSAGetLastError() );
        closesocket(SocketArray[0].fd);
        WSACleanup();
        return 1;
    }
    
    // accepting connection
    // TODO(mark): look into this https://docs.microsoft.com/en-us/windows/win32/winsock/accepting-a-connection
    bool32 Running = true;
    
    // TODO(mark): add way to create players and fill out playerdata 
    player_data Players[MAX_PLAYERS] = {};
    game_data Games[MAX_GAMES] = {};
    
    
    
    char recvbuf[DEFAULT_BUFLEN];
    int PollResult;
    int PlayerResult;
    int Result;
    //int SendResult;
    int recvbuflen = DEFAULT_BUFLEN;
    int TotalConnectedSockets = 1;
    uint64_t TotalPlayers = 0;
    uint64_t TotalGames = 0;
    
    char Valid = 0;
    char Codes[30];
    
    
    
    /*
    typedef struct pollfd {
        SOCKET fd;
        SHORT  events;
        SHORT  revents;
    } WSAPOLLFD, *PWSAPOLLFD, *LPWSAPOLLFD;
    
    int WSAAPI WSAPoll(
                       LPWSAPOLLFD fdArray,
                       ULONG       fds,
                       INT         timeout
                       );
    */
    /*
    POLLERR 	An error has occurred.
        POLLHUP 	A stream-oriented connection was either disconnected or aborted.
        POLLNVAL 	An invalid socket was used.
        
        
        POLLRDBAND 	Priority band (out-of-band) data may be read without blocking.
        POLLRDNORM 	Normal data may be read without blocking.
        POLLWRNORM 	Normal data may be written without blocking.
    */
    
    // NOTE(mark): not sure where to check if the conncection is still alive
    
    // Setup WSAPOLLFD for WSAPOLL function
    
    for (int SocketIndex = 1;
         SocketIndex < DEFAULT_NUMBER_OF_SOCKETS;
         SocketIndex++)
    {
        SocketArray[SocketIndex].fd = INVALID_SOCKET;
        // TODO(mark): check if this is ok to do here. not if it will be happy with setting events when we want to ignore them.
        SocketArray[SocketIndex].events = POLLIN;
    }
    
    
    while(Running)
    {
        PollResult = WSAPoll(SocketArray, DEFAULT_NUMBER_OF_SOCKETS, DEFAULT_WAIT_TIME);
        if (PollResult == SOCKET_ERROR)
        {
            printf("WSAPoll failed: %d\n", WSAGetLastError());
            closesocket(SocketArray[0].fd);
            WSACleanup();
            return 1;
        }
        else if (PollResult > 0)
        {
            
            // Check if new connection
            // TODO(mark): handler ERROR from the ListenSocket 
            // NOTE(mark): not sure if we want or need to handle other request on the ListenSocket
            if (SocketArray[0].revents & POLLRDNORM)
            {
                
                if (TotalConnectedSockets <= DEFAULT_NUMBER_OF_SOCKETS)
                {
                    for (int SocketIndex = 1;
                         SocketIndex < DEFAULT_NUMBER_OF_SOCKETS;
                         SocketIndex++)
                    {
                        if (SocketArray[SocketIndex].fd == INVALID_SOCKET)
                        {
                            SocketArray[SocketIndex].fd = accept(SocketArray[0].fd, NULL, NULL);
                            
                            // TODO(mark): figure out if this check is need?
                            // and if so how we want to handle it
                            if (SocketArray[SocketIndex].fd == INVALID_SOCKET) {
                                printf("accept failed: %d\n", WSAGetLastError());
                                closesocket(SocketArray[0].fd);
                                WSACleanup();
                                return 1;
                            }
                            TotalConnectedSockets++;
                            break;
                        }
                    }
                }
                
                else 
                {
                    // TODO(mark): dynamicly scale the Socket array up to a max and down to a min by default expand size
                    // maybe? or maybe just have total amount of socket prealloc. not sure have to think about it more
                    printf("Too many socket connections!\n");
                }
            }
            // handle connection
            if (TotalConnectedSockets > 1)
            {
                for (int SocketIndex = 1;
                     SocketIndex < DEFAULT_NUMBER_OF_SOCKETS;
                     SocketIndex++)
                {
                    
                    if ((SocketArray[SocketIndex].fd >= 0) && (SocketArray[SocketIndex].revents & POLLHUP))
                    {
                        printf("Socket disconnected\n");
                        SocketArray[SocketIndex].fd = INVALID_SOCKET;
                        // TODO(mark): figure out what i want player socketindex to equal when not connected
                        uint64_t playerid = GetPlayerIdBySocketIndex(Players, SocketIndex);
                        Players[playerid].SocketIndex = 0;
                        TotalConnectedSockets--;
                    }
                    if ((SocketArray[SocketIndex].fd >= 0) && (SocketArray[SocketIndex].revents & POLLIN))
                    {
                        printf("Recving client data\n");
                        // NOTE(mark): message code from Client 
                        // 8 bytes is a playerid message sent from the client
                        // sizeof(action) is a move
                        // TODO: add create game code
                        // TODO: add join game code
                        // TODO: create player code
                        
                        PlayerResult = recv(SocketArray[SocketIndex].fd, recvbuf, recvbuflen, 0);
                        //printf("PlayerOneResult: %d action size: %llu\n", PlayerOneResult, sizeof(action));
                        if (PlayerResult == sizeof(MessageAction)) {
                            // NOTE(mark): call game function here then send back result / update
                            // TODO(mark): verify that action player matchs what the server think the socket player is
                            MessageAction MessageMove = *(MessageAction *)recvbuf;
                            GameAction move = {};
                            move.move = MessageMove.Move;
                            
                            uint64_t GameIndex = GetGameIndexById(Games, MessageMove.GameId);
                            char PlayerSpot = GetPlayerSpot(MessageMove.PlayerId, &Games[GameIndex]);
                            move.player = PlayerSpot;
                            MessageMove.PlayerId = PlayerSpot;
                            // ASSERT: the player wast not part of this game!
                            Assert(move.player != 0xff);
                            LegalMove = PlayMove(&Games[GameIndex].game, move);
                            if (LegalMove)
                            {
                                EvalGame(&Games[GameIndex].game);
                                UpdateBoard(Games[GameIndex].game);
                                PartMove UpdateMessage = {};
                                if (Games[GameIndex].game.GameOver)
                                {
                                    UpdateMessage.Code = GAMEOVER;
                                    UpdateMessage.PlayerSpot = move.player;
                                    UpdateMessage.GameId = MessageMove.GameId;
                                }
                                else 
                                {
                                    UpdateMessage.Code = MOVE_OK;
                                    UpdateMessage.PlayerSpot = move.player;
                                    UpdateMessage.GameId = MessageMove.GameId;
                                }
                                if (!SendMessage(SocketArray[SocketIndex].fd, (char *)&UpdateMessage, sizeof(PartMove)))
                                {
                                    return 1;
                                }
                                // NOTE(mark): tell player one it is a valid move
                                // NOTE(mark): message code to client
                                // gameover
                                // move ok
                                // invalid move
                                // game created success
                                // game joined success
                                // player created success
                                // your turn
                                // maybe? not your turn
                                // other player move
                                // send game board over
                                // etc.
                                
                                
                                // NOTE(mark): tell player two what player ones move was
                                // TODO(MARK): Figure how i want to let other player know the game end and update there board
                                if (PlayerSpot == PLAYERONEVALUE)
                                {
                                    if (Games[GameIndex].PlayersConnected & 0x02)
                                    {
                                        int OtherPlayerSocketIndex = GetPlayerSocketIndexById(Players, Games[GameIndex].PlayerTwoId);
                                        if (!SendMessage(SocketArray[OtherPlayerSocketIndex].fd, (char *)&MessageMove, sizeof(MessageAction)))
                                        {
                                            return 1;
                                        }
                                    }
                                }
                                else 
                                {
                                    if (Games[GameIndex].PlayersConnected & 0x01)
                                    {
                                        int OtherPlayerSocketIndex = GetPlayerSocketIndexById(Players, Games[GameIndex].PlayerOneId);
                                        if (!SendMessage(SocketArray[OtherPlayerSocketIndex].fd, (char *)&MessageMove, sizeof(MessageAction)))
                                        {
                                            return 1;
                                        }
                                    }
                                }
                                // TODO(mark): this break is temp i don't like it remove later
                                // when working on a-sync 
                                
                            }
                            else
                            {
                                Valid = INVALID_MOVE;
                                if (!SendMessage(SocketArray[SocketIndex].fd, (char *) &Valid, sizeof(char)))
                                {
                                    return 1;
                                }
                            }
                        }
                        else if (PlayerResult == 1)
                        {
                            if ((char)*recvbuf == CREATE_GAME)
                            {
                                // TODO(mark): allow the player to pick if they are player one or two
                                // JOIN_GAME will have to be update too.
                                uint64_t PlayerId = GetPlayerIdBySocketIndex(Players, SocketIndex);
                                bool32 full = true;
                                Games[TotalGames].GameId = TotalGames;
                                Games[TotalGames].game = CreateBoard();
                                Games[TotalGames].PlayerOneId = PlayerId; //NOTE(mark): think
                                Games[TotalGames].PlayersConnected = 0x01;
                                if (!Players[PlayerId].GamesFull)
                                {
                                    for (int GameIdIndex = 0;
                                         GameIdIndex < MAX_GAMES_PER_PLAYER;
                                         GameIdIndex++)
                                    {
                                        // TODO(mark): not sure what i want this check to be?
                                        if (Players[PlayerId].GameId[GameIdIndex] == 0)
                                        {
                                            Players[PlayerId].GameId[GameIdIndex] = TotalGames;
                                            full = false;
                                            break;
                                        }
                                    }
                                    if (full)
                                    {
                                        Players[PlayerId].GamesFull = 1;
                                    }
                                    id NewGame = PackId(GAMEID, TotalGames);
                                    // NOTE(mark): note sure if want to incerment TotalGames before or after SendMessage
                                    TotalGames++;
                                    if (!SendMessage(SocketArray[SocketIndex].fd, (char *) &NewGame, sizeof(id)))
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    Valid = CREATE_GAME_FAILED_FULL;
                                    if (!SendMessage(SocketArray[SocketIndex].fd, (char *) &Valid, sizeof(char)))
                                    {
                                        return 1;
                                    }
                                }
                            }
                            else if ((char)*recvbuf == CREATE_PLAYER)
                            {
                                Players[TotalPlayers].SocketIndex = SocketIndex;
                                Players[TotalPlayers].PlayerId = TotalPlayers;
                                id PlayerId = PackId(CREATE_PLAYER_SUCCESS, TotalPlayers);
                                TotalPlayers++;
                                if (!SendMessage(SocketArray[SocketIndex].fd, (char *)&PlayerId, sizeof(id)))
                                {
                                    return 1;
                                }
                            }
                        }
                        // NOTE(mark): player connect to server
                        else if (PlayerResult == sizeof(id))
                        {
                            // TODO(mark): do we want the code PLAYERID or connect to server?
                            if ((char)*recvbuf == PLAYERID)
                            {
                                printf("recived Player id\n");
                                uint64_t PlayerId = UnpackId(*(id *)recvbuf);
                                uint64_t PlayerIndex = GetPlayerIndexById(Players, PlayerId);
                                Players[PlayerIndex].SocketIndex = SocketIndex;
                                Valid = CONNECTED;
                                if (!SendMessage(SocketArray[SocketIndex].fd, (char *) &Valid, sizeof(char)))
                                {
                                    return 1;
                                }
                            }
                            else if ((char) *recvbuf == JOIN_GAME)
                            {
                                uint64_t GameId = UnpackId(*(id *)recvbuf);
                                uint64_t GameIndex = GetGameIndexById(Games, GameId);
                                if (Games[GameIndex].PlayerTwoId == 0)
                                {
                                    Games[GameIndex].PlayersConnected = Games[GameIndex].PlayersConnected | 0x02;
                                    Games[GameIndex].PlayerTwoId = GetPlayerIdBySocketIndex(Players, SocketIndex);
                                    // TODO(mark): allow the player join a game to be player one
                                    // NOTE(mark): current solution always assume player join a game is player two
                                    if (Games[GameIndex].game.CurrentPlayerTurn == PLAYERTWOVALUE)
                                    {
                                        if (SendGameState(SocketArray[SocketIndex].fd, &Games[GameIndex].game))
                                        {
                                            Valid = YOUR_TURN;
                                        }
                                        else
                                        {
                                            Valid = FAILED_TO_SEND_GAME_STATE;
                                        }
                                    }
                                    else
                                    {
                                        Valid = NOT_YOUR_TURN;
                                    }
                                }
                                else 
                                {
                                    Valid = JOIN_GAME_FAILED_FULL;
                                }
                                if (!SendMessage(SocketArray[SocketIndex].fd, (char *) &Valid, sizeof(char)))
                                {
                                    return 1;
                                }
                            }
                        }
                        else if (PlayerResult == 0)
                        {
                            printf("Connection closing...\n");
                        }
                        else {
                            printf("recv failed: %d\n", WSAGetLastError());
                            closesocket(SocketArray[SocketIndex].fd);
                            WSACleanup();
                            return 1;
                        }
                    }
                }
            }
        }
    }
    // TODO(mark): peek at stdin if quit then exit
    
    // shutdown the send half of the connection since no more data will be sent
    
    // not sure if you need clean up
    for (int SocketIndex = 0;
         SocketIndex < DEFAULT_NUMBER_OF_SOCKETS;
         SocketIndex++)
    {
        Result = shutdown(SocketArray[SocketIndex].fd, SD_SEND);
        // not sure if i need to handle error when closing everthing
        if (Result == SOCKET_ERROR) {
            printf("shutdown failed: %d\n", WSAGetLastError());
            closesocket(SocketArray[SocketIndex].fd);
            WSACleanup();
            return 1;
        }
        
        // cleanup
        closesocket(SocketArray[SocketIndex].fd);
    }
    WSACleanup();
    return 0;
}
