#include "gomoku_board.h"

GomokuBoard::GomokuBoard()
{
    reset();
}

GomokuBoard::~GomokuBoard()
{
}

void GomokuBoard::reset()
{
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j] = EMPTY;
        }
    }
}

bool GomokuBoard::placeStone(int x, int y, int player)
{
    if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && board[x][y] == EMPTY) {
        board[x][y] = player;
        return true;
    }
    return false;
}

int GomokuBoard::getStone(int x, int y) const
{
    if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
        return board[x][y];
    }
    return -1;
}

bool GomokuBoard::isFull() const
{
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == EMPTY) {
                return false;
            }
        }
    }
    return true;
}

bool GomokuBoard::checkWin(int x, int y, int player) const
{
    // 检查横向
    int count = 1 + checkDirection(x, y, 1, 0, player) + checkDirection(x, y, -1, 0, player);
    if (count >= 5) return true;

    // 检查纵向
    count = 1 + checkDirection(x, y, 0, 1, player) + checkDirection(x, y, 0, -1, player);
    if (count >= 5) return true;

    // 检查左上到右下
    count = 1 + checkDirection(x, y, 1, 1, player) + checkDirection(x, y, -1, -1, player);
    if (count >= 5) return true;

    // 检查右上到左下
    count = 1 + checkDirection(x, y, 1, -1, player) + checkDirection(x, y, -1, 1, player);
    if (count >= 5) return true;

    return false;
}

int GomokuBoard::checkDirection(int x, int y, int dx, int dy, int player) const
{
    int count = 0;
    int nx = x + dx;
    int ny = y + dy;

    while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && board[nx][ny] == player) {
        count++;
        nx += dx;
        ny += dy;
    }

    return count;
}