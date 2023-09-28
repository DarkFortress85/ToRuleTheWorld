#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <QDialog>
#include <QList>
#include <QStringList>
#include "player.h"

namespace Ui {
class CGameOver;
}

class CGameOver : public QDialog
{
    Q_OBJECT

public:
    //De- Konstruktoren
    explicit CGameOver(const QList<CPlayer*> &players, QWidget *parent = nullptr);
    ~CGameOver(); //1-3-5 Regel
    CGameOver(const CGameOver&)=delete;
    CGameOver& operator=(const CGameOver) = delete;

public slots:
    void quit(); //Beendet den Dialog

private:
    Ui::CGameOver *ui;
    QList<CPlayer*> m_players; //Liste der am Spiel teilgenommenen Spieler
    QStringList m_ranks; //Namen der Ränge
};

#endif // GAMEOVER_H
