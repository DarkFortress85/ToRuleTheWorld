#ifndef AREAMENU_H
#define AREAMENU_H

#include <QDialog>
#include "area.h"
#include "game.h"

namespace Ui {
class CAreaMenu;
}

class CAreaMenu : public QDialog
{
    Q_OBJECT

public:
    //De- Konstruktoren
    explicit CAreaMenu(CGame* game, CArea* area,QWidget *parent = nullptr);
    ~CAreaMenu(); //1-3-5 Regel
    CAreaMenu(const CAreaMenu&)=delete;
    CAreaMenu& operator=(const CAreaMenu) = delete;

    //getter - gibt das jeweilige Attribug zurück
    const int getUnitsForAction() const {return m_unitsForAction;}

public slots:
    void quit(); //Schließt den Dialog ohne Änderung
    void selectTargetCountry(); //Überigbt die Anzahl der Einheiten an CGame, stellt den status in CGame richtig und schließt den Dialog
    void setSpinBoxMax(); //Trägt die maximal Mögliche Einheitenanzahl in die SpinBox ein

private:
    Ui::CAreaMenu *ui;
    CArea* m_area; //Gewählte Area
    CGame* m_game; //Pointer auf das Spiel
    int m_unitsForAction = 0; //Anzahl der vom Spieler gewählten Einheiten, die danach gesetzt werden sollen
};

#endif // AREAMENU_H
