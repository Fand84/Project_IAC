#pragma once

#include <QWidget>

class Board;

class Game;

class BoardWidget : public QWidget
{
public:
    explicit BoardWidget(QWidget* parent = nullptr);

    void setBoard(const Board* board);

    void setGame(const Game* game);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    const Board* board_ = nullptr;
    const Game* game_ = nullptr;
};