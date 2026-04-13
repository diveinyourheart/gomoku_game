#include "gomoku_board.h"

#ifndef TEST_ENVIRONMENT
#include <QDebug>
#include <QString>
#elif TEST_ENVIRONMENT
#include <iostream>
#endif

#include <random>
#include <vector>

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

bool GomokuBoard::removeStone(int x, int y)
{
    if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && board[x][y] != EMPTY) {
        board[x][y] = EMPTY;
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

void GomokuBoard::printBoard() const
{
#ifndef TEST_ENVIRONMENT
    for (int i = 0; i < BOARD_SIZE; ++i) {
        QString line;
        for (int j = 0; j < BOARD_SIZE; ++j) {
            line += QString::number(board[i][j]) + " ";
        }
        qDebug().noquote() << line;
    }
#else
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            int stone = board[i][j];
            switch (stone) {
                case EMPTY: std::cout << ".";
                    break;
                case BLACK: std::cout << "X";
                    break;
                case WHITE: std::cout << "O";
                    break;
            }
        }
        std::cout << std::endl;
    }
#endif
}

bool GomokuBoard::generateRandomBoard(int maxStones)
{
    reset();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, BOARD_SIZE - 1);

    int stoneCount = dis(gen) % (maxStones - 5 + 1) + 5;
    if (stoneCount > maxStones) {
        stoneCount = maxStones;
    }

    int placedStones = 0;
    int currentPlayer = BLACK;
    int attempts = 0;
    const int maxAttempts = 1000;

    while (placedStones < stoneCount && attempts < maxAttempts) {
        attempts++;
        int x = dis(gen);
        int y = dis(gen);

        if (board[x][y] != EMPTY) {
            continue;
        }

        board[x][y] = currentPlayer;

        bool wouldWin = checkWinForRandom(x, y, currentPlayer);

        if (wouldWin) {
            board[x][y] = EMPTY;
            continue;
        }

        placedStones++;
        currentPlayer = (currentPlayer == BLACK) ? WHITE : BLACK;
    }

    return placedStones > 0;
}

bool GomokuBoard::checkWinForRandom(int x, int y, int player) const
{
    const int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};

    for (int i = 0; i < 4; ++i) {
        int dx = directions[i][0];
        int dy = directions[i][1];

        int count = 1;
        count += countDirection(x, y, dx, dy, player);
        count += countDirection(x, y, -dx, -dy, player);

        if (count >= 5) {
            return true;
        }
    }

    return false;
}

int GomokuBoard::countDirection(int x, int y, int dx, int dy, int player) const
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

std::vector<std::pair<int, int>> GomokuBoard::getValidMoves() const
{
    std::vector<std::pair<int, int>> validMoves;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == EMPTY) {
                validMoves.push_back(std::make_pair(i, j));
            }
        }
    }
    return validMoves;
}