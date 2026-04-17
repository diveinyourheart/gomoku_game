#include "gomoku_widget.h"

GomokuWidget::GomokuWidget(QWidget *parent) : QWidget(parent)
{
    // 棋盘宽度
    int boardWidth = MARGIN * 2 + CELL_SIZE * (BOARD_SIZE - 1);

    // 创建状态标签
    statusLabel = new QLabel("游戏未开始", this);
    statusLabel->setFixedWidth(boardWidth);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setFont(FontManager::getFont(FontManager::StatusBar));

    // 创建棋盘小部件（暂时传入nullptr，后续在startNewGame中设置）
    boardWidget = new BoardWidget(nullptr, this);

    // 创建垂直布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(statusLabel);
    layout->addWidget(boardWidget);
    layout->setContentsMargins(0, 10, 0, 10);

    statusLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    
    boardWidget->setFixedSize(boardWidth, boardWidth);

    int labelHeight = statusLabel->sizeHint().height();
    int totalHeight = labelHeight + boardWidth + layout->spacing() + 20;

    setFixedSize(boardWidth, totalHeight);

    // 初始更新状态
    updateStatus();
}

GomokuWidget::~GomokuWidget(){}

void GomokuWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

}

void GomokuWidget::onMoveMade()
{
    // 落子成功后更新状态
    updateStatus();
}

void GomokuWidget::updateStatus()
{
    if (game && statusLabel) {
        int currentPlayerIndex = game->getCurrentPlayer();
        int moveCount = game->getMoveCount();
        QString playerColor = currentPlayerIndex == 0 ? "黑方" : "白方";
        auto ai_game = dynamic_cast<AIGame*>(game.get());
        QString playerType = "";
        if(ai_game != nullptr){
            playerType = ( ai_game->getCurrentPlayer() == ai_game -> getAiPlayerIndex() ? "AI" : "你" );
            statusLabel->setText(QString("当前回合：%1（%2），总回合数：%3").arg(playerColor).arg(playerType).arg(moveCount));
        }else if(dynamic_cast<GomokuGame*>(game.get()) != nullptr){
            statusLabel->setText(QString("当前回合：%1，总回合数：%2").arg(playerColor).arg(moveCount));
        }
    } else {
        statusLabel->setText("游戏未开始");
    }
    update();
}

void GomokuWidget::onGameOver(int winner)
{
    if (winner == 0) {
        QMessageBox::information(this, "游戏结束", "平局！");
    } else {
        QString winnerStr = winner == 1 ? "黑方" : "白方";
        QMessageBox::information(this, "游戏结束", winnerStr + "获胜！");
    }
}

void GomokuWidget::startNewGame(GomokuConst::GameMode mode)
{
    // 断开旧的信号槽连接
    if (game) {
        disconnect(game.get(), &Game::gameOver, this, &GomokuWidget::onGameOver);
        disconnect(game.get(), &Game::moveMade, this, &GomokuWidget::onMoveMade);
        disconnect(game.get(), &Game::undoButtonStateChanged, this, &GomokuWidget::onUndoButtonStateChanged);
        game->startNewGame();
        updateStatus();
        if(boardWidget){
            boardWidget->update();
        }
    }

    // 根据游戏模式创建不同的游戏对象
    switch (mode) {
    case GomokuConst::GameMode::Normal:
        // 普通对战模式
        qDebug() << "普通对战模式";
        game = std::make_shared<GomokuGame>();
        break;
    case GomokuConst::GameMode::AI:
        {
            // AI对战模式
            qDebug() << "AI对战模式";
            // 弹出选择颜色的对话框
            QMessageBox msgBox;
            msgBox.setWindowTitle("选择先后手");
            msgBox.setText("您是否选择执黑子（先手）？");
            msgBox.setInformativeText("提示：黑子先手，白子后手。");

            // 使用 Yes 和 No 作为标准按钮
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

            // 强行修改按钮上显示的文本
            msgBox.setButtonText(QMessageBox::Yes, "是，我执黑子");
            msgBox.setButtonText(QMessageBox::No, "否，我执白子");

            // 设置默认焦点在“是”上
            msgBox.setDefaultButton(QMessageBox::Yes);

            // 执行并获取结果
            int result = msgBox.exec();

            // 逻辑判断：点击了 Yes 即为先手（黑子）
            bool humanPlaysBlack = (result == QMessageBox::Yes);
            
            game = std::make_shared<AIGame>(humanPlaysBlack); // 创建AI游戏对象并传入用户选择
            break;
        }
    default:
        qDebug() << "未知游戏模式";
        game = std::make_shared<GomokuGame>();
        break;
    }

    // 连接信号与槽
    connect(game.get(), &Game::gameOver, this, &GomokuWidget::onGameOver);
    connect(game.get(), &Game::moveMade, this, &GomokuWidget::onMoveMade);
    connect(game.get(), &Game::undoButtonStateChanged, this, &GomokuWidget::onUndoButtonStateChanged);

    // 更新棋盘小部件的游戏对象
    boardWidget->setGame(game);

    // 重置游戏
    game->startNewGame();

    // 更新状态并重绘棋盘
    updateStatus();
}

void GomokuWidget::onUndoButtonClicked()
{
    if (game) {
        game->undoMove();
    }
}

void GomokuWidget::onUndoButtonStateChanged(bool enabled)
{
    // 转发信号给MainWindow
    emit undoButtonStateChanged(enabled);
}