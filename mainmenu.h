#ifndef CMAINMENU_H
#define CMAINMENU_H

#include <QDialog>
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CMainMenu; }
QT_END_NAMESPACE

class CMainMenu : public QDialog
{
    Q_OBJECT

public:
    //De- Konstruktoren
    CMainMenu(CGame* game, QWidget *parent = nullptr);
    ~CMainMenu(); //1-3-5 Regel
    CMainMenu(const CMainMenu&)=delete;
    CMainMenu& operator=(const CMainMenu) = delete;

    //getter - gibt das jeweilige Attribut zurück
    const bool getStartGameStaus() const {return m_startGame;}


public slots:
    void newGame(); //für PB "Neues Spiel" -> führt zur Spielerinitialisierung (playerinit)
    void loadGame(); // für PB "Spiel Laden"
    void quitGame(); // für PB "Spiel Verlassen"

private:
    Ui::CMainMenu *ui;
    CGame* m_game; //Pointer auf das Spiel
    bool m_startGame = false; //Status ob ein neues Spiel gestartet werden soll (erfolgreich mind. 2 Spielernamen eingetragen oder Spiel Laden ausgewählt)
};
#endif // CMAINMENU_H
