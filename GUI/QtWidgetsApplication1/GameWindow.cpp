
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
#include <QIcon>
#include <QHBoxLayout>

GameWindow::GameWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);


    // Mutam panoul peste arena, fara sa ocupam un rand separat
    ui.verticalLayout->removeWidget(ui.unitPanel);
    ui.unitPanel->setParent(ui.boardArea);

    // Eliminam fundalul maro
    ui.unitPanel->setStyleSheet(
        "QWidget#unitPanel { background: transparent; }"
    );

    ui.unitLayout->setContentsMargins(0, 0, 0, 0);
    ui.unitLayout->setSpacing(0);

    // Pregatim cartonașele
    auto pregatesteCartonas = [](QPushButton* buton, const QString& imagine)
        {
            buton->setText("");
            buton->setFixedSize(90, 140);
            buton->setIcon(QIcon(imagine));
            buton->setIconSize(QSize(90, 140));
            buton->setCursor(Qt::PointingHandCursor);

            buton->setCheckable(true);
            buton->setAutoExclusive(true);

            buton->setStyleSheet(
                "QPushButton { background: transparent; border: none; padding: 0px; }"
                "QPushButton:checked { border: 2px solid #FFE7A0; border-radius: 10px; }"
            );
        };

    pregatesteCartonas(ui.soldatButton,
        ":/images/Cartonase/soldat.png");

    pregatesteCartonas(ui.tancButton,
        ":/images/Cartonase/cavaler.png");

    pregatesteCartonas(ui.arcasButton,
        ":/images/Cartonase/arcas.png");

    pregatesteCartonas(ui.cercetasButton,
        ":/images/Cartonase/cercetas.png");

    pregatesteCartonas(ui.magButton,
        ":/images/Cartonase/mag.png");

    ui.soldatButton->setChecked(true);
    ui.unitPanel->setFixedSize(465, 122);

    // Pozitionam cartonașele in dreapta jos, peste arena
    auto* layoutArena = new QVBoxLayout(ui.boardArea);
    layoutArena->setContentsMargins(0, 0, 10, 8);

    layoutArena->addStretch();

    auto* randCartonase = new QHBoxLayout();
    randCartonase->addStretch();
    randCartonase->addWidget(ui.unitPanel);

    layoutArena->addLayout(randCartonase);


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

        game_.update(0.016);

        ui.energyWidget->setEnergy(
            game_.getPlayerEnergy(),
            game_.getMaxEnergy()
        );

        ui.boardArea->update();
        });

    gameTimer->start(16);


    // Calculatorul trimite automat soldati
    auto* computerTimer = new QTimer(this);

    connect(computerTimer, &QTimer::timeout, this,
        [this, pagini, bandaSus = true]() mutable {

            if (pagini->currentIndex() != 1 || game_.isGameOver())
                return;


            // Limitam numarul soldatilor calculatorului
            int numarAlbastri = 0;

            for (const auto& unit : game_.getUnits())
            {
                if (unit && unit->isAlive() &&
                    unit->getOwner() == PlayerSide::Computer)
                {
                    numarAlbastri++;
                }
            }

            if (numarAlbastri >= 6)
                return;



            int rosiiSus = 0;
            int rosiiJos = 0;

            for (const auto& unit : game_.getUnits())
            {
                if (unit && unit->isAlive() &&
                    unit->getOwner() == PlayerSide::Player)
                {
                    if (unit->getLane() == 0)
                        rosiiSus++;
                    else
                        rosiiJos++;
                }
            }

            Lane banda;

            if (rosiiSus > rosiiJos)
                banda = Lane::Top;
            else if (rosiiJos > rosiiSus)
                banda = Lane::Bottom;
            else
                banda = bandaSus ? Lane::Top : Lane::Bottom;

            UnitStats stats = UnitStats::getDefaultsFor(UnitType::Soldat);
            stats.speed = 500.0;

            game_.spawnUnit(
                UnitType::Soldat,
                PlayerSide::Computer,
                banda,
                stats
            );

            bandaSus = !bandaSus;
            ui.boardArea->update();
        });

    computerTimer->start(5000);


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

        UnitStats stats = UnitStats::getDefaultsFor(UnitType::Soldat);
        stats.speed = 500.0;

        if (game_.spawnUnit(
            UnitType::Soldat,
            PlayerSide::Player,
            Lane::Top,
            stats
        ))
        {
            ui.energyWidget->setEnergy(
                game_.getPlayerEnergy(),
                game_.getMaxEnergy()
            );

            ui.boardArea->update();
        }
        });


    // Tasta J - soldat pe banda de jos
    auto* bottomShortcut = new QShortcut(QKeySequence(Qt::Key_J), this);

    connect(bottomShortcut, &QShortcut::activated, this, [this, pagini]() {

        if (pagini->currentIndex() != 1)
            return;

        UnitStats stats = UnitStats::getDefaultsFor(UnitType::Soldat);
        stats.speed = 100.0;

        if (game_.spawnUnit(
            UnitType::Soldat,
            PlayerSide::Player,
            Lane::Bottom,
            stats))
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
