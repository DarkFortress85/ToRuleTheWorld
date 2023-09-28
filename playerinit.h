#ifndef PLAYERINIT_H
#define PLAYERINIT_H

#include <QDialog>
#include <QList>
#include "player.h"

namespace Ui {
class CPlayerInit;
}

class CPlayerInit : public QDialog
{
    Q_OBJECT

public:
    //De- Konstruktoren
    explicit CPlayerInit(QWidget *parent = nullptr);
    ~CPlayerInit(); //1-3-5 Regel
    CPlayerInit(const CPlayerInit&)=delete;
    CPlayerInit& operator=(const CPlayerInit) = delete;

    //getter  - gibt das jeweilige Attribut zurück
    const QList<CPlayer*> getPlayers() const {return m_players;}

public slots:
    void quitPlayerInit(); //für PB "Abbrechen"
    void playerInit(); //für PB "Spiel Starten"

private:
    Ui::CPlayerInit *ui;
    QList<CPlayer*> m_players; //Liste der Spieler - ID, Name und Farbe aus diesem Dialog erstellt

};

#endif // PLAYERINIT_H
