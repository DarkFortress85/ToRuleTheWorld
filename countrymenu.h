#ifndef COUNTRYMENU_H
#define COUNTRYMENU_H

#include <QDialog>
#include "game.h"

namespace Ui {
class CCountryMenu;
}

class CCountryMenu : public QDialog
{
    Q_OBJECT

public:
    explicit CCountryMenu(CGame* game,QWidget *parent = nullptr); //Game* wird übergeben, da Zweck der Klasse darin besteht, das Game auf die gewählte Aktion "einzustellen/vorzubereiten"
    ~CCountryMenu(); //1-3-5 Regel
    CCountryMenu(const CCountryMenu&)=delete;
    CCountryMenu& operator=(const CCountryMenu) = delete;

public slots:
    void quit(); //schließt den Dialog
    void tactMovement(); //behandelt taktische Verlegung (nur in eigene Nachbarländer)
    void stratMovement(); //behandelt stratgische Verlegung (Länder der selben Area)
    void attack(); //behandelt Angriff (nicht eigene Nachbarländer)

private:
    Ui::CCountryMenu *ui;

    //Attribute
    CGame* m_game; //Pointer auf das Spiel
    CCountry* m_country; //Pointer auf das Land, von dem aktion ausgehen soll

};

#endif // COUNTRYMENU_H
