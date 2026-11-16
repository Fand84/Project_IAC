#include "BoardWidget.h"
#include "../../Core/Board.h"
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QTransform>
#include <QString>
#include <QRect>
BoardWidget::BoardWidget(QWidget* parent)
    : QWidget(parent)
{}

void BoardWidget::setBoard(const Board* board)
{
    board_ = board;
    update();
}

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

    if (board_ != nullptr)
    {
        const Castle& rosu = board_->getPlayerCastle();
        const Castle& albastru = board_->getComputerCastle();

        painter.fillRect(QRect(20, 10, 185, 30), QColor(0, 0, 0, 160));
        painter.fillRect(QRect(width() - 205, 10, 185, 30), QColor(0, 0, 0, 160));

        painter.setPen(Qt::white);

        painter.drawText(
            QRect(20, 10, 185, 30),
            Qt::AlignCenter,
            QString("Rosu: %1/%2 HP")
            .arg(rosu.getCurrentHp())
            .arg(rosu.getMaxHp())
        );

        painter.drawText(
            QRect(width() - 205, 10, 185, 30),
            Qt::AlignCenter,
            QString("Albastru: %1/%2 HP")
            .arg(albastru.getCurrentHp())
            .arg(albastru.getMaxHp())
        );

        const int latimeBara = 185;
        const int inaltimeBara = 10;

        // Calculam cat din bara trebuie colorat
        int viataRosu = latimeBara * rosu.getCurrentHp() / rosu.getMaxHp();
        int viataAlbastru = latimeBara * albastru.getCurrentHp() / albastru.getMaxHp();

        // Fundalul barelor
        painter.fillRect(20, 45, latimeBara, inaltimeBara, QColor(40, 40, 40));
        painter.fillRect(width() - 205, 45, latimeBara, inaltimeBara, QColor(40, 40, 40));

        // Partea verde, proportionala cu viata ramasa
        painter.fillRect(20, 45, viataRosu, inaltimeBara, QColor(217, 169, 79));
        painter.fillRect(width() - 205, 45, viataAlbastru, inaltimeBara, QColor(217, 169, 79));
    }


}
