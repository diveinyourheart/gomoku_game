#include "gomoku_board.h"
#include <sstream>

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

std::string GomokuBoard::boardToString() const {
    std::stringstream ss;
    for (int y = 0; y < GomokuBoard::BOARD_SIZE; ++y) {
        for (int x = 0; x < GomokuBoard::BOARD_SIZE; ++x) {
            int stone = getStone(x, y);
            switch (stone) {
                case GomokuBoard::EMPTY: ss << ".";
                    break;
                case GomokuBoard::BLACK: ss << "X";
                    break;
                case GomokuBoard::WHITE: ss << "O";
                    break;
            }
        }
        ss << "\n";
    }
    return ss.str();
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

std::vector<std::pair<int, int>> GomokuBoard::getValidMoves() const
{
    std::vector<std::pair<int, int>> validMoves;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == EMPTY) {
                validMoves.emplace_back(i, j);
            }
        }
    }
    return validMoves;
}

bool GomokuBoard::inBounds(int x, int y) const
{
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

// 方向定义
const std::vector<std::pair<int,int>> directions = {
    {1, 0},   // 横
    {0, 1},   // 竖
    {1, 1},   // 主对角
    {1, -1}   // 副对角
};

// 统计一条线上的连续子
int GomokuBoard::countDirection(int x, int y, int dx, int dy, int player) const
{
    int count = 0;
    int nx = x + dx;
    int ny = y + dy;

    while (inBounds(nx, ny) &&
           getStone(nx, ny) == player) {
        count++;
        nx += dx;
        ny += dy;
    }

    return count;
}

// 判断一端是否为空
bool GomokuBoard::isOpen(int x, int y, int dx, int dy) const
{
    return inBounds(x + dx, y + dy) &&
           getStone(x + dx, y + dy) == EMPTY;
}

// 单方向评分
int GomokuBoard::evaluateDirection(int x, int y, int dx, int dy, int player) const
{
    int left = countDirection(x, y, -dx, -dy, player);
    int right = countDirection(x, y, dx, dy, player);

    int total = left + right + 1;

    bool openLeft = isOpen(x - left * dx, y - left * dy, -dx, -dy);
    bool openRight = isOpen(x + right * dx, y + right * dy, dx, dy);

    int openEnds = openLeft + openRight;

    // 核心判断
    if (total >= 5) return FIVE;

    if (total == 4) {
        if (openEnds == 2) return OPEN_FOUR; // 活四
        if (openEnds == 1) return FOUR;      // 冲四
    }

    if (total == 3) {
        if (openEnds == 2) return OPEN_THREE; // 活三
        if (openEnds == 1) return THREE;      // 眠三
    }

    if (total == 2) {
        return TWO;
    }

    return ONE;
}

// 总评分函数
int GomokuBoard::evaluateMove(int x, int y, int player) const
{
    if (getStone(x, y) != EMPTY) {
        return -1;
    }

    int score = 0;

    // 模拟落子
    const_cast<GomokuBoard*>(this)->placeStone(x, y, player);

    for (auto [dx, dy] : directions) {
        score += evaluateDirection(x, y, dx, dy, player);
    }

    // 撤销
    const_cast<GomokuBoard*>(this)->removeStone(x, y);

    // 防守权重
    int opponent = (player == BLACK)
                   ? WHITE
                   : BLACK;

    int defenseScore = 0;

    const_cast<GomokuBoard*>(this)->placeStone(x, y, opponent);

    for (auto [dx, dy] : directions) {
        defenseScore += evaluateDirection(x, y, dx, dy, opponent);
    }

    const_cast<GomokuBoard*>(this)->removeStone(x, y);

    // 防守权重更高
    return score + static_cast<int>(defenseScore * 1.2);
}