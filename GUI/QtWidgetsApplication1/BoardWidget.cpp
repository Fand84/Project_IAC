
#include "BoardWidget.h"
#include "../../Core/Board.h"
#include "../../Core/Game.h"
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QTransform>
#include <QString>
#include <QRect>
#include <QSize>

BoardWidget::BoardWidget(QWidget* parent)
    : QWidget(parent)
{}

void BoardWidget::setBoard(const Board* board)
{
    board_ = board;
    update();
}

void BoardWidget::setGame(const Game* game)
{
    game_ = game;
    update();
}

void BoardWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

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

    // 2. Alegem imaginile in functie de viata castelelor
    QString stareRosu = "normal";
    QString stareAlbastru = "normal";

    if (board_ != nullptr)
    {
        const Castle& rosu = board_->getPlayerCastle();
        const Castle& albastru = board_->getComputerCastle();

        // Verificam castelul rosu
        if (rosu.getCurrentHp() <= 0)
        {
            stareRosu = "distrus";
        }
        else if (rosu.getCurrentHp() <= rosu.getMaxHp() / 2)
        {
            stareRosu = "grav_deteriorat";
        }
        else if (rosu.getCurrentHp() <= rosu.getMaxHp() * 3 / 4)
        {
            stareRosu = "deteriorat";
        }

        // Verificam castelul albastru
        if (albastru.getCurrentHp() <= 0)
        {
            stareAlbastru = "distrus";
        }
        else if (albastru.getCurrentHp() <= albastru.getMaxHp() / 2)
        {
            stareAlbastru = "grav_deteriorat";
        }
        else if (albastru.getCurrentHp() <= albastru.getMaxHp() * 3 / 4)
        {
            stareAlbastru = "deteriorat";
        }
    }

    QPixmap imagineRosie(
        QString(":/images/castel_rosu_%1.png").arg(stareRosu)
    );

    QPixmap imagineAlbastra(
        QString(":/images/castel_albastru_%1.png").arg(stareAlbastru)
    );

    QPixmap imagineAlbastraOglindita = imagineAlbastra.transformed(
        QTransform().scale(-1, 1),
        Qt::FastTransformation
    );

    // 3. Adaptam dimensiunile la fereastra
    int latimeCastel = width() / 5;
    int inaltimeCastel = height() / 2;

    QSize dimensiuneRosie = imagineRosie.size();
    dimensiuneRosie.scale(
        latimeCastel, inaltimeCastel,
        Qt::KeepAspectRatio
    );

    QSize dimensiuneAlbastra = imagineAlbastra.size();
    dimensiuneAlbastra.scale(
        latimeCastel, inaltimeCastel,
        Qt::KeepAspectRatio
    );

    // 4. Stabilim pozitiile
    int margine = -width() / 50;
    int liniaSus = height() * 45 / 100;
    int liniaJos = height() * 82 / 100;

    int xRosu = margine;
    int xAlbastru = width() - margine - dimensiuneAlbastra.width();

    // 5. Desenam castelele pe ambele linii
    painter.drawPixmap(
        QRect(
            xRosu,
            liniaSus - dimensiuneRosie.height(),
            dimensiuneRosie.width(),
            dimensiuneRosie.height()
        ),
        imagineRosie
    );

    painter.drawPixmap(
        QRect(
            xAlbastru,
            liniaSus - dimensiuneAlbastra.height(),
            dimensiuneAlbastra.width(),
            dimensiuneAlbastra.height()
        ),
        imagineAlbastraOglindita
    );

    painter.drawPixmap(
        QRect(
            xRosu,
            liniaJos - dimensiuneRosie.height(),
            dimensiuneRosie.width(),
            dimensiuneRosie.height()
        ),
        imagineRosie
    );

    painter.drawPixmap(
        QRect(
            xAlbastru,
            liniaJos - dimensiuneAlbastra.height(),
            dimensiuneAlbastra.width(),
            dimensiuneAlbastra.height()
        ),
        imagineAlbastraOglindita
    );

    // 6. Desenam soldatii
    if (game_ != nullptr)
    {
        double lungimeBanda = game_->getBoard().getLaneLength();

        if (lungimeBanda > 0)
        {
            int inceput = xRosu + dimensiuneRosie.width() - 20;
            int sfarsit = xAlbastru + 20;

            for (const auto& unit : game_->getUnits())
            {
                if (!unit || !unit->isAlive())
                    continue;

                double progres = unit->getPositionX() / lungimeBanda;

                int x = inceput +
                    static_cast<int>((sfarsit - inceput) * progres);

                int y;

                if (unit->getLane() == 0)
                    y = liniaSus - 12;
                else
                    y = liniaJos - 12;

                painter.setPen(Qt::black);

                if (unit->getOwner() == PlayerSide::Player)
                    painter.setBrush(QColor(190, 30, 30));
                else
                    painter.setBrush(QColor(30, 90, 210));

                painter.drawEllipse(QPoint(x, y), 12, 12);
            }
        }
    }

    // 7. Afisam viata castelelor
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
        QColor culoareRosu(80, 190, 90);
        QColor culoareAlbastru(80, 190, 90);

        if (rosu.getCurrentHp() <= rosu.getMaxHp() / 2)
        {
            culoareRosu = QColor(220, 60, 60);
        }
        else if (rosu.getCurrentHp() <= rosu.getMaxHp() * 3 / 4)
        {
            culoareRosu = QColor(240, 190, 60);
        }

        if (albastru.getCurrentHp() <= albastru.getMaxHp() / 2)
        {
            culoareAlbastru = QColor(220, 60, 60);
        }
        else if (albastru.getCurrentHp() <= albastru.getMaxHp() * 3 / 4)
        {
            culoareAlbastru = QColor(240, 190, 60);
        }

        painter.fillRect(20, 45, viataRosu, inaltimeBara, culoareRosu);
        painter.fillRect(width() - 205, 45, viataAlbastru, inaltimeBara, culoareAlbastru);

        QString mesaj;

        if (rosu.getCurrentHp() <= 0 && albastru.getCurrentHp() <= 0)
        {
            mesaj = "Remiza!";
        }
        else if (rosu.getCurrentHp() <= 0)
        {
            mesaj = "Albastru a castigat!";
        }
        else if (albastru.getCurrentHp() <= 0)
        {
            mesaj = "Rosu a castigat!";
        }

        if (!mesaj.isEmpty())
        {
            QRect zonaMesaj(width() / 2 - 160, 15, 320, 40);

            painter.fillRect(zonaMesaj, QColor(0, 0, 0, 180));
            painter.setPen(Qt::white);
            painter.drawText(zonaMesaj, Qt::AlignCenter, mesaj);
        }
    }
}
