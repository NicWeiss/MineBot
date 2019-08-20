#include "packet.h"
#include <QTextCodec>
#include <zlib.h>

Packet::Packet()
{
    cursor=0;
}

Packet::Packet(QByteArray &input, int &cursor, bool &truncated_packet, bool compression)
{
    this->cursor=0;
    uint packet_length;
    truncated_packet=false;
    int old_cursor = cursor;
    if(!compression){
        packet_length = read_varint(input, cursor);
        if(cursor+packet_length>input.length()){
            truncated_packet=true;
            cursor=old_cursor;
            return;
        }
        data = input.mid(cursor,packet_length);
        cursor+=packet_length;
    }else{
        packet_length = read_varint(input,cursor);
        int tmp_cursor=cursor;
        int data_length = read_varint(input,cursor);
        packet_length -= cursor - tmp_cursor;
        if(cursor+packet_length>input.length()){
            truncated_packet=true;
            cursor=old_cursor;
            return;
        }
        if(data_length==0){
            data = input.mid(cursor,packet_length);
            cursor+=packet_length;
        }else{
            //qDebug()<<"compressed_packet packet_length="<<packet_length<<" data_length="<<data_length;
            Bytef *dest;
            uLongf destLen = data_length;
            QByteArray src = input.mid(cursor,packet_length);
            cursor+=packet_length;
            dest = (Bytef*)malloc(destLen);
            uncompress(dest,&destLen,(const Bytef*)src.data(),src.length());
            data = QByteArray((const char*)dest,destLen);
            //qDebug()<<out;
            free(dest);
            //sock.close();
        }
    }
}

void Packet::write_varint(int value)
{
    if(value==0){
        data.append((char)0);
        return;
    }
    int v=value;
    uint8_t byte;
    while(v>0){
        byte=0;
        byte = v & 0b1111111;
        v>>=7;
        if(v>0)
            byte|=0b10000000;
        data.push_back(byte);
    }
}

void Packet::write_string(QString str)
{

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QByteArray t = codec->fromUnicode(str);
    write_varint(t.length());
    data.append(t);

    return;
}

void Packet::write_ushort(uint16_t value)
{
    data.append(value&0xFF);
    data.append((value>>8)&0xFF);
    return;
}

void Packet::write_long(int64_t value)
{
    data.append(value&0xFFL);
    data.append((value>>8)&0xFF);
    data.append((value>>16)&0xFF);
    data.append((value>>24)&0xFF);

    data.append((value>>32)&0xFF);
    data.append((value>>40)&0xFF);
    data.append((value>>48)&0xFF);
    data.append((value>>56)&0xFF);

}

void Packet::write_double(double d)
{
    union{
        double d;
        uint64_t i;
    } un;

    un.d = d;
    uint64_t value = un.i;

    data.append((value>>56)&0xFFL);
    data.append((value>>48)&0xFFL);
    data.append((value>>40)&0xFFL);
    data.append((value>>32)&0xFFL);

    data.append((value>>24)&0xFFL);
    data.append((value>>16)&0xFFL);
    data.append((value>>8)&0xFFL);
    data.append((value>>0)&0xFFL);

}

void Packet::write_bool(bool value)
{
    if (value){
        data.append(value&0x01);
    }else{
        data.append(value&0x00);
    }
}

int Packet::read_varint()
{
    return read_varint(data,cursor);
}

int Packet::read_int()
{
    return read_int(data,cursor);
}

int64_t Packet::read_long()
{
    return read_long(data,cursor);
}

uint8_t Packet::read_ubyte()
{
    return read_ubyte(data,cursor);
}

QString Packet::read_string()
{
    return read_string(data,cursor);
}

float Packet::read_float()
{
    return read_float(data,cursor);
}

double Packet::read_double()
{
    return read_double(data,cursor);
}

