#ifndef PACKET_H
#define PACKET_H

#include <QString>
#include <QTcpSocket>

class Packet
{
public:
    Packet();
    Packet(QByteArray &data,int &cursor,bool &truncated_packet, bool compression);

    void write_varint(int value);
    void write_string(QString str);
    void write_ushort(uint16_t value);
    void write_long(int64_t value);
    void write_double(double d);
    void write_bool(bool value);

    int read_varint();
    int read_int();
    int64_t read_long();
    uint8_t read_ubyte();
    QString read_string();
    float read_float();
    double read_double();


    static int read_varint(QByteArray &data, int &cursor);
    static int read_int(QByteArray &data, int &cursor);
    static int64_t read_long(QByteArray &data, int &cursor);
    static uint8_t read_ubyte(QByteArray &data, int &cursor);
    static QString read_string(QByteArray &data, int &cursor);
    static float read_float(QByteArray &data, int &cursor);
    static double read_double(QByteArray &data, int &cursor);

    QByteArray get_packet(bool compression, uint compression_treshold);
    void clear();
    void resetCursor();
    void append(QByteArray data);
    QByteArray get_data();

private:
    QByteArray data;
    int cursor;
};

#endif // PACKET_H
