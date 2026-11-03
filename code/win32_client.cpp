#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <Windows.h>
#include "Client_TicTacToe.cpp"
#include "Shared.h"
#include "Shared.cpp"
#include "utility.cpp"


#include <conio.h>


// TODO(mark): get the correct turn to display
// TODO(mark): start processing user input so i can finish server recving part
// TODO(mark): test that the display code works

// NOTE(mark): i want to function to pick how may games to display and how many columin and row to have

// TODO(mark): figure out how i want the single game and chat to look
//sytle_template GameTest1 = {0};
//GameTest1.data[1000];
// TODO(mark): add a sleep so the console app runs at 60fps. not to eat all the compute for nothing

// TODO(mark): process user input
// TODO(mark): think how i want to display games
// TODO(mark): handle multi games
// TODO(mark): add chat


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define TEST_IP "192.168.1.25"
#define DEFAULT_NUMBER_OF_SOCKETS 10
#define DEFAULT_WAIT_TIME 10

// NOTE(mark): Game settings
#define MAX_GAMES 10


// NOTE(Mark): Games display settings
#define MAX_GAMES_DISPLAY_BUFFER 5000
#define MAX_DISPLAY_GAMES 10
#define MAX_DISPLAY_ROWS 2
#define MAX_DISPLAY_GAMES_PER_ROW 5



// TODO(mark): figure out what these setting should be
#define MAX_DISPLAY_LINE_BUFFER 1000
//#pragma comment(lib, "Ws2_32.lib")


int recvbuflen = DEFAULT_BUFLEN;
char recvbuf[DEFAULT_BUFLEN];

// TODO(mark): move to a generic shared header file i use
#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
#define ArrayZero(Array) (memset(Array, 0, ArrayCount(Array)))
// TODO(mark): this macro is not safe. should i check for overflow?
#define ArrayPartZero(Array, Loc, Amount) (memset((Arrary + Loc), 0, Amount));
// TODO(mark): need to test this macro

// TODO(mark): change this struct's names to something better
struct sytle_template
{
    int CharPerRow;
    int RowsPerGame;
    // NOTE(mark): not sure if SizeBeforeFormat is need?
    int SizeBeforeFormat;
    int SizeAfterFormat;
    char *data;
};

struct client_game_data
{
    int NumberOfGames;
    uint64_t GameId[MAX_GAMES];
    board_postions Game[MAX_GAMES];
    // TODO(mark): figure out if i want to change how the tic tac toe his handle because it is hard to hand playerid vs player (88, 79)
};

void ClearLine(int size)
{
    for (int PostionIndex = 0;
         PostionIndex < size;
         PostionIndex++)
    {
        _putch(0);
    }
    _putch(13);
}



void DisplayMainMenu(client_game_data *Games, sytle_template *GameLayout)
{
    char DisplayBuffer[MAX_GAMES_DISPLAY_BUFFER];
    char DisplayLine[MAX_DISPLAY_LINE_BUFFER];
    char TurnMessage[20] = " Your Turn      ";
    int NumberOfRowDisplayed;
    int GamesPerRow;
    int NumberOfRowWithExtraGame;
    
    // NOTE(mark): exit early if there are no games. so it doesn't have divide by zero error
    if (Games->NumberOfGames == 0) return;
    
    //NOTE(mark): figure out how many games need to be displayed
    int NumberOfGamesDisplayed = MAX_DISPLAY_GAMES;
    if (NumberOfGamesDisplayed > Games->NumberOfGames) NumberOfGamesDisplayed = Games->NumberOfGames;
    
    
    // NOTE(mark): figure out how many rows are need 
    if (MAX_DISPLAY_ROWS > Games->NumberOfGames)
    {
        NumberOfRowDisplayed = Games->NumberOfGames;
    }
    else 
    {
        NumberOfRowDisplayed = MAX_DISPLAY_ROWS;
    }
    //NOTE(mark): figure out how many games per row
    GamesPerRow = MAX_DISPLAY_GAMES_PER_ROW;
    if (GamesPerRow > (Games->NumberOfGames / NumberOfRowDisplayed)) GamesPerRow = (Games->NumberOfGames / NumberOfRowDisplayed);
    NumberOfRowWithExtraGame = Games->NumberOfGames % NumberOfRowDisplayed;
    
    
    //if (NUmberOfRowDisplayed > (Games->NumberOfGames));
    
    
    
    // NOTE(mark): fill out the template with the game state
    printf("%d\n", NumberOfGamesDisplayed);
    for (int GameIndex = 0;
         GameIndex < NumberOfGamesDisplayed;
         GameIndex++)
    {
        sprintf(DisplayBuffer + (GameIndex * GameLayout->SizeAfterFormat), GameLayout->data,
                TurnMessage, 
                Games->Game[GameIndex].TopLeft, Games->Game[GameIndex].TopMiddle, Games->Game[GameIndex].TopRight,
                Games->Game[GameIndex].MiddleLeft, Games->Game[GameIndex].MiddleMiddle, Games->Game[GameIndex].MiddleRight,
                Games->Game[GameIndex].BottomLeft, Games->Game[GameIndex].BottomMiddle, Games->Game[GameIndex].BottomRight);
    }
    
    // NOTE(mark): arrange and display  the games 
    // TODO(mark): this is broken
    for (int RowIndex = 0;
         RowIndex < (NumberOfRowDisplayed * GameLayout->RowsPerGame);
         RowIndex++)
    {
        for (int GameIndex = 0;
             GameIndex < (GamesPerRow);
             GameIndex++)
        {
            strncpy(DisplayLine + (GameIndex * GameLayout->CharPerRow), 
                    DisplayBuffer + ((GameLayout->CharPerRow * RowIndex) + (GameIndex * GameLayout->SizeAfterFormat)), 
                    GameLayout->CharPerRow);
            if (GameIndex + 1 == GamesPerRow)
            {
                // NOTE(mark): check if this row needs extra game displayed
                if (NumberOfRowWithExtraGame)
                {
                    strncpy(DisplayLine + (GameIndex * GameLayout->CharPerRow), 
                            DisplayBuffer + ((GameLayout->CharPerRow * RowIndex) + (GameIndex * GameLayout->SizeAfterFormat)), 
                            GameLayout->CharPerRow);
                    
                    NumberOfRowWithExtraGame--;
                }
                strncpy((DisplayLine + (GameLayout->CharPerRow * (GameIndex + 1))), "\n\0", 2);
            }
            
        }
        printf(DisplayLine);
    }
}

