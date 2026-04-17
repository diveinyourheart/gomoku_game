#ifndef PLAYER_H
#define PLAYER_H

#include <stack>

struct PlayerMove {
    int x;
    int y;
    PlayerMove(int x, int y) : x(x), y(y) {}
    PlayerMove() : x(-1), y(-1) {}
    bool operator==(const PlayerMove& other) const;
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
    void addMove(int x, int y);
    PlayerMove undoMove();
    bool hasMoves() const;
    const std::stack<PlayerMove>& getMoveHistory() const;
    PlayerMove getLastMove() const;

protected:
    int type;
    int color;
    std::stack<PlayerMove> moveHistory;
};

#endif // PLAYER_H