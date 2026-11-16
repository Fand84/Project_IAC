#include "GameWindow.h"
#include <QShortcut>
#include <QKeySequence>

GameWindow::GameWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.boardArea->setBoard(&board_);

    // Control temporar pentru testarea daunelor
    auto* damageShortcut = new QShortcut(QKeySequence(Qt::Key_D), this);

    connect(damageShortcut, &QShortcut::activated, this, [this]() {
        board_.getComputerCastle().takeDamage(100);
        ui.boardArea->update();
        });

    setCentralWidget(ui.boardArea);

    setWindowTitle("Castel Crush");
    resize(1100, 650);

    // Resetarea tablei pentru testare
    auto* resetShortcut = new QShortcut(QKeySequence(Qt::Key_R), this);

    connect(resetShortcut, &QShortcut::activated, this, [this]() {
        board_.reset();
        ui.boardArea->update();
        });
}

GameWindow::~GameWindow()
{}
