#include <stdio.h>
//#include <ctype.h>
#include <stdint.h>
#include <string.h>

typedef int bool32;

#define PLAYERONEVALUE 88
#define PLAYERTWOVALUE 79
// 8 total ways to win
#define DIAGONAL 2;
#define VERTICAL_WAYS 3
#define HORIZONTAL_WAYS 3

enum Game_moves { TL, TM, TR,
    ML, MM, MR,
    BL, BM, BR, ERR };


struct GameAction
{
    char player;
    Game_moves move;
};


// TODO(mark): rename to gamestate
struct board_postions
{
    union {
        char Postions[9];
        struct {
            char TopLeft;
            char TopMiddle;
            char TopRight;
            char MiddleLeft;
            char MiddleMiddle;
            char MiddleRight;
            char BottomLeft;
            char BottomMiddle;
            char BottomRight;
        };
    };
    char Winner;
    char CurrentPlayerTurn;
    bool32 GameOver;
    
};

void UpdateBoard(board_postions BoardState)
{
    printf(" %c | %c | %c \n---|---|---\n %c | %c | %c \n---|---|---\n %c | %c | %c \n",
           BoardState.TopLeft,
           BoardState.TopMiddle,
           BoardState.TopRight,
           BoardState.MiddleLeft,
           BoardState.MiddleMiddle,
           BoardState.MiddleRight,
           BoardState.BottomLeft,
           BoardState.BottomMiddle,
           BoardState.BottomRight
           );
}

board_postions CreateBoard(void)
{
    board_postions Result = {};
    for (int PostionIndex = 0;
         PostionIndex < 9;
         PostionIndex++)
    {
        Result.Postions[PostionIndex] = (char)32;
    }
    Result.CurrentPlayerTurn = PLAYERONEVALUE;
    return (Result);
}

void EvalGame(board_postions *board)
{
    char check;
    
    
    for (int horizontal_index = 0;
         horizontal_index < 3 *HORIZONTAL_WAYS;
         horizontal_index += 3)
    {
        check = board->Postions[horizontal_index] &
            board->Postions[horizontal_index + 1] &
            board->Postions[horizontal_index + 2];
        
        if ((check == PLAYERONEVALUE) || (check == PLAYERTWOVALUE))
        {
            board->GameOver = true;
            board->Winner = (int) check;
        }
    }
    
    for (int Vertical_index = 0;
         Vertical_index < VERTICAL_WAYS;
         Vertical_index++)
    {
        check = board->Postions[Vertical_index] &
            board->Postions[Vertical_index + 3] &
            board->Postions[Vertical_index + 6];
        
        if ((check == PLAYERONEVALUE) || (check == PLAYERTWOVALUE))
        {
            board->GameOver = true;
            board->Winner = (int) check;
        }
    }
    
    // diagonal TopLeft to BottomRight
    check = board->Postions[0] &
        board->Postions[4] &
        board->Postions[8];
    
    if ((check == PLAYERONEVALUE) || (check == PLAYERTWOVALUE))
    {
        board->GameOver = true;
        board->Winner = (int) check;
    }
    
    // diagonal TopRight to BottomLeft
    check = board->Postions[2] &
        board->Postions[4] &
        board->Postions[6];
    
    if ((check  == PLAYERONEVALUE) || (check == PLAYERTWOVALUE))
    {
        board->GameOver = true;
        board->Winner = (int) check;
    }
}


inline void EndPlayerTurn(board_postions *board)
{
    if (board->CurrentPlayerTurn == PLAYERONEVALUE)
    {
        board->CurrentPlayerTurn = PLAYERTWOVALUE;
    }
    else
    {
        board->CurrentPlayerTurn = PLAYERONEVALUE;
    }
}


