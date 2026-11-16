#include "GameWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    GameWindow window;
    window.show();

    return app.exec();
}
