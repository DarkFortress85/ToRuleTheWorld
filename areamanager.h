#ifndef CAREAMANAGER_H
#define CAREAMANAGER_H

#include <QList>
#include "area.h"
#include "country.h"


class CAreaManager
{
public:
    //De- Konstruktoren
    CAreaManager() {};
    ~CAreaManager(); //1-3-5 Regel
    CAreaManager(const CAreaManager&)=delete;
    CAreaManager& operator=(const CAreaManager) = delete;

    //getter / setter --> gibt das jeweilige Arttribut zurück
    const QList<CArea*>& getAreas() const {return m_areas;}

    //Methoden
    void initAreas(QList<CCountry*> countrys); //erstellt die Initial-Areas für alle Spieler nach der Länderauswahl
    void renewAreaForPlayer(QList<CCountry*> countrys, CPlayer* player); //entfernt alle Areas des Spieler und erstellt sie neu
    void newTurnSupplyRatio(CPlayer* player); //Für alle Areas des aktuellen Spielers, werden die Versorgungsraten berechnet
    void newTurnUnitsToSet(CPlayer* player); //Ruft Funktion zur Berechnung der neuen Einheiten für den aktuellen Spieler auf

private:
    //Attribute
    QList<CArea*> m_areas; //Liste der Areas die es derzeit gibt

};

#endif // CAREAMANAGER_H
