#include "GameWindow.h"

GameWindow::GameWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    setCentralWidget(ui.boardArea);

    setWindowTitle("Castel Crush");
    resize(1100, 650);
}

GameWindow::~GameWindow()
{}
