#ifndef GOMOKU_WIDGET_H
#define GOMOKU_WIDGET_H

#include <QWidget>
#include <QLabel>
class GomokuGame;

class GomokuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GomokuWidget(QWidget *parent = nullptr);
    ~GomokuWidget();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    static const int BOARD_SIZE = 15;
    static const int CELL_SIZE = 30;
    static const int MARGIN = 20;

    GomokuGame *game; // 游戏逻辑对象
    QLabel *statusLabel; // 显示当前状态的标签

    void drawBoard(QPainter &painter);
    void drawStones(QPainter &painter);
    void updateStatus(); // 更新状态显示

private slots:
    void onGameOver(int winner); // 处理游戏结束信号
    void onMoveMade(); // 处理落子成功信号

public slots:
    void startNewGame();
};

#endif // GOMOKU_WIDGET_H