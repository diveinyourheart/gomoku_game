#ifndef GOMOKU_WIDGET_H
#define GOMOKU_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <memory>
class GomokuGame;
class BoardWidget;

class GomokuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GomokuWidget(QWidget *parent = nullptr);
    ~GomokuWidget();

protected:
    void paintEvent(QPaintEvent *event) override;
    // void mousePressEvent(QMouseEvent *event) override;

private:

    std::shared_ptr<GomokuGame> game; // 游戏逻辑对象
    QLabel *statusLabel; // 显示当前状态的标签
    BoardWidget *boardWidget; // 棋盘小部件

    void updateStatus(); // 更新状态显示

private slots:
    void onGameOver(int winner); // 处理游戏结束信号
    void onMoveMade(); // 处理落子成功信号

public slots:
    void startNewGame();
};

#endif // GOMOKU_WIDGET_H