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
#include <QDateTime>
#include <QMouseEvent>

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

// Main rendering function
void BoardWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // Draw the arena background
    QPixmap arena(":/images/arena.png");

    if (arena.isNull())
    {
        painter.fillRect(rect(), QColor("#7DAA5A"));
        painter.drawText(rect(), Qt::AlignCenter,
            "Imaginea arena.png nu a fost gasita!");
        return;
    }

    painter.drawPixmap(rect(), arena);

    // Select castle images based on HP
    auto stareCastel = [](const Castle& castel) -> QString
        {
            if (castel.getCurrentHp() <= 0)
                return "distrus";

            if (castel.getCurrentHp() <= castel.getMaxHp() / 2)
                return "grav_deteriorat";

            if (castel.getCurrentHp() <= castel.getMaxHp() * 3 / 4)
                return "deteriorat";

            return "normal";
        };

    QString stareRosu = "normal";
    QString stareAlbastru = "normal";

    if (board_ != nullptr)
    {
        stareRosu = stareCastel(board_->getPlayerCastle());
        stareAlbastru = stareCastel(board_->getComputerCastle());
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

    // Calculate castle dimensions
    int latimeCastel = width() / 5;
    int inaltimeCastel = height() / 2;

    QSize dimensiuneRosie = imagineRosie.size();
    dimensiuneRosie.scale(
        latimeCastel, inaltimeCastel, Qt::KeepAspectRatio
    );

    QSize dimensiuneAlbastra = imagineAlbastra.size();
    dimensiuneAlbastra.scale(
        latimeCastel, inaltimeCastel, Qt::KeepAspectRatio
    );

    // Calculate positions on the board
    int margine = -width() / 50;
    int liniaSus = height() * 45 / 100;
    int liniaJos = height() * 82 / 100;

    int xRosu = margine;
    int xAlbastru = width() - margine - dimensiuneAlbastra.width();

    // Draw castles on both lanes
    for (int i = 0; i < 2; i++)
    {
        int linie = (i == 0) ? liniaSus : liniaJos;

        painter.drawPixmap(
            QRect(xRosu, linie - dimensiuneRosie.height(),
                dimensiuneRosie.width(), dimensiuneRosie.height()),
            imagineRosie
        );

        painter.drawPixmap(
            QRect(xAlbastru, linie - dimensiuneAlbastra.height(),
                dimensiuneAlbastra.width(), dimensiuneAlbastra.height()),
            imagineAlbastraOglindita
        );
    }

    // Draw units and health indicators
    deseneazaUnitati(
        painter,
        xRosu + dimensiuneRosie.width() - 20,
        xAlbastru + 20,
        liniaSus,
        liniaJos
    );

    deseneazaViata(painter);
}

