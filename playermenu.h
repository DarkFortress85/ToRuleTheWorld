#ifndef PLAYERMENU_H
#define PLAYERMENU_H

#include <QDialog>
#include "player.h"

namespace Ui {
class CPlayerMenu;
}

class CPlayerMenu : public QDialog
{
    Q_OBJECT

public:
    //De- Konstruktoren
    explicit CPlayerMenu(const int numOfMapCountrys, const CPlayer* player, QWidget *parent = nullptr);
    ~CPlayerMenu(); //1-3-5 Regel
    CPlayerMenu(const CPlayerMenu&)=delete;
    CPlayerMenu& operator=(const CPlayerMenu) = delete;

public slots:
    void quit(); //Für PB "Schließen"

private:
    //Attribute
    Ui::CPlayerMenu *ui;
    const CPlayer* m_player;
};

#endif // PLAYERMENU_H
