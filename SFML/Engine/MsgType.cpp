#include "MsgType.h"

sf::Packet& operator<<(sf::Packet& packet, MsgType type)
{
    return packet << (sf::Uint16)type;
}

sf::Packet& operator>>(sf::Packet& packet, MsgType& type)
{
    sf::Uint16 temp;
    packet >> temp;
    type = (MsgType)temp;
    return packet;
}
