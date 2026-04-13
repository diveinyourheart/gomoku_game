#ifndef GOMOKU_BOARD_H
#define GOMOKU_BOARD_H

class GomokuBoard
{
public:
    static const int BOARD_SIZE = 15;
    static const int EMPTY = 0;
    static const int BLACK = 1;
    static const int WHITE = 2;

    GomokuBoard();
    ~GomokuBoard();

    void reset();
    bool placeStone(int x, int y, int player);
    bool removeStone(int x, int y);
    int getStone(int x, int y) const;
    bool isFull() const;
    bool checkWin(int x, int y, int player) const;
    void printBoard() const;
    bool generateRandomBoard(int maxStones);

private:
    int board[BOARD_SIZE][BOARD_SIZE];
    int checkDirection(int x, int y, int dx, int dy, int player) const;
    bool checkWinForRandom(int x, int y, int player) const;
    int countDirection(int x, int y, int dx, int dy, int player) const;
};

#endif // GOMOKU_BOARD_H
