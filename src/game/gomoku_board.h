#ifndef GOMOKU_BOARD_H
#define GOMOKU_BOARD_H

#include <vector>
#include <utility>

class GomokuBoard
{
public:
    static const int BOARD_SIZE = 15;
    static const int EMPTY = 0;
    static const int BLACK = 1;
    static const int WHITE = 2;

    // 棋型定义
    enum PatternScore {
        FIVE        = 1000000, // 连五
        OPEN_FOUR   = 100000,  // 活四
        FOUR        = 10000,   // 冲四
        OPEN_THREE  = 1000,   // 活三
        THREE       = 200,    // 眠三
        TWO         = 50,
        ONE         = 10
    };

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
    std::vector<std::pair<int, int>> getValidMoves() const;
    bool inBounds(int x, int y) const;
    
    // 启发式评分相关函数
    bool isOpen(int x, int y, int dx, int dy) const;
    int evaluateDirection(int x, int y, int dx, int dy, int player) const;
    int evaluateMove(int x, int y, int player) const;

private:
    int board[BOARD_SIZE][BOARD_SIZE];
    int checkDirection(int x, int y, int dx, int dy, int player) const;
    bool checkWinForRandom(int x, int y, int player) const;
    int countDirection(int x, int y, int dx, int dy, int player) const;
};

#endif // GOMOKU_BOARD_H
