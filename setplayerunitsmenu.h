#ifndef SETPLAYERUNITSMENU_H
#define SETPLAYERUNITSMENU_H

#include <QDialog>
#include <QGraphicsScene>
#include "country.h"
#include "player.h"


namespace Ui {
class CSetPlayerUnitsMenu;
}

class CSetPlayerUnitsMenu : public QDialog
{
    Q_OBJECT

public:
    //De- Konstruktoren
    explicit CSetPlayerUnitsMenu(CCountry *country, CPlayer *player, QGraphicsScene *scene, QWidget *parent = nullptr);
    ~CSetPlayerUnitsMenu(); //1-3-5 Regel
    CSetPlayerUnitsMenu(const CSetPlayerUnitsMenu&)=delete;
    CSetPlayerUnitsMenu& operator=(const CSetPlayerUnitsMenu) = delete;

public slots:
    void quit(); // für den PB "Abbrechen" --> schließt den Dialog
    void setUnits(); // für den PB "Sezten!" --> setzt die gewählten Einheiten in das Land und schließt den Dialog

private:
    Ui::CSetPlayerUnitsMenu *ui;
    CCountry* m_country; //das Quelland aus CGame//der aktuelle Spieler aus CGame
    CPlayer* m_player; //der aktuelle Spieler aus CGame
};

#endif // SETPLAYERUNITSMENU_H