// Render units and animations
void BoardWidget::deseneazaUnitati(
    QPainter& painter, int inceput, int sfarsit,
    int liniaSus, int liniaJos) const
{
    if (game_ == nullptr)
        return;

    double lungimeBanda = game_->getBoard().getLaneLength();

    if (lungimeBanda <= 0)
        return;

    // Load sprites only once
    static const QPixmap soldatRosu(":/images/Personaje/soldat.png");
    static const QPixmap soldatAlbastru(":/images/Personaje/soldat_albastru.png");

    static const QPixmap cavalerRosu(":/images/Personaje/cavaler.png");
    static const QPixmap cavalerAlbastru(":/images/Personaje/cavaler_albastru.png");

    static const QPixmap arcasRosu(":/images/Personaje/arcas.png");
    static const QPixmap arcasAlbastru(":/images/Personaje/arcas_albastru.png");

    static const QPixmap cercetasRosu(":/images/Personaje/cercetas.png");
    static const QPixmap cercetasAlbastru(":/images/Personaje/cercetas_albastru.png");

    static const QPixmap magRosu(":/images/Personaje/mag.png");
    static const QPixmap magAlbastru(":/images/Personaje/mag_albastru.png");

    for (const auto& unit : game_->getUnits())
    {
        if (!unit || !unit->isAlive())
            continue;

        double progres = unit->getPositionX() / lungimeBanda;

        int x = inceput +
            static_cast<int>((sfarsit - inceput) * progres);

        int y = (unit->getLane() == 0)
            ? liniaSus - 12
            : liniaJos - 12;

        bool esteRosu = unit->getOwner() == PlayerSide::Player;

        // Select the unit sprite
        const QPixmap* imagine = esteRosu ? &soldatRosu : &soldatAlbastru;

        switch (unit->getType())
        {
        case UnitType::Soldat:
            break;

        case UnitType::Tanc:
            imagine = esteRosu ? &cavalerRosu : &cavalerAlbastru;
            break;

        case UnitType::Arcas:
            imagine = esteRosu ? &arcasRosu : &arcasAlbastru;
            break;

        case UnitType::Cercetas:
            imagine = esteRosu ? &cercetasRosu : &cercetasAlbastru;
            break;

        case UnitType::Mag:
            imagine = esteRosu ? &magRosu : &magAlbastru;
            break;
        }

        if (imagine->isNull())
        {
            painter.setPen(Qt::black);
            painter.setBrush(esteRosu
                ? QColor(190, 30, 30)
                : QColor(30, 90, 210));

            painter.drawEllipse(QPoint(x, y), 12, 12);
            continue;
        }

        // Each sprite sheet has 4 columns and 2 rows
        int latimeCadru = imagine->width() / 4;
        int inaltimeCadru = imagine->height() / 2;

        double distanta = esteRosu
            ? unit->getPositionX()
            : lungimeBanda - unit->getPositionX();

        bool ajunsLaCastel = esteRosu
            ? unit->getPositionX() >= lungimeBanda
            : unit->getPositionX() <= 0.0;

        // Check for enemies within attack range
        bool inAtac = false;

        for (const auto& adversar : game_->getUnits())
        {
            if (!adversar || !adversar->isAlive())
                continue;

            if (adversar->getOwner() == unit->getOwner() ||
                adversar->getLane() != unit->getLane())
                continue;

            double diferenta =
                adversar->getPositionX() - unit->getPositionX();

            bool adversarInRaza = esteRosu
                ? diferenta >= 0 && diferenta <= unit->getRange()
                : diferenta <= 0 && -diferenta <= unit->getRange();

            if (adversarInRaza)
            {
                inAtac = true;
                break;
            }
        }

        // Select the walking or attack frame
        int cadru = ajunsLaCastel
            ? 0
            : static_cast<int>(distanta / 9.0) % 4;

        int rand = 0;

        if (inAtac)
        {
            rand = 1;
            cadru = static_cast<int>(
                (QDateTime::currentMSecsSinceEpoch() / 160) % 4
                );
        }

        QRect sursa(
            cadru * latimeCadru,
            rand * inaltimeCadru,
            latimeCadru,
            inaltimeCadru
        );

        // Set unit dimensions
        int latime = 70;
        int inaltime = 96;

        switch (unit->getType())
        {
        case UnitType::Soldat:
            if (esteRosu)
            {
                latime = 64;
                inaltime = 88;
            }
            break;

        case UnitType::Tanc:
            latime = 80;
            inaltime = 108;
            break;

        case UnitType::Arcas:
            latime = 60;
            inaltime = 85;
            break;

        case UnitType::Cercetas:
            latime = 58;
            inaltime = 83;
            break;

        case UnitType::Mag:
            latime = 65;
            inaltime = 95;
            break;
        }

        QRect destinatie(
            x - latime / 2,
            y + 12 - inaltime,
            latime,
            inaltime
        );

        painter.drawPixmap(destinatie, *imagine, sursa);
    }
}

// Render castle HP bars and match result
void BoardWidget::deseneazaViata(QPainter& painter) const
{
    if (board_ == nullptr)
        return;

    const Castle& rosu = board_->getPlayerCastle();
    const Castle& albastru = board_->getComputerCastle();

    //Reuse the same drawing logic for both castles
    auto deseneazaBara = [&painter](
        const Castle& castel, const QString& nume, int x)
        {
            int hp = castel.getCurrentHp();
            int maxHp = castel.getMaxHp();

            if (maxHp <= 0)
                return;

            painter.fillRect(
                QRect(x, 10, 185, 30),
                QColor(0, 0, 0, 160)
            );

            painter.setPen(Qt::white);
            painter.drawText(
                QRect(x, 10, 185, 30),
                Qt::AlignCenter,
                QString("%1: %2/%3 HP")
                .arg(nume).arg(hp).arg(maxHp)
            );

            QColor culoare(80, 190, 90);

            if (hp <= maxHp / 2)
                culoare = QColor(220, 60, 60);
            else if (hp <= maxHp * 3 / 4)
                culoare = QColor(240, 190, 60);

            painter.fillRect(
                x, 45, 185, 10,
                QColor(40, 40, 40)
            );

            painter.fillRect(
                x, 45, 185 * hp / maxHp, 10, culoare
            );
        };

    deseneazaBara(rosu, "Rosu", 20);
    deseneazaBara(albastru, "Albastru", width() - 205);

    QString mesaj;

    if (rosu.getCurrentHp() <= 0 &&
        albastru.getCurrentHp() <= 0)
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

// Handle lane selection by mouse click
void BoardWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton &&
        game_ != nullptr && !game_->isGameOver())
    {
        if (event->pos().y() > height() - 140)
        {
            QWidget::mousePressEvent(event);
            return;
        }

        const Lane selectedLane =
            (event->pos().y() < height() / 2)
            ? Lane::Top
            : Lane::Bottom;

        if (onLaneClicked_)
            onLaneClicked_(selectedLane);
    }

    QWidget::mousePressEvent(event);
}