int CheckInput(char *Buffer, int *BufferIndex, int BufferSize)
{
    int Result = 0;
    char tempchar;
    if (_kbhit())
    {
        
        tempchar = _getche();
        Buffer[*BufferIndex] = tempchar;
        (*BufferIndex)++;
        if (*BufferIndex >= BufferSize)
        {
            // TODO(mark): this stop overflow but not sure if this is how i want to handle it?
            BufferIndex = 0;
        }
        //printf("%d\n", tempchar);
        if (tempchar == 8)
        {
            // NOTE(mark): these wipe the char from the stdout after backspace key has move the cursor backwords
            _putch(0);
            // NOTE(mark): send the backspace key to stdout to move the cursor location backwords after the blank of the char move the cursor forward
            _putch(tempchar);
        }
        if (tempchar == 13)
        {
            // TODO(mark): have to figure out when to clear the buffer?
            ClearLine(*BufferIndex);
            //printf("cleared: %s\n", testBuffer);
            BufferIndex = 0;
            Result = 1;
        }
        if (tempchar == 3) {
            // TODO(mark); figure out how i want to handle Ctrl-C
            printf("testing\n");
            return 0;
        }
        if (!tempchar)
        {
            printf("2) char: %c char: %d\n", tempchar, tempchar);
            tempchar = _getch();
        }
    }
    return Result;
}


inline int GetGameIndex(client_game_data *Games, uint64_t GameId)
{
    int Result;
    for (int Index = 0;
         Index < Games->NumberOfGames;
         Index++)
    {
        if (Games->GameId[Index] == GameId) Result = Index;
    }
    return Result;
}

// TODO(mark): not sure if i want this function to return something
bool32 PlayMoveClient(SOCKET ConnectSocket, uint64_t PlayerId, uint64_t GameId, MessageAction *MessageMoveOut)
{
    bool32 Result = 0;
    char input[65];
    printf("Your Move: ");
    
    fgets(input, 15, stdin);
    // TODO(mark): maker sure this is need if not go through the code and remove it
    input[strlen(input) - 1] = '\0';
    ToUpper(input, 4);
    MessageMoveOut->Move = ConvertStrToMove((input));
    MessageMoveOut->PlayerId = PlayerId;
    MessageMoveOut->GameId = GameId;
    return SendMessage(ConnectSocket, (char *)MessageMoveOut, sizeof(MessageAction));
}

#define MAX_GAMES 10

/*
struct games
{
    board_postions boards[MAX_GAMES];
}
*/

