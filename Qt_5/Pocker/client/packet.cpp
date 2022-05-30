#include "packet.h"
#include <QByteArray>
#include <QString>
#include <string>

QByteArray Packet::package(){
    for(int i = 0; i < dest.size(); i++){
        //arr[i] = dest[i];
        arr.setRawData(dest.toStdString().c_str(), dest.size());
    }

    int temp = int(opcode);
    QString scode = QString::number(temp);

    QString s = dest.leftJustified(DEST_SIZE, '\0') + scode.leftJustified(TYPE_SIZE, '\0') + payload.leftJustified(PAYLOAD_SIZE, '\0');

    arr.append(s);
    return arr;
}

void Packet::unpack(){
    for(int i = 0; i < DEST_SIZE; i++){
        if(arr[i] == '\0') break;
        dest += arr[i];
    }

    QString temp;

    for(int i = DEST_SIZE; i < DEST_SIZE + TYPE_SIZE; i++){
        if(arr[i] == '\0') break;
        temp += arr[i];
    }

    opcode = Opcode(temp.toInt());

    qDebug() << "Begin payload.";
    for(int i = DEST_SIZE + TYPE_SIZE; i < PACKET_SIZE; i++){
        if(arr[i] == '\0') break;
        payload += arr[i];
    }
}

void Packet::clear(){
    dest.clear();
    opcode = INCOMPLETE;
    payload.clear();
}


