#ifndef CARD_H
#define CARD_H

#include <QImage>
#include <QLabel>

class Card
{
public:
    Card(int id):id(id), rank(id %13), suit(Suit(id / 13)){}
    Card():id(0), rank(0), suit(Suit::HEART){}

    const int id;
    const int rank;
    //            0      1       2      3
    enum Suit { HEART, DIAMOND, CLUB, SPADE } suit;

    QLabel* label;
};
#endif // CARD_H
