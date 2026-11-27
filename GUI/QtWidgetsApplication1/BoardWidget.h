#pragma once
#include <QWidget>
#include <functional>
#include <utility>
#include "../../Core/Enums.h"

class Board;
class Game;
class QPainter;

class BoardWidget : public QWidget {
public:
    explicit BoardWidget(QWidget* parent = nullptr);
    void setBoard(const Board* board);
    void setGame(const Game* game);
    void setOnLaneClicked(std::function<void(Lane)> callback) { onLaneClicked_ = std::move(callback); }

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    void deseneazaUnitati(QPainter& painter, int inceput, int sfarsit, int liniaSus, int liniaJos) const;
    void deseneazaViata(QPainter& painter) const;

    const Board* board_ = nullptr;
    const Game* game_ = nullptr;
    std::function<void(Lane)> onLaneClicked_;
};
