#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QList>
#include <QTextStream>
#include "country.h"
#include "map.h"
#include "player.h"
#include "areamanager.h"

namespace Ui {
class CGame;
}

class CGame : public QWidget
{
    Q_OBJECT

public:
    //Stati in dem sich das Spiel befinden kann - ausschlaggebend für was bei Doppelklick auf Land passiert und Status (aktiv/inaktiv) der Push-Buttons
    enum Status {main, countryInitial,generalSet,areaSet, waitForNewPlayer,stratMovement,tactMovement,attack};

    //De- Konstruktoren
    explicit CGame(QWidget *parent = nullptr);
    ~CGame(); //1-3-5 Regel
    CGame(const CGame&)=delete;
    CGame& operator=(const CGame) = delete;

    //getter - gibt das jeweilige Attribut zurück
    CCountry* getSourceCountry() const {return m_sourceCountry;}
    CPlayer* getActualPlayer() const {return m_actualPlayer;}
    const CMap* getMap() const {return m_map;}
    CMap* getMap() {return m_map;}
    int getNumberOfCountryOwenedByPlayer(const CPlayer* player) const; //Retourniert die Anahl der Länder des als Parameter gelieferten Spielers
    const Status getGameState() const {return m_gameSate;}

    //setter
    void setPlayer(const QList<CPlayer*> players) {m_players = players;} //Setzt die Spielerliste (wird vom Hauptmenü aufgerufen)
    void setUnitsForAction(int units) {m_unitsForAction = units;} //setzt die Anzahl der Einheiten, die für die Folgeaktion verwendet werden
    void setGameState(Status gameState) {m_gameSate = gameState;} //setzt den aktuellen Spielstatus
    void setInfoText(QString& text); //setzt den Infomations/Anweisungstext

    //Methoden
    friend QTextStream& operator<<(QTextStream& out, const CGame& game); ////Schreibt essentielle Attribute für Save-Datei
    bool loadGame(QTextStream& stream); //lädt das Spiel und gibt zurück ob dies erfolgreich war



public slots:
    void handleCountryDoubleClicked(CCountry* country); //Empfängt Signal "countryDoubleClicked" und reagiert je nach Spielstatus
    void setGameInit(); //für PB "Länderauswahl"
    void startTurn(); //für PB "Zug Beginnen"
    void endSet(); //für PB "Setzen Beenden"
    void endTurn(); //für PB "Zug Beenden"
    void changeCards(); //für PB "Karten eintauschen"
    void openAreaMenu(CArea* area); //für jeweiligen "Area-PB"
    void quitAction(); // für PB "Aktion abbrechen"
    void playerMenu(CPlayer* player); //für jeweiligen "Player-PB"
    void endGame(); //für PB "Partie Beenden"
    void quitGame(); //für PB "Spiel Verlassen"
    void saveGame(); //für PB "Speichern"

private:
    //Attribute
    Ui::CGame *ui;
    Status m_gameSate = countryInitial; //Gamestatus zur Steuerung der Push-Buttons in dem Doppelcklick-Signa auf ein Land
    CMap* m_map; //Enthält Liste der Länder und Scene für die Game-View
    QList<CPlayer*> m_players = QList<CPlayer*>(); //List der Spieler
    CPlayer* m_actualPlayer=nullptr; //Spieler der derzeit am Zug ist
    CAreaManager m_areaManager; //Area Manager zur Verwaltung der Areas
    int m_actualRound =0; //Aktuelle Spielrunde
    CCountry* m_sourceCountry; //Land von dem die gewählte Aktion ausgeht
    CCountry* m_targetCountry; //Zielland für die gewählte Aktion
    CArea* m_actualArea = nullptr; //Aktuell ausgewählt Area für die Aktion
    int m_unitsForAction =0; //Anzahl der gewählten Einheiten für die nächste Aktion

    //Methoden
    void setInitCountry(); //erste Zuteilung Spieler zu Land
    void disableAllPB(bool status); //de- aktiviert alle PushButtons
    void nextPlayer(); //stellt den aktuellen Spieler auf den nächsten Spieler
    void setPBToGameState(); //aktiviert bzw. deaktiviert die PushButtons je nach GameStatus
    void setPlayerUnits(); //setzen der allgemeinen Spieler-Einheiten
    void updateAreaPB(); //löscht und erstellt Area Push-Buttons
    void updateAreaPBFont(); //Schrift der Area-PBs entweder fett oder normal je nach dem ob Einheiten zu setzen sind
    void openCountryMenu(); //öffnet das Ländermenü
    void openAttackMenu(); //öffnet das Battle-Menü
    void createPlayerPB(); //erstellt die Player Push-Buttons
};

#endif // GAME_H
