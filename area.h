#ifndef CAREA_H
#define CAREA_H

#include <QString>
#include <QList>
#include "player.h"


class CCountry; //Forward deklaration

class CArea
{
public:
    //De- Konstruktoren
    CArea(CCountry* baseCountry, CPlayer *owner, const QString& name);
    ~CArea(); //1-3-5 Regel
    CArea(const CArea&)=delete;
    CArea& operator=(const CArea) = delete;

    //Methoden
    void spreadSupplyRatio(bool informationOnly); //aktualisiert Versorgungsrate und gibt sie den Ländern weiter
    void calculateUnitsToSet(); //Berechnet die Einheiten die in dieser Runde gesetzt werden können (m_UnitsToSet)
    void removeUnitsToSet(int units) {m_unitsToSet -= units;} //Verringert die Anzahl der zu setzenden Einheiten


    //getter - gibt das jeweilige Attribut zurück
    const CPlayer* getOwner() const {return m_owner;}
    const QList<CCountry*>& getMemberCountrys() const {return m_memberCountrys;}
    const CCountry* getBaseCountry() const {return m_baseCountry;}
    const int getUnitsInArea() const {return m_unitsInArea;}
    const int getUnitsToSet() const {return m_unitsToSet;}
    const float getSupplyRatio() const {return m_supplyRatio;}
    const int getMaxUnitSupply() const {return m_maxUnitSupply;}



private:
    //Attribute:
    CCountry* m_baseCountry; //Baisland der Area
    QString m_name; //Name der Area = Name des Baislandes
    CPlayer* m_owner; //Spieler zu dem die Area gehört
    QList<CCountry*> m_memberCountrys; //Länder aus denen die Area besteht
    int m_numberOfCountrys=0; //Anzahl der Länder aus denen die Area besteht
    int m_unitsInArea=0; //Anzal der Einheiten die aktuell in der Area plaziert sind
    int m_maxUnitSupply=0; //Maximale Anzahl an Einheiten die durch die Area versorgt werden können
    float m_supplyRatio=0; //Aktuelle Versorungsrate der Einheiten innerhalb der Area
    int m_unitsToSet=0; //Anzahl der Einheiten die von der Area generiert und noch plaziert werden können

    //Methoden
    void addNeighbourCountrys(CCountry*& baseCountry); //Fügt passende Nachbarländer zur Area hinzu - nur im Konstruktor
    void calculateStaticAttributes(); //Berechnet die statischen Attribute "numberOfCountrys" und "maxUnitSupply"
    void calculateSupplyRatio(); //Berechnet die aktuelle Supply-Ratio


};

#endif // CAREA_H
