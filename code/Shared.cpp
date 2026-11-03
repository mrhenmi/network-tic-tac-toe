char *PrintGameMoves(Game_moves move)
{
    switch(move)
    {
        case TL:
        {
            return "Top Left";
        }
        case TM:
        {
            return "Top Middle";
        }
        case TR:
        {
            return "Top Right";
        }
        case ML:
        {
            return "Middle Left";
        }
        case MM:
        {
            return "Middle Middle";
        }
        case MR:
        {
            return "Middle Right";
        }
        case BL:
        {
            return "Bottom Left";
        }
        case BM:
        {
            return "Bottom Middle";
        }
        case BR:
        {
            return "Bottom Right";
        }
        case ERR:
        {
            return "ERROR";
        }
        default:
        {
            return "No Idea what was given";
        }
    }
}

void PrintMessageCode(char *data, int size)
{
    if (size == 1)
    {
        switch((char)*data)
        {
            case CREATE_GAME:
            {
                printf("DEBUG sent Code: Create Game\n");
            } break;
            case JOIN_GAME:
            {
                printf("DEBUG sent Code: Join Game\n");
            } break;
            case CREATE_PLAYER:
            {
                printf("DEBUG sent Code: Create Player\n");
            } break;
            case GAMEOVER:
            {
                printf("DEBUG sent Code: GameOver\n");
            } break;
            case MOVE_OK:
            {
                printf("DEBUG sent Code: Move Ok\n");
            } break;
            case INVALID_MOVE:
            {
                printf("DEBUG sent Code: Invalid Move\n");
            } break;
            /*
            case CREATE_GAME_SUCCESS:
            {
                printf("DEBUG sent Code: Create Game Success\n");
            } break;*/
            case JOIN_GAME_SUCCESS:
            {
                printf("DEBUG sent Code: Join Game Success\n");
            } break;
            case CREATE_PLAYER_SUCCESS:
            {
                printf("DEBUG sent Code: Create Player Success\n");
            } break;
            case YOUR_TURN:
            {
                printf("DEBUG sent Code: Your Turn\n");
            } break;
            case NOT_YOUR_TURN:
            {
                printf("DEBUG sent Code: Not Your Turn\n");
            } break;
            case DISCONNECT:
            {
                printf("DEBUG sent Code: Disconnect\n");
            } break;
            default:
            {
                printf("DEBUG sent Code: Unkown\n");
            }
        }
    }
    else if(size == sizeof(MessageAction))
    {
        MessageAction Result = *(MessageAction *)data;
#if SERVER
        printf("DEBUG Server Sent Action\nPlayer: %llu\nMove: %s\n", Result.PlayerId, PrintGameMoves(Result.Move));
#endif
#if CLIENT
        printf("DEBUG Client Sent Action\nPlayerId: %llu\nMove: %s\n", Result.PlayerId, PrintGameMoves(Result.Move));
#endif
    }
}

int SendMessage(SOCKET socket, char *data, int size)
{
    int Result = 0;
    int SendResult = send(socket, data, size, 0);
    if (SendResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(socket);
        WSACleanup();
        return Result;
    }
#if SLOW
    PrintMessageCode(data, size);
#endif
    Result = 1;
    return Result;
}


// NOTE(mark): not sure if the PackId and UnpackId should be inline or not?
// TODO(mark): test these idea later
id PackId(char code, uint64_t Value)
{
    // NOTE(mark): pack the struct with a uint64_t in to char array
    // to remove padding from struct to shrink the amount of data sent over the network
    id Result = {};
    Result.code = code;
    for (int index = 0;
         index < 8;
         index++)
    {
        Result.id[index] = ((char *)&Value)[index];
    }
    return Result;
}

uint64_t UnpackId(id data)
{
    uint64_t Result;
    Result = *(uint64_t *)data.id;
    return Result;
}

