#ifndef PACKET_H
#define PACKET_H

#define PACKET_SIZE 256
#define PAYLOAD_SIZE 240
#define DEST_SIZE 8
#define TYPE_SIZE 8

#include <QString>
#include <QByteArray>
#include <QDebug>

class Packet
{
public:
    enum Opcode { INCOMPLETE,
        S2C_GAME_INFO,
        S2C_ADD_PLAYER,
        C2S_CHOSE_SEAT,
        S2C_CARDSDEALT,
        C2S_MESSAGE,
        S2C_MESSAGE,
        S2C_PLAYER_TURN,
        C2S_PLAYER_MOVE,
        S2C_PLAYER_MOVE,
        S2C_BLINDS,
        S2C_DISCONNECT,
        C2S_DISCONNECT,
        S2C_ROUND_COUNTDOWN,
        S2C_REMOVE_PLAYER,
        C2S_MOVE_TYPE,
        C2S_JOIN_GAME,
        S2C_JOIN_GAME
    } opcode;

    Packet() = default;
    Packet(Opcode code):opcode(code){}
    Packet(Opcode code, QString contents):opcode(code), payload(contents){}

    QByteArray package();

    void unpack();
    void clear();

    QString dest;
    QString payload;

    Packet& operator=(QByteArray rhs){this->arr = rhs; return *this; }

    QByteArray arr;
};

inline QByteArray& operator >> (QByteArray& str, Packet &packet){
    packet.arr = str;
    return str;
}

#endif // PACKET_H
