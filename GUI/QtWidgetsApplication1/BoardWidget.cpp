
#include "BoardWidget.h"

#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QTransform>

BoardWidget::BoardWidget(QWidget* parent)
    : QWidget(parent)
{}

void BoardWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    // 1. Desenam fundalul
    QPixmap arena(":/images/arena.png");

    if (arena.isNull())
    {
        painter.fillRect(rect(), QColor("#7DAA5A"));
        painter.drawText(rect(), Qt::AlignCenter,
            "Imaginea arena.png nu a fost gasita!");
        return;
    }

    painter.drawPixmap(rect(), arena);

    // 2. Incarcam castelele normale
    QPixmap imagineRosie(":/images/castel_rosu_normal.png");
    QPixmap imagineAlbastra(":/images/castel_albastru_normal.png");

    if (imagineRosie.isNull() || imagineAlbastra.isNull())
    {
        return;
    }

    // 3. Adaptam dimensiunile la fereastra
    int latimeCastel = width() / 5;
    int inaltimeCastel = height() / 2;

    QPixmap castelRosu = imagineRosie.scaled(
        latimeCastel, inaltimeCastel,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
    );

    QPixmap castelAlbastru = imagineAlbastra.scaled(
        latimeCastel, inaltimeCastel,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
    ).transformed(
        QTransform().scale(-1, 1),
        Qt::SmoothTransformation
    );

    // 4. Stabilim pozitiile
    int margine = -width() / 50;
    int liniaSus = height() * 45 / 100;
    int liniaJos = height() * 82 / 100;

    int xRosu = margine;
    int xAlbastru = width() - margine - castelAlbastru.width();

    // 5. Desenam castelele pe ambele linii
    painter.drawPixmap(
        xRosu, liniaSus - castelRosu.height(), castelRosu
    );

    painter.drawPixmap(
        xAlbastru, liniaSus - castelAlbastru.height(), castelAlbastru
    );

    painter.drawPixmap(
        xRosu, liniaJos - castelRosu.height(), castelRosu
    );

    painter.drawPixmap(
        xAlbastru, liniaJos - castelAlbastru.height(), castelAlbastru
    );
}
