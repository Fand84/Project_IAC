#pragma once

#include <QWidget>

class Board;

class BoardWidget : public QWidget
{
public:
    explicit BoardWidget(QWidget* parent = nullptr);

    void setBoard(const Board* board);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    const Board* board_ = nullptr;
};