#ifndef CPLAYER_H
#define CPLAYER_H

#include <QString>
#include <QColor>
#include <QVector>
#include <QRandomGenerator>
#include <QTextStream>


class CPlayer
{

private:
    static QRandomGenerator m_cardGenerator; //erstellt Zufallszahlen zwischen 1 und 3 für die Tauschkarten

public:
    //De- Konstruktoren
    CPlayer(const int& id, const QString& name, const QColor& color) : m_playerID(id), m_playerName(name), m_color(color) //für neuen Spielbeginn
    {
        createCard();
    }
    CPlayer(const int& id, const QString& name, const QColor& color, const QVector<int>& cards,const bool inGameStatus,const int& gameOverTurn) : //für LoadGame
        m_playerID(id), m_playerName(name), m_color(color), m_cards(cards), m_activeInGame(inGameStatus),m_playerGameOverTurn(gameOverTurn) {};

    //getter - gibt das jeweilige Attribut zurück
    const QString& getName() const {return m_playerName;}
    const QColor& getColor() const {return m_color;}
    const int getID() const {return m_playerID;}
    const int getNumberOfCards() const {return m_cards.size();}
    const int getPlayerUnits() const {return m_playerUnits;}
    QVector<int>* getCards_ptr() {return &m_cards;}
    const bool getAreaState() const {return m_areaStatus;}
    const bool getActiveInGame() const {return m_activeInGame;}
    const int getNumberOfCountrysHold() const {return m_numberOfCountrysHold;}
    const int getTotalNumberOfUnits() const {return m_totalNumberOfUnits;}
    const int getGameOverTurn() const {return m_playerGameOverTurn;}


    //setter
    void setAreaState(bool state) {m_areaStatus = state;} //wird gesetzt (false) bei Gebietsverlust
    void setActiveInGameState(bool state) {m_activeInGame = state;} //wird gesetzt (false) wenn Spieler keine Länder mehr hat
    void setGameOverTurn(int turn) {m_playerGameOverTurn = turn;} //wird gesetzt wenn Spieler aus Spiel ausscheidet bzw. wenn Partie beendet wird
    void setNumberOfCountrysHold(int countrys) {m_numberOfCountrysHold = countrys;} //Anzahl der Länder des Spielers (wird nach Gebietsveränderung gesetzt)
    void setTotalNumberOfUnits(int units) {m_totalNumberOfUnits = units;} //Anzahl aller Einheiten des Spielers - wird gesetzt wenn Partie beendet wird

    //Methoden
    void createCard(); //erstellt eine Zufallszahl zwischen 1 und 3 als Einheiten-Tauschkarten
    void addUnits(int units) {m_playerUnits += units;} //fügt die Einheiten, die der Spieler beim Kartentausch erhält, dem Spieler hinzu
    void removeUnits(int units) {m_playerUnits -= units;} //vermindert die Einheiten, wenn der Spieler diese in die Länder setzt

    friend QTextStream& operator<<(QTextStream& out, const CPlayer& player); //Schreibt essentielle Attribute für die Save Datei
    bool operator<(const CPlayer& otherPlayer) const; //Sortierung für Rangliste am Spielende - besser Spieler "ist kleiner"


private:
    //Attribute:
    const int m_playerID; //fortlaufende Nummer der jeweiligen Spieler
    const QString m_playerName; //vergebener Name
    const QColor m_color; //gewählte Spielerfarbe
    QVector<int> m_cards; //Spielerkarten
    int m_playerUnits = 0; //Einheiten durch Kartentausch erhalten
    bool m_activeInGame = true; //Status ob schon ausgeschieden
    bool m_areaStatus = true; //Status ob Areas für den Spieler aktuell sind
    int m_playerGameOverTurn = 0; //Runde in der Spieler ausgeschieden ist
    int m_numberOfCountrysHold = 0; //Anzahl der Länder die der Spieler hält
    int m_totalNumberOfUnits = 0; //Anzahl der Einheiten die der Spieler gesamt auf der Karte hat
};

#endif // CPLAYER_H
