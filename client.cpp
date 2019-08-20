#include "client.h"
#include <QTextCodec>

bool Client::compression=false;
uint Client::compression_treshold=0;

QTcpSocket Client::sock;

QByteArray Client::send_buffer;
QMutex Client::mutex;

Client::Client(QString host, int port, QString name)
{
    playerName = name;
    hostname = host;
    this->port=port;
    connect(&sock,&QTcpSocket::connected,this,&Client::connected);
    connect(&sock,&QTcpSocket::disconnected,this,&Client::disconnected);
    sock.connectToHost(host,port);
    qDebug()<<"connecting to"<<host<<port<<"...";
    sock.waitForConnected(30000);
}

void Client::handshake()
{
    qDebug()<<"handshake";
    Packet data;
    data.write_varint(0);
    data.write_varint(480);
    data.write_string(hostname);

    data.write_ushort(port);
    data.write_varint(2);

    send_packet(data);
}

void Client::login_start()
{
    qDebug()<<"login_start";
    Packet data;
    data.write_varint(0);
    data.write_string(playerName);
    send_packet(data);
}


QString Client::by_bit(uint8_t b)
{
   QString out;
    uint8_t t=0b10000000;
    while(t>0){
        out.append(((b&t)>0)?" 1":" 0");
        t>>=1;
    }
    return out;
}

void Client::send_packet(Packet &packet)
{
    //qDebug()<<"send_packet();";
    //packet.send(sock,compression,compression_treshold);
    mutex.lock();
    //qDebug()<<"locked";
    send_buffer.append(packet.get_packet(compression,compression_treshold));
    //qDebug()<<send_buffer;
    mutex.unlock();
    //qDebug()<<"unlocked";
}

