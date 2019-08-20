#include "timer.h"
#include "client.h"

Timer::Timer()
{

}

void Timer::run()
{
qDebug()<< "THREAD IS STARTED";
int x = 0;
int y = 0;
int z = 0;
Packet chat;
int tp_out=0;
while (true){
        //Player::y-=0.0001;
        Player::z-=1;
        tp_out++;
    int tmp_x = Player::x;
    int tmp_y = Player::y;
    int tmp_z = Player::z;
        Packet GoDown;
        GoDown.write_varint(0x11);
        GoDown.write_double(Player::x);
        GoDown.write_double(Player::y);
        GoDown.write_double(Player::z);
        GoDown.write_bool(false);
        Client::send_packet(GoDown);

        if (tp_out>100){
            tp_out=0;
            chat.clear();
            chat.write_varint(0x03);
            chat.write_string("/tp "+QString::number(tmp_x-1)+" "+QString::number(tmp_y)+" "+QString::number(tmp_z+30));
            Client::send_packet(chat);
        }

        if(tmp_z%25==0)

        if ((x != tmp_x) || (y!=tmp_y) || (z!=tmp_z)){
            tp_out=0;
            msleep(100);

            //glass
        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/fill "+QString::number(tmp_x-3)+" "+QString::number(tmp_y-1)+" "+QString::number(tmp_z-50)+" "+QString::number(tmp_x+3)+" "+QString::number(tmp_y+3)+" "+QString::number(tmp_z-1)+" cyan_stained_glass");
        Client::send_packet(chat);
        msleep(100);
            //clear
        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/fill "+QString::number(tmp_x-2)+" "+QString::number(tmp_y)+" "+QString::number(tmp_z-50)+" "+QString::number(tmp_x+2)+" "+QString::number(tmp_y+2)+" "+QString::number(tmp_z-1)+" air");
        Client::send_packet(chat);
        msleep(100);
            //redstone
        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/fill "+QString::number(tmp_x-1)+" "+QString::number(tmp_y-1)+" "+QString::number(tmp_z-50)+" "+QString::number(tmp_x-1)+" "+QString::number(tmp_y-1)+" "+QString::number(tmp_z-1)+" minecraft:redstone_block");
        Client::send_packet(chat);
        msleep(100);
        //redstone
        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/fill "+QString::number(tmp_x+1)+" "+QString::number(tmp_y-1)+" "+QString::number(tmp_z-50)+" "+QString::number(tmp_x+1)+" "+QString::number(tmp_y-1)+" "+QString::number(tmp_z-1)+" minecraft:redstone_block");
        Client::send_packet(chat);
        msleep(100);
           //powered rail
        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/fill "+QString::number(tmp_x-1)+" "+QString::number(tmp_y)+" "+QString::number(tmp_z-50)+" "+QString::number(tmp_x-1)+" "+QString::number(tmp_y)+" "+QString::number(tmp_z-1)+" minecraft:powered_rail");
        Client::send_packet(chat);
        msleep(100);
        //powered rail
        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/fill "+QString::number(tmp_x+1)+" "+QString::number(tmp_y)+" "+QString::number(tmp_z-50)+" "+QString::number(tmp_x+1)+" "+QString::number(tmp_y)+" "+QString::number(tmp_z-1)+" minecraft:powered_rail");
        Client::send_packet(chat);
        msleep(100);
            //lantern block
        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/fill "+QString::number(tmp_x-2)+" "+QString::number(tmp_y-1)+" "+QString::number(tmp_z-50)+" "+QString::number(tmp_x-2)+" "+QString::number(tmp_y-1)+" "+QString::number(tmp_z-1)+" minecraft:sea_lantern");
        Client::send_packet(chat);
        msleep(100);

        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/fill "+QString::number(tmp_x)+" "+QString::number(tmp_y-1)+" "+QString::number(tmp_z-50)+" "+QString::number(tmp_x)+" "+QString::number(tmp_y-1)+" "+QString::number(tmp_z-1)+" minecraft:sea_lantern");
        Client::send_packet(chat);
        msleep(100);

        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/fill "+QString::number(tmp_x+2)+" "+QString::number(tmp_y-1)+" "+QString::number(tmp_z-50)+" "+QString::number(tmp_x+2)+" "+QString::number(tmp_y-1)+" "+QString::number(tmp_z-1)+" minecraft:sea_lantern");
        Client::send_packet(chat);
        msleep(100);

        //stone
        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/setblock "+QString::number(tmp_x-1)+" "+QString::number(tmp_y-1)+" "+QString::number(tmp_z-1)+" minecraft:stone");
        Client::send_packet(chat);
        msleep(30);
        //redstone
        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/fill "+QString::number(tmp_x+2)+" "+QString::number(tmp_y-1)+" "+QString::number(tmp_z-3)+" "+QString::number(tmp_x+2)+" "+QString::number(tmp_y-1)+" "+QString::number(tmp_z-1)+" minecraft:redstone_block");
        Client::send_packet(chat);
        msleep(100);
        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/fill "+QString::number(tmp_x-1)+" "+QString::number(tmp_y-1)+" "+QString::number(tmp_z-3)+" "+QString::number(tmp_x-2)+" "+QString::number(tmp_y-1)+" "+QString::number(tmp_z-1)+" minecraft:stone");
        Client::send_packet(chat);
        msleep(30);


        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/setblock "+QString::number(tmp_x-2)+" "+QString::number(tmp_y)+" "+QString::number(tmp_z-1)+" minecraft:rail");
        Client::send_packet(chat);
        msleep(30);
        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/setblock "+QString::number(tmp_x-2)+" "+QString::number(tmp_y)+" "+QString::number(tmp_z-2)+" minecraft:rail");
        Client::send_packet(chat);
        msleep(30);
        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/setblock "+QString::number(tmp_x-1)+" "+QString::number(tmp_y)+" "+QString::number(tmp_z-1)+" minecraft:rail[shape=east_west]");
        Client::send_packet(chat);
        msleep(30);
        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/setblock "+QString::number(tmp_x-2)+" "+QString::number(tmp_y)+" "+QString::number(tmp_z-3)+" minecraft:rail");
        Client::send_packet(chat);
        msleep(30);
        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/setblock "+QString::number(tmp_x-1)+" "+QString::number(tmp_y)+" "+QString::number(tmp_z-2)+" minecraft:rail");
        Client::send_packet(chat);
        msleep(30);
        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/setblock "+QString::number(tmp_x-1)+" "+QString::number(tmp_y)+" "+QString::number(tmp_z-3)+" minecraft:rail");
        Client::send_packet(chat);
        msleep(30);



        //redstone
        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/fill "+QString::number(tmp_x+2)+" "+QString::number(tmp_y-1)+" "+QString::number(tmp_z-3)+" "+QString::number(tmp_x+2)+" "+QString::number(tmp_y-1)+" "+QString::number(tmp_z-1)+" minecraft:stone");
        Client::send_packet(chat);
        msleep(100);

        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/setblock "+QString::number(tmp_x+2)+" "+QString::number(tmp_y)+" "+QString::number(tmp_z-2)+" minecraft:rail");
        Client::send_packet(chat);
        msleep(30);
        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/setblock "+QString::number(tmp_x+2)+" "+QString::number(tmp_y)+" "+QString::number(tmp_z-3)+"  minecraft:rail");
        Client::send_packet(chat);
        msleep(30);
        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/setblock "+QString::number(tmp_x+1)+" "+QString::number(tmp_y)+" "+QString::number(tmp_z-3)+"  minecraft:rail");
        Client::send_packet(chat);
        msleep(30);
        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/setblock "+QString::number(tmp_x+2)+" "+QString::number(tmp_y)+" "+QString::number(tmp_z-1)+"  minecraft:rail[shape=east_west]");
        Client::send_packet(chat);
        msleep(30);
        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/setblock "+QString::number(tmp_x+1)+" "+QString::number(tmp_y)+" "+QString::number(tmp_z-1)+" minecraft:rail[shape=east_west]");
        Client::send_packet(chat);
        msleep(30);
        chat.clear();
        chat.write_varint(0x03);
        chat.write_string("/setblock "+QString::number(tmp_x+1)+" "+QString::number(tmp_y)+" "+QString::number(tmp_z-2)+" minecraft:rail");
        Client::send_packet(chat);
        msleep(30);



        x=tmp_x;
        y=tmp_y;
        z=tmp_z;
        }

        msleep(50);
        //qDebug()<< Player::y;
    }
}
