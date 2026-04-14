# 五子棋游戏 (Gomoku Game)

一个基于Qt框架开发的五子棋游戏，支持双人对战和AI对战两种模式。

## 项目简介

本项目是一个跨平台的五子棋游戏，使用C++和Qt框架开发。游戏提供了友好的图形用户界面，支持双人对战和AI对战两种游戏模式。在AI对战模式中，集成了DeepSeek大语言模型API，实现了智能化的AI对手。

## 功能特性

### 游戏模式
- **双人对战模式**: 两个玩家在同一台电脑上轮流下棋
- **AI对战模式**: 玩家与AI对战，可选择执黑子或白子

### AI特性
- **本地评估函数**: 使用启发式评估函数对棋盘进行评分，包括活三、冲四、活四、连五等棋型识别
- **Minimax算法**: 实现了Minimax算法配合Alpha-Beta剪枝，进行本地决策
- **大语言模型集成**: 集成DeepSeek API，利用大语言模型进行更智能的决策
- **异步网络通信**: 使用独立线程处理网络请求，避免阻塞UI线程

### 用户界面
- **图形化棋盘**: 15x15的标准五子棋棋盘
- **游戏状态显示**: 实时显示当前玩家、游戏状态和回合数
- **悔棋功能**: 支持悔棋操作（仅双人对战模式）
- **游戏控制**: 提供新游戏、返回主菜单等功能

## 项目结构

```
game/
├── CMakeLists.txt          # CMake构建配置文件
├── README.md               # 项目说明文档
├── .env                    # 环境变量配置文件
├── forms/                  # Qt界面文件
│   └── mainwindow.ui
├── src/                    # 源代码目录
│   ├── main.cpp            # 程序入口
│   ├── game/               # 游戏逻辑模块
│   │   ├── game.h/cpp      # 游戏基类
│   │   ├── gomoku_game.h/cpp # 双人对战游戏类
│   │   ├── ai_game.h/cpp   # AI对战游戏类
│   │   ├── gomoku_board.h/cpp # 棋盘类
│   │   ├── player.h/cpp    # 玩家基类
│   │   ├── gomoku_player.h/cpp # 人类玩家类
│   │   └── ai_player.h/cpp # AI玩家类
│   ├── network/            # 网络通信模块
│   │   ├── network_manager.h/cpp # 网络管理器
│   │   ├── network_event.h # 网络事件定义
│   │   ├── httplib.h       # HTTP库
│   │   ├── nlohmann/       # JSON库
│   │   └── cameron314/     # 并发队列库
│   ├── ui/                 # 用户界面模块
│   │   ├── mainwindow.h/cpp # 主窗口
│   │   ├── gomoku_widget.h/cpp # 游戏窗口
│   │   ├── board_widget.h/cpp # 棋盘控件
│   │   └── gomoku_constants.h # 常量定义
│   ├── util/               # 工具模块
│   │   └── env_util.h/cpp  # 环境变量工具
│   └── config/             # 配置模块
│       └── config.h/cpp    # 配置管理
├── resources/              # 资源文件
│   ├── resources.qrc
│   └── icons/
│       └── app_icon.svg
└── tests/                  # 测试代码
    ├── CMakeLists.txt
    └── test_http_client.cpp
```

## 技术栈

- **编程语言**: C++17
- **GUI框架**: Qt 5.15
- **构建工具**: CMake 3.10+
- **网络库**: cpp-httplib
- **JSON库**: nlohmann/json
- **并发库**: moodycamel::ConcurrentQueue
- **加密库**: OpenSSL
- **AI服务**: DeepSeek API

## 构建说明

### 前置要求

- CMake 3.10 或更高版本
- Qt 5.15 或更高版本
- MinGW-w64 或 MSVC 编译器
- OpenSSL 库

### 构建步骤

1. 克隆项目到本地
```bash
git clone <repository-url>
cd game
```

2. 配置环境变量
创建 `.env` 文件并添加DeepSeek API密钥：
```
DEEPSEEK_API_KEY=your_api_key_here
```

3. 创建构建目录
```bash
mkdir build
cd build
```

4. 配置CMake
```bash
cmake ..
```

5. 编译项目
```bash
cmake --build . --config Debug
```

6. 运行程序
```bash
./gomoku_game.exe
```

## 使用说明

### 启动游戏
运行程序后，会显示主菜单界面。

### 选择游戏模式
- 点击"双人对战"按钮进入双人对战模式
- 点击"AI对战"按钮进入AI对战模式，会弹出对话框让你选择执黑子或白子

### 游戏操作
- 在棋盘上点击鼠标左键落子
- 点击"新游戏"按钮重新开始游戏
- 点击"悔棋"按钮撤销上一步（仅双人对战模式）
- 点击"返回主菜单"按钮返回主菜单

### AI对战说明
- 黑子先行，选择执黑子则玩家先手，选择执白子则AI先手
- AI会根据当前棋盘状态进行智能决策
- AI决策过程包括本地评估、Minimax算法和大语言模型分析

## AI算法说明

### 本地评估函数
评估函数考虑以下棋型：
- 连五 (FIVE): 100000分
- 活四 (OPEN_FOUR): 10000分
- 冲四 (FOUR): 5000分
- 活三 (OPEN_THREE): 1000分
- 眠三 (THREE): 200分
- 双活二 (TWO): 50分
- 单活二 (ONE): 10分

评分公式：`总分 = 进攻分 × 1.0 + 防守分 × 1.2`

### Minimax算法
使用Minimax算法配合Alpha-Beta剪枝，搜索深度默认为3层。

### 大语言模型集成
将本地评估后的候选点发送给DeepSeek API，让AI进行更深入的分析和评分。

## 配置说明

### 环境变量
在 `.env` 文件中配置以下环境变量：
- `DEEPSEEK_API_KEY`: DeepSeek API密钥

### 编译选项
在CMakeLists.txt中可以配置以下选项：
- `CMAKE_CXX_STANDARD`: C++标准版本
- `Qt5_DIR`: Qt安装路径

## 已知问题

- AI对战模式下暂不支持悔棋功能
- 网络请求失败时会使用本地决策作为兜底策略

## 未来计划

- [ ] 添加游戏记录和回放功能
- [ ] 优化AI算法，提高决策效率
- [ ] 添加更多游戏模式（如限时模式）
- [ ] 支持网络对战功能
- [ ] 添加游戏统计和历史记录

## 贡献指南

欢迎提交Issue和Pull Request来帮助改进项目。

## 许可证

本项目采用MIT许可证。

## 联系方式

如有问题或建议，请通过Issue联系项目维护者。
