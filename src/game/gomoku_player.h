#ifndef GOMOKU_PLAYER_H
#define GOMOKU_PLAYER_H

class GomokuPlayer
{
public:
    static const int HUMAN = 0;
    static const int AI = 1;

    GomokuPlayer(int type, int color);
    ~GomokuPlayer();

    int getType() const;
    int getColor() const;

private:
    int type;
    int color;
};

#endif // GOMOKU_PLAYER_H