bool32 PlayMove(board_postions *board, GameAction move)
{
    bool32 Result = false;
    if (move.player == board->CurrentPlayerTurn)
    {
        switch (move.move)
        {
            case (TL):
            {
                if(board->TopLeft != (char) 32)
                {
                    break;
                }
                board->TopLeft = (char) move.player;
                Result = true;
                EndPlayerTurn(board);
            } break;
            case (TM):
            {
                if(board->TopMiddle != (char) 32)
                {
                    break;
                }
                board->TopMiddle = (char) move.player;
                Result = true;
                EndPlayerTurn(board);
            } break;
            case (TR):
            {
                if(board->TopRight != (char) 32)
                {
                    break;
                }
                board->TopRight = (char) move.player;
                Result = true;
                EndPlayerTurn(board);
            } break;
            case (ML):
            {
                if(board->MiddleLeft != (char) 32)
                {
                    break;
                }
                board->MiddleLeft = (char) move.player;
                Result = true;
                EndPlayerTurn(board);
            } break;
            case (MM):
            {
                if(board->MiddleMiddle != (char) 32)
                {
                    break;
                }
                board->MiddleMiddle = (char) move.player;
                Result = true;
                EndPlayerTurn(board);
            } break;
            case (MR):
            {
                if(board->MiddleRight != (char) 32)
                {
                    break;
                }
                board->MiddleRight = (char) move.player;
                Result = true;
                EndPlayerTurn(board);
            } break;
            case (BL):
            {
                if(board->BottomLeft != (char) 32)
                {
                    break;
                }
                board->BottomLeft = (char) move.player;
                Result = true;
                EndPlayerTurn(board);
            } break;
            case (BM):
            {
                if(board->BottomMiddle != (char) 32)
                {
                    break;
                }
                board->BottomMiddle = (char) move.player;
                Result = true;
                EndPlayerTurn(board);
            } break;
            case (BR):
            {
                if(board->BottomRight != (char) 32)
                {
                    break;
                }
                board->BottomRight = (char) move.player;
                Result = true;
                EndPlayerTurn(board);
            }break;
            default:
            {
                // NOTE(mark): does nothing but catchs errors
            }break;
        };
    }
    
    
    return (Result);
}

char *GetPlayerName(int player)
{
    switch (player)
    {
        case PLAYERONEVALUE:
        {
            return "Player 1";
        }
        case PLAYERTWOVALUE:
        {
            return "Player 2";
        }
        default:
        {
            return "ERROR: no such player";
        }
    }
}


Game_moves ConvertStrToMove(char *input)
{
    Game_moves Result = ERR;
    if ((strcmp(input, "TL") == 0) || (strcmp(input, "1") == 0))
    {
        Result = TL;
    }
    else if ((strcmp(input, "TM") == 0) || (strcmp(input, "2") == 0))
    {
        Result = TM;
    }
    else if ((strcmp(input, "TR") == 0) || (strcmp(input, "3") == 0))
    {
        Result = TR;
    }
    else if ((strcmp(input, "ML") == 0) || (strcmp(input, "4") == 0))
    {
        Result = ML;
    }
    else if ((strcmp(input, "MM") == 0) || (strcmp(input, "5") == 0))
    {
        Result = MM;
    }
    else if ((strcmp(input, "MR") == 0) || (strcmp(input, "6") == 0))
    {
        Result = MR;
    }
    else if ((strcmp(input, "BL") == 0) || (strcmp(input, "7") == 0))
    {
        Result = BL;
    }
    else if ((strcmp(input, "BM") == 0) || (strcmp(input, "8") == 0))
    {
        Result = BM;
    }
    else if ((strcmp(input, "BR") == 0) || (strcmp(input, "9") == 0))
    {
        Result = BR;
    }
    
    return Result;
}

void ToUpper(char *input, int size)
{
    // TODO(mark): i don't like this function think of something differnt
    for (int index = 0;
         index < size;
         index++)
    {
        input[index] = toupper(input[index]);
    }
}

/*
int main()
{
    // TODO(mark): make the server
    // TODO(mark): add client networking
    // TODO(mark): move some of this code to the server or both?
    // TODO(mark): add multi platform layers
    // TODO(mark): figure out a way to open a terminal when the program is clicked on NOTE(mark): this might work all ready
    // TODO(mark): more throughly check that the code works
    // TODO(mark): handle draws in the EvalGame 
    
    board_postions board = CreateBoard();
    game_state game = {};
    action move = {};
    bool32 LegalMove = false;
    char input[15];
    while(!game.GameOver)
    {
        do {
            printf("Player one turn: ");
            fgets(input, 15, stdin);
            input[strlen(input) - 1] = '\0';
            ToUpper(input, 3);
            move.move = ConvertStrToMove(input);
            move.player = PLAYERONEVALUE;
            LegalMove = PlayMove(&board, move);
            if (!LegalMove)
            {
                printf("Move: %s not allowed or has been played\n", input);
            }
        }while(!LegalMove);
        game = EvalGame(board);
        UpdateBoard(board);
        
        if (!game.GameOver)
        {
            do {
                printf("Player two turn: ");
                fgets(input, 15, stdin);
                input[strlen(input) - 1] = '\0';
                ToUpper(input, 15);
                move.move = ConvertStrToMove(input);
                move.player = PLAYERTWOVALUE;
                LegalMove = PlayMove(&board, move);
                if (!LegalMove)
                {
                    printf("Move: %s not allowed or has been played\n", input);
                }
            }while(!LegalMove);
            game = EvalGame(board);
            UpdateBoard(board);
        }
    }
    
    printf("Winner: %s\n",GetPlayerName(game.Winner));
    
    return 0;
}*/