int Packet::read_varint(QByteArray &data, int &cursor)
{
    int out=0;
    u_int8_t byte;
    bool next_byte;
    int counter=0;
    do{
        next_byte=false;
        byte=data[cursor];
        if((byte&0b10000000)>0)
            next_byte=true;
        out|=((byte&0b1111111)<<7*counter);
        counter++;
        cursor++;
        if(counter>5){
            out=0;
            qDebug()<<"warning! varint can't be > 5 bytes";
            break;
        }
    }while(next_byte);

    return out;
}

int Packet::read_int(QByteArray &data, int &cursor)
{
    int out=0;
    out |= data.at(cursor)&0xFF;
    out |= (data.at(cursor+1)&0xFF)<<8;
    out |= (data.at(cursor+2)&0xFF)<<16;
    out |= (data.at(cursor+3)&0xFF)<<24;
    cursor+=4;
    return out;
}

int64_t Packet::read_long(QByteArray &data, int &cursor)
{
    int64_t out=0;
    out |= (data.at(cursor+0)&0xFFL)<<0;
    out |= (data.at(cursor+1)&0xFFL)<<8;
    out |= (data.at(cursor+2)&0xFFL)<<16;
    out |= (data.at(cursor+3)&0xFFL)<<24;

    out |= (data.at(cursor+4)&0xFFL)<<32;
    out |= (data.at(cursor+5)&0xFFL)<<40;
    out |= (data.at(cursor+6)&0xFFL)<<48;
    out |= (data.at(cursor+7)&0xFFL)<<56;

    cursor+=8;
    return out;
}

uint8_t Packet::read_ubyte(QByteArray &data, int &cursor)
{
   uint8_t out=0;
   out=data.at(cursor);
   cursor++;
   return out;
}

QString Packet::read_string(QByteArray &data, int &cursor)
{
    uint len = read_varint(data,cursor);
    QByteArray t = data.mid(cursor,len);
    cursor+=len;
    QString out;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    out = codec->toUnicode(t);
    return out;
}

float Packet::read_float(QByteArray &data, int &cursor)
{
    //uint32_t whole = read_int(data,cursor);

    uint32_t whole=0;
    whole |= (data.at(cursor+3)&0xFF)<<0;
    whole |= (data.at(cursor+2)&0xFF)<<8;
    whole |= (data.at(cursor+1)&0xFF)<<16;
    whole |= (data.at(cursor+0)&0xFF)<<24;
    cursor+=4;

    union {
        float    fl;
        uint32_t dw;
    } f;

    f.dw = whole;
    return f.fl;
}

double Packet::read_double(QByteArray &data, int &cursor)
{
    //uint32_t whole = read_int(data,cursor);

    unsigned long whole=0;
    whole |= (data.at(cursor+7)&0xFFUL)<<0;
    whole |= (data.at(cursor+6)&0xFFUL)<<8;
    whole |= (data.at(cursor+5)&0xFFUL)<<16;
    whole |= (data.at(cursor+4)&0xFFUL)<<24;

    whole |= (data.at(cursor+3)&0xFFUL)<<32;
    whole |= (data.at(cursor+2)&0xFFUL)<<40;
    whole |= (data.at(cursor+1)&0xFFUL)<<48;
    whole |= (data.at(cursor+0)&0xFFUL)<<56;
    cursor+=8;

    union {
        double   fl;
        unsigned long dw;
    } f;

    f.dw = whole;
    return f.fl;
}

QByteArray Packet::get_packet(bool compression, uint compression_treshold)
{
    Packet packet;
    if(!compression){
        packet.write_varint(data.length());
        packet.append(data);
    }else{
        if(data.length()<=compression_treshold){
            packet.write_varint(1+data.length());
            packet.write_varint(0);
            packet.append(data);
        }else{
            qFatal("packet is too large! Need to compress!");
        }
    }


    return packet.get_data();
}

void Packet::clear()
{
    cursor=0;
    data.clear();
}

void Packet::resetCursor()
{
    cursor=0;
}

void Packet::append(QByteArray data)
{
    this->data.append(data);
}

QByteArray Packet::get_data()
{
    return data;
}

