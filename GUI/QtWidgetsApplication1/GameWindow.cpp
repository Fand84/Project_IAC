
#include "GameWindow.h"
#include <QShortcut>
#include <QKeySequence>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QSizePolicy>
#include <QGraphicsDropShadowEffect>
#include <QColor>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QTimer>

GameWindow::GameWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ui.boardArea->setBoard(&game_.getBoard());
    ui.boardArea->setGame(&game_);

    ui.energyWidget->setEnergy(
        game_.getPlayerEnergy(),
        game_.getMaxEnergy()
    );

    auto* paginaJoc = takeCentralWidget();
    auto* pagini = new QStackedWidget(this);

    // 1. Meniul principal
    auto* paginaStart = new QLabel();

    paginaStart->setPixmap(QPixmap(":/images/arena_meniu.png"));
    paginaStart->setScaledContents(true);
    paginaStart->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    auto* layoutStart = new QVBoxLayout(paginaStart);

    // 2. Titlul jocului
    auto* titlu = new QLabel("CASTEL CRUSH");

    titlu->setAlignment(Qt::AlignCenter);

    titlu->setStyleSheet(
        "color: #FFE2A0;"
        "background: transparent;"
        "border: none;"
        "font-family: Georgia;"
        "font-size: 64px;"
        "font-weight: bold;"
    );

    // Umbra titlului
    auto* umbraTitlu = new QGraphicsDropShadowEffect(titlu);

    umbraTitlu->setBlurRadius(18);
    umbraTitlu->setOffset(2, 4);
    umbraTitlu->setColor(QColor(20, 10, 5, 230));

    titlu->setGraphicsEffect(umbraTitlu);

    // 3. Butonul de start
    auto* butonStart = new QPushButton("INCEPE JOCUL");

    butonStart->setFixedSize(360, 105);
    butonStart->setCursor(Qt::PointingHandCursor);

    butonStart->setStyleSheet(
        "QPushButton {"
        "border: none;"
        "border-image: url(:/images/buton_start_medieval.png);"
        "color: #FFE2A0;"
        "font-family: Georgia;"
        "font-size: 23px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "color: white;"
        "}"
    );

    // 4. Pozitionarea elementelor
    layoutStart->addStretch(3);
    layoutStart->addWidget(titlu, 0, Qt::AlignHCenter);
    layoutStart->addSpacing(140);
    layoutStart->addWidget(butonStart, 0, Qt::AlignHCenter);
    layoutStart->addStretch(8);

    // 5. Paginile jocului
    pagini->addWidget(paginaStart);
    pagini->addWidget(paginaJoc);

    setCentralWidget(pagini);

    ui.menuBar->hide();
    ui.mainToolBar->hide();
    ui.statusBar->hide();

    // 6. Trecerea de la meniu la joc
    connect(butonStart, &QPushButton::clicked, this, [this, pagini, paginaStart]() {

        QPixmap captura = paginaStart->grab();

        pagini->setCurrentIndex(1);

        auto* tranzitie = new QLabel(pagini);

        tranzitie->setPixmap(captura);
        tranzitie->setScaledContents(true);
        tranzitie->setGeometry(pagini->rect());
        tranzitie->show();
        tranzitie->raise();

        auto* efect = new QGraphicsOpacityEffect(tranzitie);

        tranzitie->setGraphicsEffect(efect);
        efect->setOpacity(1.0);

        auto* animatie = new QPropertyAnimation(efect, "opacity", tranzitie);

        animatie->setDuration(1500);
        animatie->setStartValue(1.0);
        animatie->setEndValue(0.0);

        connect(animatie, &QPropertyAnimation::finished,
            tranzitie, &QObject::deleteLater);

        animatie->start(QAbstractAnimation::DeleteWhenStopped);
        });

    // Regenerarea energiei si actualizarea soldatilor
    auto* gameTimer = new QTimer(this);

    connect(gameTimer, &QTimer::timeout, this, [this, pagini]() {

        if (pagini->currentIndex() != 1)
            return;

        game_.update(0.05);

        ui.energyWidget->setEnergy(
            game_.getPlayerEnergy(),
            game_.getMaxEnergy()
        );

        ui.boardArea->update();
        });

    gameTimer->start(50);

    // 7. Control temporar pentru testare

    // Tasta D - daune pentru castelul albastru
    auto* damageShortcut = new QShortcut(QKeySequence(Qt::Key_D), this);

    connect(damageShortcut, &QShortcut::activated, this, [this, pagini]() {

        if (pagini->currentIndex() != 1)
            return;

        game_.getBoard().getComputerCastle().takeDamage(100);
        ui.boardArea->update();
        });

    // Tasta A - daune pentru castelul rosu
    auto* redDamageShortcut = new QShortcut(QKeySequence(Qt::Key_A), this);

    connect(redDamageShortcut, &QShortcut::activated, this, [this, pagini]() {

        if (pagini->currentIndex() != 1)
            return;

        game_.getBoard().getPlayerCastle().takeDamage(100);
        ui.boardArea->update();
        });

    // Tasta E - generam un soldat rosu pe banda de sus
    auto* energyShortcut = new QShortcut(QKeySequence(Qt::Key_E), this);

    connect(energyShortcut, &QShortcut::activated, this, [this, pagini]() {

        if (pagini->currentIndex() != 1)
            return;

        if (game_.spawnUnit(
            UnitType::Soldat,
            PlayerSide::Player,
            Lane::Top,
            UnitStats::getDefaultsFor(UnitType::Soldat)
        ))
        {
            ui.energyWidget->setEnergy(
                game_.getPlayerEnergy(),
                game_.getMaxEnergy()
            );

            ui.boardArea->update();
        }
        });

    // Tasta R - resetarea jocului
    auto* resetShortcut = new QShortcut(QKeySequence(Qt::Key_R), this);

    connect(resetShortcut, &QShortcut::activated, this, [this, pagini]() {

        if (pagini->currentIndex() != 1)
            return;

        game_.restart();

        ui.energyWidget->setEnergy(
            game_.getPlayerEnergy(),
            game_.getMaxEnergy()
        );

        ui.boardArea->update();
        });

    setWindowTitle("Castel Crush");
    resize(1100, 650);
}

GameWindow::~GameWindow()
{}