void Client::parse_response(QByteArray data)
{
    //qDebug()<<"parse_response";
    int cursor=0;
    //QByteArray packet;
    int packet_id;
    bool truncated_packet;

    socket_buffer.append(data);

    while(cursor<socket_buffer.length()){
        truncated_packet=false;
        Packet packet(socket_buffer,cursor,truncated_packet,compression);
        if(truncated_packet){
            if(cursor>0){
                socket_buffer=socket_buffer.mid(cursor);
            }
            //qDebug()<<"packet was truncated... wating data...";
            //qDebug()<<socket_buffer;
            return;
        }
        packet_id = packet.read_varint();

        if(state==LOGIN){
            switch (packet_id) {
                case LOGIN_PACKET_ID_SET_COMPRESSION:{
                    qDebug()<<"LOGIN: Set Compression";
                    int treshold = packet.read_varint();
                    if(treshold>0){
                        compression_treshold=treshold;
                        compression=true;
                    }else{
                        compression_treshold=0;
                        compression=false;
                    }
                }break;
                case LOGIN_PACKET_ID_SUCCESS:{
                    Player::UUID_STR = packet.read_string();
                    qDebug()<<"LOGIN SUCCESS "<<Player::UUID_STR<<packet.read_string();
                    state=PLAY;
                    continue;
                }break;
                default:
                    qDebug()<<"LOGIN: unknown packet "<<packet_id;
                    qDebug()<<packet.get_data();
                    sock.close();
                break;
            }
        }
        if(state==PLAY){
            switch (packet_id) {

                case PLAY_PACKET_ID_JOIN_GAME:{
                qDebug()<<"PLAY: Join Game";
                Player::ENTITY_id = packet.read_int();
                qDebug()<<"Entity ID: "<<Player::ENTITY_id;
                Player::GAMEMODE = packet.read_ubyte();
                qDebug()<<"GAME MODE: "<<Player::GAMEMODE;
                Player::DIMENSION = packet.read_int();
                qDebug()<<"DIMENSION: "<<Player::DIMENSION;
                Player::DIFFICULTY = packet.read_ubyte();
                qDebug()<<"DIFFICULTY: "<<Player::DIFFICULTY;
                uint8_t max_players = packet.read_ubyte();
                qDebug()<<"max_players: "<<max_players;
                //QString level_type = packet.read_string();
                //qDebug()<<"level_type: "<<level_type;
                //bool dbg_info = packet.read_ubyte();
                //qDebug()<<"debug_info: "<<dbg_info;
                }break;

                case PLAY_PACKET_ID_PLUGIN_MESSAGE:{
                QString channel = packet.read_string();
                QString data = packet.read_string();
                qDebug()<<"PLAY: Plugin message ["<< channel << data <<"]";
                }
                break;

                case PLAY_PACKET_ID_SERVER_DIFFICULTY:{
                Player::SERVER_DIFFICULTY = packet.read_ubyte();
                qDebug()<<"SERVER_DIFFICULTY: "<<Player::SERVER_DIFFICULTY;
                }
                break;

                case PLAY_PACKET_ID_PLAYER_ABILITIES:{
                Player::PLAYER_ABILITIES_FLAGS = packet.read_ubyte();
                qDebug()<<"FLAGS: "<<by_bit(Player::PLAYER_ABILITIES_FLAGS);
                Player::PLAYER_ABILITIES_FLYING_SPEED = packet.read_float();
                qDebug()<<"FLYING SPEED: "<<Player::PLAYER_ABILITIES_FLYING_SPEED;
                 Player::PLAYER_ABILITIES_FIELD_OF_WIEV = packet.read_float();
                 qDebug()<<"FIELD OF WIEV: "<<Player::PLAYER_ABILITIES_FIELD_OF_WIEV;
                }
                break;

                case PLAY_PACKET_ID_PLAYER_HELD_ITEM:{
                Player::PLAYER_HELD_ITEM = packet.read_ubyte();
                qDebug()<<"PLAYER HELD ITEM: "<<Player::PLAYER_HELD_ITEM;
                }
                break;

                case PLAY_PACKET_ID_DECLARE_RECEPIES:{
                //qDebug()<<"DECLARE_RECEPIES: SKIPED";

                }
                break;

                case PLAY_PACKET_ID_TAGS:{
                qDebug()<<"TAGS: READED";
                int identifier = packet.read_varint();
                int count = 0;
                QString name;
                for (int i = 0; i < identifier; i++){
                    name = packet.read_string();
                    //qDebug()<<name;
                    count = packet.read_varint();
                    for (int j = 0; j< count; j++){
                        int entries = packet.read_varint();
                        //qDebug()<<"BLOCK: "<<entries;
                    }
                }
                identifier = packet.read_varint();
                count = 0;
                for (int i = 0; i < identifier; i++){
                    name = packet.read_string();
                    //qDebug()<<name;
                    count = packet.read_varint();
                    for (int j = 0; j< count; j++){
                        int entries = packet.read_varint();
                       // qDebug()<<"ITEM: "<<entries;
                    }
                }
                identifier = packet.read_varint();
                count = 0;
                for (int i = 0; i < identifier; i++){
                    name = packet.read_string();
                    //qDebug()<<name;
                    count = packet.read_varint();
                    for (int j = 0; j< count; j++){
                        int entries = packet.read_varint();
                        //qDebug()<<"FLUID: "<<entries;
                    }
                }
                }
                break;

                case PLAY_PACKET_ID_ENTITY_STATUS:{
                QString array[40];

                array[0] = "Spawns tipped arrow particle effects, if the color is not -1";
                array[1] = "Rabbit, MinecartSpawner";
                array[2] = "Living";
                array[3] = "Snowball, Egg, Living";
                array[4] = "IronGolem, EvocationFangs";
                array[5] = "none";
                array[6] = "AbstractHorse, TameableAnimal";
                array[7] = "AbstractHorse, TameableAnimal";
                array[8] = "Wolf";
                array[9] = "Living";
                array[10] = "Sheep, MinecartTNT";
                array[11] = "IronGolem";
                array[12] = "Villager";
                array[13] = "Villager";
                array[14] = "Villager";
                array[15] = "Witch";
                array[16] = "Zombie Villager";
                array[17] = "Triggers the firework explosion effect ";
                array[18] = "Spawn 'love mode' heart particles";
                array[19] = "Resets the squid's rotation to 0 radians";
                array[20] = "Spawn explosion particle.";
                array[21] = "Plays the guardian attack sound effect from this entity.";
                array[22] = "Enables reduced debug screen information";
                array[23] = "Disables reduced debug screen information";
                array[24] = "Set op permission level to 0";
                array[25] = "Set op permission level to 1";
                array[26] = "Set op permission level to 2";
                array[27] = "Set op permission level to 3";
                array[28] = "Set op permission level to 4";
                array[29] = "Plays the shield block sound";
                array[30] = "Plays the shield break sound";
                array[31] = "If the caught entity is the connected player, then cause them to be pulled toward the caster of the fishing rod.";
                array[32] = "Plays the hit sound, and resets a hit cooldown.";
                array[33] = "Plays the thorns sound in addition to playing the hurt animation and hurt sound";
                array[34] = "Puts away golem's poppy";
                array[35] = "Plays the Totem of Undying animation";
                array[36] = "Plays the hurt animation and drown hurt sound";
                array[37] = "Plays the hurt animation and burn hurt sound";
                array[38] = "Causes several 'happy villager' particles to appear; used when the dolphin has been fed and is locating a structure";

                int entity = packet.read_int();
                int id = packet.read_ubyte();
                //qDebug()<<"Entity " << entity <<" Status: "<< id << " - " << array[id];
                }
                break;

                case PLAY_PACKET_ID_PLAYER_POSITION_AND_LOOK:{
                if (!thread_one.isRunning()){
                    thread_one.start();
                }
                Player::x = packet.read_double();
                Player::y = packet.read_double();
                Player::z = packet.read_double();
                qDebug()<<"POSITION_AND_LOOK";
                qDebug()<<"X: "<< Player::x <<" Y: "<< Player::y <<" Z: "<< Player::z;
                float yaw = packet.read_float();
                float pitch = packet.read_float();
                uint8_t flags = packet.read_ubyte();
                int teleportID = packet.read_varint();
                Packet Teleport_Confirm;
                Teleport_Confirm.write_varint(0x00);
                Teleport_Confirm.write_varint(teleportID);
                send_packet(Teleport_Confirm);
                }

                break;


                case PLAY_PACKET_ID_PLAYER_POSITION:{
                }
                break;


                case PLAY_PACKET_ID_UNLOCK_RECIPIES:{
                //qDebug()<<"UNLOCK_RECIPIES: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_PLAYER_INFO:{
                qDebug()<<"PLAYER INFO: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_UPDATE_VIEW_POSITION:{
                }
                break;

                case PLAY_PACKET_ID_UPDATE_LIGHT:{
                }
                break;

                case PLAY_PACKET_ID_CHUNK_DATA:{
                //qDebug()<<"CHUNK DATA: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_SPAWN_MOB:{
                //qDebug()<<"Spawn Mob: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_ENTITY_METADATA:{
                //qDebug()<<"Entity Metadata: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_ENTITY_PROPERTIES:{
                //qDebug()<<"Entity Properties: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_SPAWN_OBJECT:{
                //qDebug()<<"Spawn objects: SKIPED";
                }
                break;
                case PLAY_PACKET_ID_ENTITY_VELOCITY:{
                //qDebug()<<"Entity Velocity: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_WORLD_BORDER:{
                //qDebug()<<"WORLD_BORDER: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_TIME_UPDATE:{
                //qDebug()<<"TIME_UPDATE: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_PLAYER_ON_GROUND:{
                }
                break;

                case PLAY_PACKET_ID_SPAWN_POSITION:{
                qDebug()<<"SPAWN_POSITION: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_UPDATE_HEALTH:{
                float health = packet.read_float();
                int del=health;
                QString str_health="[";
                QString str_food="[";
                int food = packet.read_varint();
                float food_saturation = packet.read_float();
                for (float i=0; i<health; i=i+1){
                    str_health.append("#");
                }
                if (health != del ){str_health.append(">");}
                for (float i=0; i<20-health; i=i+1){
                    str_health.append("-");
                }
                str_health.append("]");

                for (int i=0; i<food/2; i=i+1){
                    str_food.append("#");
                }
                for (float i=0; i<10-(food/2); i=i+1){
                    str_food.append(".");
                }
                str_food.append("]");

                qDebug()<<"HEALTH: "<< health <<"                                                 FOOD "<< food << " " << food_saturation;
                qDebug()<<str_health << "                                 " <<str_food;


                if (health == 0){
                    int val = 0;
                    Packet respawn;
                    respawn.write_varint(0x03);
                    respawn.write_varint(val);
                    send_packet(respawn);
                }

                }
                break;

                case PLAY_PACKET_ID_SET_EXPIRIENCE:{
                //qDebug()<<"SET_EXPIRIENCE: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_KEEP_ALIVE:{
                    //qDebug()<<"KEEP_ALIVE: CONFIRMED";

                    int64_t val = packet.read_long();

                    Packet KA;
                    KA.write_varint(0x0F);
                    KA.write_long(val);
                    send_packet(KA);
                    //qDebug()<<"pack:"<<packet;
                    //qDebug()<<"KA:"<<KA;
                }
                break;

                case PLAY_PACKET_ID_DISCONNECT:{
                thread_one.terminate();
                qDebug()<<"DISCONNECT"<<packet.read_string();

                }


                case PLAY_PACKET_ID_ADVANCEMENTS:{
                qDebug()<<"ADVANCEMENTS: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_ENTITY_RELATIVE_MOVE:{
                //qDebug()<<"ENTITY_RELATIVE_MOVE: SKIPED";
                }
                break;


                case PLAY_PACKET_ID_ENTITY_EQUIPMENT:{
                //qDebug()<<"Entity Equipment: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_ENTITY_LOOK_AND_RELATIVE_MOVE:{
                //qDebug()<<"ENTITY_LOOK_AND_RELATIVE_MOVE: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_ENTITY_HEAD_LOOK:{
                //qDebug()<<"ENTITY_HEAD_LOOK: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_ENTITY_TELEPORT:{
                //qDebug()<<"ENTITY_TELEPORT: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_ENTITY_LOOK:{
                }
                break;

                case PLAY_PACKET_ID_BLOCK_CHANGE:{
                qDebug()<<"BLOCK_CHANGE: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_MULTI_BLOCK_CHANGES:{
                qDebug()<<"MULTI_BLOCK_CHANGES: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_SOUND_EFFECT:{
                }
                break;

                case PLAY_PACKET_ID_DESTROY_ENTITIES:{
                //qDebug()<<"DESTROY_ENTITIES: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_EFFECT:{
                }
                break;

                case PLAY_PACKET_ID_CHANGE_GAME_STATE:{
                }
                break;

                case PLAY_PACKET_ID_CHAT_MESSAGE:{
                QString message = packet.read_string();
                qDebug()<<"CHAT MESSAGE: "<< message;
                }
                break;

                case PLAY_PACKET_ID_SPAWN_PLAYER:{
                qDebug()<<"SPAWN_PLAYER: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_UNLOAD_CHUNK:{
                }
                break;

                case PLAY_PACKET_ID_ANIMATION:{
                }
                break;

                case PLAY_PACKET_ID_SET_PASSENGERS:{
                }
                break;

                case PLAY_PACKET_ID_COLLECT_ITEM:{
                qDebug()<<"ITEM COLLECTED: ";
                }
                break;


                case PLAY_PACKET_ID_ENTITY_EFFEKT:{
                }
                break;

                case PLAY_PACKET_ID_UPDATE_BLOCK_ENTITY:{

                }
                break;

                case PLAY_PACKET_ID_PARTICLE:{
                }
                break;

                case PLAY_PACKET_ID_SPAWN_GLOBAL_ENTITIES:{
                }
                break;

                case PLAY_PACKET_ID_SPAWN_EXPIRIENCE_ORB:{
                qDebug()<<" Expiriecne orb has bin spawned";
                }
                break;

                case PLAY_PACKET_ID_PICK_ITEM:{
                //qDebug()<<"PICK_ITEM: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_SET_SLOT:{
                //qDebug()<<"SET_SLOT: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_COMBAT_EVENT:{
                }
                break;

                case PLAY_PACKET_ID_RESPAWN:{
                qDebug()<<"RESPAWN: SKIPED";
                }
                break;

                case PLAY_PACKET_ID_BLOCK_BREAKE_ANIMATION:{
                }
                break;

                case PLAY_PACKET_ID_REMOVE_ENTITY_EFFEKT:{
                }
                break;

                case PLAY_PACKET_ID_ATTACH_ENTITY:{
                }
                break;

                default:
                    qDebug()<<"PLAY: unknown packet "<<hex<<packet_id;
                    qDebug()<<packet.get_data();
                    sock.close();
                break;
            }
        }
    }
    socket_buffer.clear();
    /*if(cursor!=data.length()){
        qDebug()<<"error, cursor="<<cursor<<"|data_len="<<data.length();
        qDebug()<<data;
        sock.close();
    }*/
}

void Client::connected()
{
    //qDebug()<<"connected";
    state=LOGIN;
    handshake();
    login_start();
    socket_buffer.clear();
    while(true){
        if(!sock.isOpen())
            break;

        if(send_buffer.length()>0){
            //qDebug()<<send_buffer;
            mutex.lock();
            sock.write(send_buffer);
            sock.waitForBytesWritten(30000);
            send_buffer.clear();
            mutex.unlock();
        }

        sock.waitForReadyRead(30000);
        if(sock.bytesAvailable()>0)
            parse_response(sock.readAll());


    }
    thread_one.terminate();
}

void Client::disconnected()
{
    qDebug()<<"disconnected";
}
