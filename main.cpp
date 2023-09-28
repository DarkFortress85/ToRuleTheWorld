#include <QApplication>
#include <QDebug>

#include "game.h"
#include "mainmenu.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CGame* game = new CGame;
    CMainMenu* mainMenu = new CMainMenu(game);
    mainMenu->show();
    mainMenu->exec();
    if(mainMenu->getStartGameStaus())
    {
        game->show();
        delete mainMenu;
        return a.exec();
    }
    else
    {
        delete mainMenu;
    }
    return 0;

}
