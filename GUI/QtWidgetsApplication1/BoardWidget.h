#pragma once

#include <QWidget>

class BoardWidget : public QWidget
{
public:
    explicit BoardWidget(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
};