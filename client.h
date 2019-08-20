#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QObject>
#include <QMutex>
#include <zlib.h>
#include <math.h>

#include "packet.h"
#include "timer.h"
#include "player.h"

class Client : public QObject
{
public:
    Client(QString host, int port, QString name);
    static void send_packet(Packet &packet);
private:
    static QTcpSocket sock;
    QString playerName;
    QString hostname;
    uint port;


    Timer thread_one;

    static bool compression;
    static uint compression_treshold;

    void handshake();
    void login_start();

    QString by_bit(uint8_t b);

    enum state_enum {LOGIN,PLAY};
    state_enum state;

    static QByteArray send_buffer;
    static QMutex mutex;


    void parse_response(QByteArray data);

    //LOGIN
    static const int LOGIN_PACKET_ID_SET_COMPRESSION=0x03;
    static const int LOGIN_PACKET_ID_SUCCESS=0x02;

    //PLAY
    //1.14.1
    static const int PLAY_PACKET_ID_JOIN_GAME=0x25;
    static const int PLAY_PACKET_ID_SERVER_DIFFICULTY=0x0d;
    static const int PLAY_PACKET_ID_PLUGIN_MESSAGE=0x18;
    static const int PLAY_PACKET_ID_PLAYER_ABILITIES=0x31;
    static const int PLAY_PACKET_ID_PLAYER_HELD_ITEM=0x3f;
    static const int PLAY_PACKET_ID_DECLARE_RECEPIES=0x5a;
    static const int PLAY_PACKET_ID_TAGS=0x5b;
    static const int PLAY_PACKET_ID_ENTITY_STATUS=0x1b;
    static const int PLAY_PACKET_ID_PLAYER_POSITION=0x11;
    static const int PLAY_PACKET_ID_PLAYER_POSITION_AND_LOOK=0x35;
    static const int PLAY_PACKET_ID_UNLOCK_RECIPIES=0x36;
    static const int PLAY_PACKET_ID_PLAYER_INFO=0x33;
    static const int PLAY_PACKET_ID_UPDATE_VIEW_POSITION=0x40;
    static const int PLAY_PACKET_ID_UPDATE_LIGHT=0x24;
    static const int PLAY_PACKET_ID_CHUNK_DATA=0x21;
    static const int PLAY_PACKET_ID_SPAWN_MOB=0x03;
    static const int PLAY_PACKET_ID_ENTITY_METADATA=0x43;
    static const int PLAY_PACKET_ID_ENTITY_PROPERTIES=0x58;
    static const int PLAY_PACKET_ID_SPAWN_OBJECT=0x00;
    static const int PLAY_PACKET_ID_ENTITY_VELOCITY=0x45;
    static const int PLAY_PACKET_ID_WORLD_BORDER=0x3d;
    static const int PLAY_PACKET_ID_TIME_UPDATE=0x4e;
    static const int PLAY_PACKET_ID_SPAWN_POSITION=0x4d;
    static const int PLAY_PACKET_ID_PLAYER_ON_GROUND=0x14;
    static const int PLAY_PACKET_ID_UPDATE_HEALTH=0x48;
    static const int PLAY_PACKET_ID_SET_EXPIRIENCE=0x47;
    static const int PLAY_PACKET_ID_KEEP_ALIVE=0x20;
    static const int PLAY_PACKET_ID_ADVANCEMENTS=0x57;
    static const int PLAY_PACKET_ID_DISCONNECT=0x1a;
    static const int PLAY_PACKET_ID_ENTITY_RELATIVE_MOVE=0x28;
    static const int PLAY_PACKET_ID_ENTITY_EQUIPMENT=0x46;
    static const int PLAY_PACKET_ID_ENTITY_LOOK_AND_RELATIVE_MOVE=0x29;
    static const int PLAY_PACKET_ID_ENTITY_HEAD_LOOK=0x3b;
    static const int PLAY_PACKET_ID_ENTITY_TELEPORT=0x56;
    static const int PLAY_PACKET_ID_ENTITY_LOOK=0x2a;
    static const int PLAY_PACKET_ID_BLOCK_CHANGE=0x0b;
    static const int PLAY_PACKET_ID_MULTI_BLOCK_CHANGES=0x0f;
    static const int PLAY_PACKET_ID_SOUND_EFFECT=0x51;
    static const int PLAY_PACKET_ID_DESTROY_ENTITIES=0x37;
    static const int PLAY_PACKET_ID_EFFECT=0x22;
    static const int PLAY_PACKET_ID_CHANGE_GAME_STATE=0x1E;
    static const int PLAY_PACKET_ID_CHAT_MESSAGE=0x0e;
    static const int PLAY_PACKET_ID_SPAWN_PLAYER=0x05;
    static const int PLAY_PACKET_ID_UNLOAD_CHUNK=0x1d;
    static const int PLAY_PACKET_ID_ANIMATION=0x06;
    static const int PLAY_PACKET_ID_SET_PASSENGERS=0x4a;
    static const int PLAY_PACKET_ID_COLLECT_ITEM=0x55;
    static const int PLAY_PACKET_ID_ENTITY_EFFEKT=0x59;
    static const int PLAY_PACKET_ID_UPDATE_BLOCK_ENTITY=0x09;
    static const int PLAY_PACKET_ID_PARTICLE=0x23;
    static const int PLAY_PACKET_ID_SPAWN_GLOBAL_ENTITIES=0x02;
    static const int PLAY_PACKET_ID_SPAWN_EXPIRIENCE_ORB=0x01;
    static const int PLAY_PACKET_ID_PICK_ITEM=0x17;
    static const int PLAY_PACKET_ID_SET_SLOT=0x16;
    static const int PLAY_PACKET_ID_COMBAT_EVENT=0x32;
    static const int PLAY_PACKET_ID_RESPAWN=0x3a;
    static const int PLAY_PACKET_ID_BLOCK_BREAKE_ANIMATION=0x08;
    static const int PLAY_PACKET_ID_REMOVE_ENTITY_EFFEKT=0x38;
    static const int PLAY_PACKET_ID_ATTACH_ENTITY=0x44;





    QByteArray socket_buffer;


private Q_SLOTS:
    void connected();
    void disconnected();
};

#endif // CLIENT_H