int main() {
    printf("Starting..\n");
    // NOTE(mark): new code here
    client_game_data Games = {};
    
    WSAPOLLFD SocketArray[DEFAULT_NUMBER_OF_SOCKETS] = {}; 
    SocketArray[0].fd = INVALID_SOCKET;
    SocketArray[0].events = POLLRDNORM;
    
    // TODO(mark): need to ask user for server ip?
    char ServerName[] = TEST_IP;
    char Port_Number[] = DEFAULT_PORT;
    
    int ServerResponse;
    int PollResult;
    int GameIndex;
    uint64_t PlayerId = UINT64_MAX;
    int UpdateFlag = 1;
    // TODO(mark): setup socket polling 
    // TODO(mark): rethink how the handle sned and reciving data from the server
    
    
    
    
    // TODO: fix this //NOTE(mark): using uint64_t max as error currently
    
    
    
    
    // NOTE(mark): code below here will be keep after finishing the new code
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    MessageAction MessageMove = {};
    GameAction move = {};
    bool32 LegalMove = false;
    char UserInput[65];
    bool32 MyTurn = true; // NOTE(mark): not sure about if it should be true or false
    
    
    // TODO(mark): handle conneting to the server message and UserInput here
    // TODO(mark): figure out how i want to handle server ip and port?
    
    WSADATA wsaData;
    int iResult;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }
    
    // getting socket
    struct addrinfo *result = NULL, hints;
    
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    
    
    
    // Resolve the server address and port
    
    iResult = getaddrinfo(ServerName, Port_Number, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        printf("Press enter to close\n");
        fgets(UserInput, 15, stdin);
        WSACleanup();
        return 1;
    }
    
    //SOCKET ConnectSocket = INVALID_SOCKET;
    
    // Create a SOCKET for connecting to server
    SocketArray[0].fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (SocketArray[0].fd == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        printf("Press enter to close\n");
        fgets(UserInput, 15, stdin);
        return 1;
    }
    
    // Connect to server.
    iResult = connect( SocketArray[0].fd, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(SocketArray[0].fd);
        SocketArray[0].fd = INVALID_SOCKET;
    }
    
    // Should really try the next address returned by getaddrinfo
    // if the connect call failed
    // But for this simple example we just free the resources
    // returned by getaddrinfo and print an error message
    
    freeaddrinfo(result);
    
    if (SocketArray[0].fd == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        printf("Press enter to close\n");
        fgets(UserInput, 15, stdin);
        return 1;
    }
    
    
    // Sendding and reciving 
    
    
    
    // TODO(mark): first the client look if it has save player id if not then ask user for a player id or to create one
    // connect to the server with the player id then the server tell the client what games he's in and which was are his
    // turn to play a move. then the client display this to the user with other standard option like join a game or create a game
    // and etc.
    //
    
    // TODO(mark): add the abilty to for the sever to tell the client it their turn
    printf("Starting... ");
    
    // TODO(mark): test connecting to the server
    // NOTE(mark): temp code
    // create player
    // ask to join or create game
    char Valid = CREATE_PLAYER;
    if (!SendMessage(SocketArray[0].fd, (char *)&Valid, sizeof(char)))
    {
        return 1;
    }
    
    
    // NOTE(mark): this should be play game function wants it cleaned up 
    // Receive data until the server closes the connection
    
    char testBuffer[100] = {0};
    int BufferIndex = 0;
    
    
    sytle_template GameLayout = {0};
    GameLayout.data = "                   | 1)%s|                   |     %c | %c | %c     |    ---|---|---    |     %c | %c | %c     |    ---|---|---    |     %c | %c | %c     |___________________|";
    //NOTE(mark): this is not realy true because %c is count as one because %c will be replaced with one char
    GameLayout.CharPerRow = 20;
    GameLayout.RowsPerGame = 9;
    //GameLayout.size = 8 * 20;
    GameLayout.SizeBeforeFormat = strlen(GameLayout.data);
    GameLayout.SizeAfterFormat = GameLayout.CharPerRow * GameLayout.RowsPerGame;
    // DEBUG: games not working
    
    
    // TODO(mark): load saved data (playerid, and game state)
    // wait for user to create a player
    // then go in to main loop
    int Running = 1;
    while(Running){
        // TEST CODE
        
        // NOTE(mark): Rendering
        if (UpdateFlag)
        {
            DisplayMainMenu(&Games, &GameLayout);
            UpdateFlag = 0;
        }
        
        // NOTE(mark): handle User Input
        if (CheckInput(testBuffer, &BufferIndex, sizeof(testBuffer)))
        {
            // TODO(mark): do something with input
            printf("does this work? %s\n", testBuffer);
            BufferIndex = 0;
            ArrayZero(testBuffer);
            
        }
        
        // NOTE(mark): handle Server response
        PollResult = WSAPoll(SocketArray, DEFAULT_NUMBER_OF_SOCKETS, DEFAULT_WAIT_TIME);
        if (PollResult == SOCKET_ERROR)
        {
            printf("WSAPoll failed: %d\n", WSAGetLastError());
            closesocket(SocketArray[0].fd);
            WSACleanup();
            return 1;
        }
        
        
        // NOTE(mark): this code assumes the client will connect to at most one server
        if (PollResult > 0)
        {
            ServerResponse = recv(SocketArray[0].fd, recvbuf, recvbuflen, 0);
            // TODO (mark): not sure how i want to handle game over yet?
            if (ServerResponse == 1)
            {
                if (*recvbuf == YOUR_TURN)
                {
                    /*
                if (!PlayMoveClient(SocketArray[0].fd, PlayerId, GameId, &MessageMove))
                {
                    return 1;
                }
*/
                }
                else if (*recvbuf == INVALID_MOVE)
                {
                    /*
                printf("Move: %s not allowed or has been played\n", UserInput);
                if (!PlayMoveClient(SocketArray[0].fd, PlayerId, GameId, &MessageMove))
                {
                    return 1;
                }
*/
                }
            }
            else if (ServerResponse == sizeof(PartMove))
            {
                // NOTE(mark): this only work if MOVE_Ok and GAMEOVER use the PartMove struct
                move.move = MessageMove.Move;
                move.player = recvbuf[1];
                GameIndex = GetGameIndex(&Games, MessageMove.GameId);
                PlayMove(&Games.Game[GameIndex], move);
                EvalGame(&Games.Game[GameIndex]);
                UpdateFlag = 1;
                if (recvbuf[0] == GAMEOVER)
                {
                    printf("Game Over\n");
                    break;
                }
            }
            else if (ServerResponse == sizeof(id))
            {
                if ((char)*recvbuf == CREATE_PLAYER_SUCCESS)
                {
                    PlayerId = UnpackId(*(id *)recvbuf);
                    printf("PlayerId: %llu\n", PlayerId);
                    printf("1) Create Game\n2) JoinGame\n>");
                    fgets(UserInput, 15, stdin);
                    if (UserInput[0] == '1')
                    {
                        Valid = CREATE_GAME;
                        if (!SendMessage(SocketArray[0].fd, (char *)&Valid, sizeof(char)))
                        {
                            return 1;
                        }
                    }
                    else if (UserInput[0] == '2')
                    {
                        printf("Join Game Code: ");
                        fgets(UserInput, 15, stdin);
                        Games.GameId[Games.NumberOfGames] = StrToInt64(UserInput, strlen(UserInput));
                        id Message = PackId(JOIN_GAME, Games.GameId[Games.NumberOfGames]);
                        if (!SendMessage(SocketArray[0].fd, (char *)&Message, sizeof(id)))
                        {
                            return 1;
                        }
                        Games.NumberOfGames++;
                    }
                }
                else if ((char)*recvbuf == GAMEID)
                {
                    // TODO(mark): make this more robut
                    // for example if you chose to be player two you can't make a move and have to wait
                    // need to add gameid to the index of game the player is currently playing
                    Games.GameId[Games.NumberOfGames] = UnpackId(*(id *)recvbuf);
                    Games.Game[Games.NumberOfGames] = CreateBoard();
                    printf("Join Code: %llu\n", Games.GameId[Games.NumberOfGames]);
                    if (!PlayMoveClient(SocketArray[0].fd, PlayerId, Games.GameId[Games.NumberOfGames], &MessageMove))
                    {
                        return 1;
                    }
                    Games.NumberOfGames++;
                }
            }
            else if (ServerResponse == sizeof(MessageAction))
            {
                // NOTE(mark): recive and update other players move
                //printf("Other player's move\n");
                MessageMove = *(MessageAction *)recvbuf;
                move.move = MessageMove.Move;
                //move.player = MessageMove.PlayerId;
                GameIndex = GetGameIndex(&Games, MessageMove.GameId);
                PlayMove(&Games.Game[GameIndex], move);
                EvalGame(&Games.Game[GameIndex]);
                UpdateFlag = 1;
            }
            else if (ServerResponse == sizeof(board_postions))
            {
                // TODO(mark): make more robust
                // TODO(mark): need to send the gameid and the board postions
                // NOTE(mark): i think this is not implmented on the server yet
                //board = *(board_postions *)recvbuf;
                //UpdateBoard(board);
            }
            
            
            else if (ServerResponse == 0)
                printf("Connection closed\n");
            else
                printf("recv failed: %d\n", WSAGetLastError());
        }
    }
    
    // NOTE(mark): get response and update display
    
    
    
    
    
    
    // shutdown the send half of the connection since no more data will be sent
    ServerResponse = shutdown(SocketArray[0].fd, SD_SEND);
    if (ServerResponse == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(SocketArray[0].fd);
        WSACleanup();
        return 1;
    }
    
    // cleanup
    closesocket(SocketArray[0].fd);
    WSACleanup();
    
    printf("Press enter to close\n");
    fgets(UserInput, 15, stdin);
    
    return 0;
}