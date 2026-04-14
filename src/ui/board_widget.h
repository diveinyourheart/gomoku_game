#ifndef BOARD_WIDGET_H
#define BOARD_WIDGET_H

#include <QWidget>
#include <memory>
class Game;

class BoardWidget : public QWidget {
    Q_OBJECT
public:
    explicit BoardWidget(std::shared_ptr<Game> game, QWidget* parent = nullptr);
    void setGame(std::shared_ptr<Game> game);

protected:
    void paintEvent(QPaintEvent*) override;

    void mousePressEvent(QMouseEvent* event) override;

private:
    std::shared_ptr<Game> game;

    void drawBoard(QPainter& painter);
    void drawStones(QPainter& painter);

private slots:
    void onMoveMade(); // 处理落子成功信号
};

#endif // BOARD_WIDGET_H
