#ifndef PLAYER_H
#define PLAYER_H
#include <QString>


class Player
{
public:
    static QString UUID_STR;
    static int ENTITY_id;
    static int DIMENSION;
    static uint8_t GAMEMODE;
    static uint8_t DIFFICULTY;
    static uint8_t SERVER_DIFFICULTY;
    static uint8_t PLAYER_ABILITIES_FLAGS;
    static uint8_t PLAYER_HELD_ITEM;
    static float PLAYER_ABILITIES_FLYING_SPEED;
    static float PLAYER_ABILITIES_FIELD_OF_WIEV;
    static double x;
    static double y;
    static double z;
    Player();
};

#endif // PLAYER_H
