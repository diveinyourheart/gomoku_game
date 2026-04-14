// gomoku_constants.h
#pragma once

namespace GomokuConst {
    constexpr int BOARD_SIZE = 15;
    constexpr int CELL_SIZE = 40;
    constexpr int MARGIN = 30;
    
    // 游戏模式枚举
    enum class GameMode {
        None,    // 未选择模式（零值）
        Normal,  // 普通对战模式
        AI       // AI对战模式
    };
}