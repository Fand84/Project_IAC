#include "GameWindow.h"
#include <QShortcut>
#include <QKeySequence>
#include <QTimer>

GameWindow::GameWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.boardArea->setBoard(&board_);
    ui.energyWidget->setEnergy(board_.getPlayerEnergy(), board_.getMaxEnergy());

   
    auto* gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, [this]() {
        board_.updateEnergy(0.05);
        ui.energyWidget->setEnergy(board_.getPlayerEnergy(), board_.getMaxEnergy());
        });
    gameTimer->start(50);

    
    auto* damageShortcut = new QShortcut(QKeySequence(Qt::Key_D), this);
    connect(damageShortcut, &QShortcut::activated, this, [this]() {
        board_.getComputerCastle().takeDamage(100);
        ui.boardArea->update();
        });

    
    auto* energyShortcut = new QShortcut(QKeySequence(Qt::Key_E), this);
    connect(energyShortcut, &QShortcut::activated, this, [this]() {
        if (board_.spendPlayerEnergy(3.0)) {
            ui.energyWidget->setEnergy(board_.getPlayerEnergy(), board_.getMaxEnergy());
        }
        });

    setWindowTitle("Castel Crush");
    resize(1100, 650);

    auto* resetShortcut = new QShortcut(QKeySequence(Qt::Key_R), this);
    connect(resetShortcut, &QShortcut::activated, this, [this]() {
        board_.reset();
        ui.energyWidget->setEnergy(board_.getPlayerEnergy(), board_.getMaxEnergy());
        ui.boardArea->update();
        });
}

GameWindow::~GameWindow()
{
}