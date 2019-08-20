#include <QCoreApplication>
#include <qdebug.h>

#include "client.h"

int main(int argc, char *argv[])
{

    if(argc<4){
        qDebug()<<"Usage: "<<argv[0]<<"[address] [port] [player name]";
        return 0;
    }

    QString host = argv[1];
    QString port = argv[2];
    QString name = argv[3];


    QCoreApplication a(argc, argv);

    bool ok;
    Client client(host,port.toInt(&ok,10),name);

    return a.exec();
}
