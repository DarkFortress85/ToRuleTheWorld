#ifndef CARDCHANGEMENU_H
#define CARDCHANGEMENU_H

#include <QDialog>
#include <QList>
#include <QLabel>
#include "player.h"

namespace Ui {
class CCardChangeMenu;
}

class CCardChangeMenu : public QDialog
{
    Q_OBJECT

public:
    explicit CCardChangeMenu(CPlayer* player, QWidget *parent = nullptr);
    ~CCardChangeMenu(); //1-3-5 Regel
    CCardChangeMenu(const CCardChangeMenu&)=delete;
    CCardChangeMenu& operator=(const CCardChangeMenu) = delete;

public slots:
    void changeCards(); //tauscht die bestmöglichste Kombination an Karten gegen Einheiten aus
    void quit(); //schließt das Menü

private:
    Ui::CCardChangeMenu *ui;

    //Attribute
    CPlayer* m_player; //der Spieler für den das Menü geöffnet wurde
    QVector<int>* m_playersCards; //die Werte der Karten des Spielers
    int m_units=0; //Anzahl der Einheiten die beim Tausch erzielt werden
    int m_whatToChange=0; //Von welcher Zahl sind 3 gleiche vorhanden
    bool m_combinedCards = false; //Ob von jeder Karte mind. eine vorhanden ist
    QList<QLabel*> m_cardLayouts; //zur Speicherung der Poiner des UI->Card-Layouts

    //Methoden
    bool changeCardsPossible(); //Prüft ob Tausch möglich ist
    void fillCardLayout(); // Erstellt QLabels für die Karten und fügt sie dem Layout hinzu
    void clearCardLayout(); //Entfernt alle QLabels aus dem Karten-Layout
};

#endif // CARDCHANGEMENU_H
