/* date = August 9th 2021 9:37 pm */

#ifndef _SHARED_H
#define _SHARED_H

#endif //_SHARED_H

// NOTE(mark): note sure if we need NOT_YOUR_TURN
// NOTE(mark): do not allow more then 255 codes. so it fits in a char

// NOTE(mark): remove code that i am unsure about.
// CREATE_GAME_SUCCESS,
enum MessageCodes {
    CREATE_GAME, JOIN_GAME, CREATE_PLAYER, GAMEOVER, MOVE_OK, INVALID_MOVE,  JOIN_GAME_SUCCESS,
    CREATE_PLAYER_SUCCESS, YOUR_TURN, NOT_YOUR_TURN, DISCONNECT, CREATE_GAME_FAILED_FULL, PLAYERID, 
    GAMEID, JOIN_GAME_FAILED_FULL, CONNECTED, FAILED_TO_SEND_GAME_STATE 
};

struct id {
    char code;
    char id[8];
};



// TODO(Mark): slpit this up in to server to clinet and clinet to server
struct MessageAction
{
    // TODO(mark): pass what game this move  is for
    Game_moves Move;
    uint64_t GameId;
    uint64_t PlayerId;
    
};

struct PartMove
{
    char Code;
    char PlayerSpot;
    uint64_t GameId;
};