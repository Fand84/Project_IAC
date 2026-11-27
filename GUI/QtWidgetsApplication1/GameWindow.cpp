#include "GameWindow.h"
#include <QShortcut>
#include <QKeySequence>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
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

GameWindow::GameWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // Move the unit panel over the arena
    ui.verticalLayout->removeWidget(ui.unitPanel);
    ui.unitPanel->setParent(ui.boardArea);

    ui.unitPanel->setStyleSheet(
        "QWidget#unitPanel { background: transparent; }"
    );

    ui.unitLayout->setContentsMargins(0, 0, 0, 0);
    ui.unitLayout->setSpacing(0);

    // Configure unit cards
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
                "QPushButton:disabled { opacity: 0.4; }"
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

    // Position unit cards in the bottom-right corner
    auto* layoutArena = new QVBoxLayout(ui.boardArea);
    layoutArena->setContentsMargins(0, 0, 10, 8);

    layoutArena->addStretch();

    auto* randCartonase = new QHBoxLayout();
    randCartonase->addStretch();
    randCartonase->addWidget(ui.unitPanel);

    layoutArena->addLayout(randCartonase);

    // Connect the board widget to the game
    ui.boardArea->setBoard(&game_.getBoard());
    ui.boardArea->setGame(&game_);

    // Spawn the selected unit type
    auto genereazaUnitate = [this](Lane banda)
        {
            if (game_.isGameOver())
                return;

            UnitType tip = UnitType::Soldat;

            if (ui.tancButton->isChecked())
                tip = UnitType::Tanc;
            else if (ui.arcasButton->isChecked())
                tip = UnitType::Arcas;
            else if (ui.cercetasButton->isChecked())
                tip = UnitType::Cercetas;
            else if (ui.magButton->isChecked())
                tip = UnitType::Mag;

            UnitStats stats = UnitStats::getDefaultsFor(tip);

            // Temporary movement speed for gameplay testing
            stats.speed = 500.0;

            if (game_.spawnUnit(tip, PlayerSide::Player, banda, stats))
            {
                ui.energyWidget->setEnergy(
                    game_.getPlayerEnergy(),
                    game_.getMaxEnergy()
                );

                ui.boardArea->update();
            }
        };

    // Spawn units when a lane is clicked
    ui.boardArea->setOnLaneClicked(genereazaUnitate);

    ui.energyWidget->setEnergy(
        game_.getPlayerEnergy(),
        game_.getMaxEnergy()
    );

    // Set up the game pages
    auto* paginaJoc = takeCentralWidget();
    auto* pagini = new QStackedWidget(this);

    // Main menu
    auto* paginaStart = new QLabel();

    paginaStart->setPixmap(QPixmap(":/images/arena_meniu.png"));
    paginaStart->setScaledContents(true);
    paginaStart->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    auto* layoutStart = new QVBoxLayout(paginaStart);

    // Game title
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

    auto* umbraTitlu = new QGraphicsDropShadowEffect(titlu);

    umbraTitlu->setBlurRadius(18);
    umbraTitlu->setOffset(2, 4);
    umbraTitlu->setColor(QColor(20, 10, 5, 230));

    titlu->setGraphicsEffect(umbraTitlu);

    // Start button
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

    // Arrange menu elements
    layoutStart->addStretch(3);
    layoutStart->addWidget(titlu, 0, Qt::AlignHCenter);
    layoutStart->addSpacing(140);
    layoutStart->addWidget(butonStart, 0, Qt::AlignHCenter);
    layoutStart->addStretch(8);

    // Add the menu and gameplay pages
    pagini->addWidget(paginaStart);
    pagini->addWidget(paginaJoc);

    setCentralWidget(pagini);

    ui.menuBar->hide();
    ui.mainToolBar->hide();
    ui.statusBar->hide();

    // Fade transition from menu to gameplay
    connect(butonStart, &QPushButton::clicked, this,
        [this, pagini, paginaStart]()
        {
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

            auto* animatie = new QPropertyAnimation(
                efect, "opacity", tranzitie
            );

            animatie->setDuration(1500);
            animatie->setStartValue(1.0);
            animatie->setEndValue(0.0);

            connect(
                animatie,
                &QPropertyAnimation::finished,
                tranzitie,
                &QObject::deleteLater
            );

            animatie->start(QAbstractAnimation::DeleteWhenStopped);
        });

    // Update gameplay and energy
    auto* gameTimer = new QTimer(this);

    connect(gameTimer, &QTimer::timeout, this, [this, pagini]()
        {
            if (pagini->currentIndex() != 1)
                return;

            game_.update(0.016);

            ui.energyWidget->setEnergy(
                game_.getPlayerEnergy(),
                game_.getMaxEnergy()
            );

            // Enable or disable cards based on available energy
            ui.soldatButton->setEnabled(
                game_.canPlaceUnit(
                    UnitStats::getDefaultsFor(UnitType::Soldat).cost
                )
            );

            ui.tancButton->setEnabled(
                game_.canPlaceUnit(
                    UnitStats::getDefaultsFor(UnitType::Tanc).cost
                )
            );

            ui.arcasButton->setEnabled(
                game_.canPlaceUnit(
                    UnitStats::getDefaultsFor(UnitType::Arcas).cost
                )
            );

            ui.cercetasButton->setEnabled(
                game_.canPlaceUnit(
                    UnitStats::getDefaultsFor(UnitType::Cercetas).cost
                )
            );

            ui.magButton->setEnabled(
                game_.canPlaceUnit(
                    UnitStats::getDefaultsFor(UnitType::Mag).cost
                )
            );

            ui.boardArea->update();
        });

    gameTimer->start(16);

    // Spawn computer-controlled soldiers
    auto* computerTimer = new QTimer(this);

    connect(computerTimer, &QTimer::timeout, this,
        [this, pagini, bandaSus = true]() mutable
        {
            if (pagini->currentIndex() != 1 || game_.isGameOver())
                return;

            // Limit the number of active computer units
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

            // Set computer unit movement speed to 500
            UnitStats stats = UnitStats::getDefaultsFor(UnitType::Soldat);
            stats.speed = 500.0;

            (void)game_.spawnUnit(
                UnitType::Soldat,
                PlayerSide::Computer,
                banda,
                stats
            );

            bandaSus = !bandaSus;
            ui.boardArea->update();
        });

    computerTimer->start(5000);

    // Keyboard shortcuts for gameplay and testing

    // D - damage the blue castle
    auto* damageShortcut = new QShortcut(
        QKeySequence(Qt::Key_D), this
    );

    connect(damageShortcut, &QShortcut::activated, this,
        [this, pagini]()
        {
            if (pagini->currentIndex() != 1)
                return;

            game_.getBoard().getComputerCastle().takeDamage(100);
            ui.boardArea->update();
        });

    // A - damage the red castle
    auto* redDamageShortcut = new QShortcut(
        QKeySequence(Qt::Key_A), this
    );

    connect(redDamageShortcut, &QShortcut::activated, this,
        [this, pagini]()
        {
            if (pagini->currentIndex() != 1)
                return;

            game_.getBoard().getPlayerCastle().takeDamage(100);
            ui.boardArea->update();
        });

    // E - spawn on the top lane
    auto* topShortcut = new QShortcut(
        QKeySequence(Qt::Key_E), this
    );

    connect(topShortcut, &QShortcut::activated, this,
        [genereazaUnitate, pagini]()
        {
            if (pagini->currentIndex() == 1)
                genereazaUnitate(Lane::Top);
        });

    // J - spawn on the bottom lane
    auto* bottomShortcut = new QShortcut(
        QKeySequence(Qt::Key_J), this
    );

    connect(bottomShortcut, &QShortcut::activated, this,
        [genereazaUnitate, pagini]()
        {
            if (pagini->currentIndex() == 1)
                genereazaUnitate(Lane::Bottom);
        });

    // R - restart the game
    auto* resetShortcut = new QShortcut(
        QKeySequence(Qt::Key_R), this
    );

    connect(resetShortcut, &QShortcut::activated, this,
        [this, pagini]()
        {
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
