#ifndef PLAYER_H
#define PLAYER_H

#include <stack>

struct PlayerMove {
    int x;
    int y;
    PlayerMove(int x, int y) : x(x), y(y) {}
    PlayerMove() : x(-1), y(-1) {}
};

class Player
{
public:
    static const int HUMAN = 0;
    static const int AI = 1;

    Player(int type, int color);
    virtual ~Player();

    int getType() const;
    int getColor() const;
    virtual void addMove(int x, int y);

private:
    int type;
    int color;
};

#endif // PLAYER_H