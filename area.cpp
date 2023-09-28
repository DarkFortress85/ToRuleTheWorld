#include "area.h"
#include "country.h"
#include <QtMath>


CArea::CArea(CCountry *baseCountry, CPlayer *owner, const QString &name) : m_baseCountry(baseCountry), m_name(name) ,m_owner(owner)
{
    m_memberCountrys.append(m_baseCountry); //Basecountry in Liste Länderliste hinzufügen
    baseCountry->setArea(this); //Area in Basecountry eintragen
    addNeighbourCountrys(m_baseCountry); //Nachbarländer checken und hinzufügen
    //Statische Attribute berechnen
    calculateStaticAttributes();
    //Versorungsraten berechnen und verteilen zur Info
    spreadSupplyRatio(true);
}

CArea::~CArea()
{
    //Pointer auf Nullptr setzten - nulling out
    m_owner=nullptr;
    m_baseCountry=nullptr;
}

void CArea::spreadSupplyRatio(bool informationOnly)
{
    if (informationOnly)  //An Länder für ICON-Darstellung weitergeben
    {
        calculateSupplyRatio(); //aktuelle SupplyRatio berechnen
        for (auto country : m_memberCountrys) //diese an Länder weitergeben
            country->handleNewSupplyRatioInfoOnly(m_supplyRatio);
    }
    else //An Länder für Verschleiß Berechnung weitergeben
    {
        calculateSupplyRatio(); //aktuelle SupplyRatio berechnen und Ländern zur Verschleißberechnung weitergeben
        for (auto country : m_memberCountrys)
            country->handleNewSupplyRatio(m_supplyRatio);

        calculateSupplyRatio(); //danach noch einmal berechnen und zur Icon-Darstellung noch mal übergeben
        for (auto country : m_memberCountrys)
             country->handleNewSupplyRatioInfoOnly(m_supplyRatio);
    }
}

void CArea::calculateUnitsToSet()
{
    m_unitsToSet = 0; //bestehende Werte ggf. aus früheren Runden auf null setzen;
    int baseUnits = 20;
    double baseExpo = 1.1; //double damit Warnung von Compiler verschwindet
    float addExpo = static_cast<float>(m_numberOfCountrys)/50;
    float expo = baseExpo + addExpo;
    float factor = qPow(m_numberOfCountrys,expo);
    m_unitsToSet = baseUnits * factor;
}

void CArea::addNeighbourCountrys(CCountry *&baseCountry)
{
    for(auto neighbour : baseCountry->getNeighbourPtr())
    {
        if (neighbour->getOwner() == baseCountry->getOwner() && neighbour->getArea() != this) //wenn der selbe besitzer und die Area nicht diese Area ist dann
        {
            m_memberCountrys.append(neighbour); //dieses Land der Länderliste hinzufügen
            neighbour->setArea(this); //dem Land die Area zuweisen
            addNeighbourCountrys(neighbour); //die Nachbarn dieses Landes prüfen - rekursiver Aufruf
        }
    }
}

void CArea::calculateStaticAttributes()
{
    //Anzahl der Länder
    m_numberOfCountrys = m_memberCountrys.size();
    //Maximale Versorgung - in einzelne berechnungsschritte aufgeteilt, da sonst andere Werte entstanden sind
    int baseRatio = 50;
    double baseExpo = 1.1; //double damit Warnung von Compiler verschwindet
    float addExpo = static_cast<float>(m_numberOfCountrys)/50;
    float expo = baseExpo + addExpo;
    float factor = qPow(m_numberOfCountrys,expo);
    m_maxUnitSupply = baseRatio * factor;
}

void CArea::calculateSupplyRatio()
{
    //Anzahl der Einheiten der Area berechnen
    m_unitsInArea = 0;
    for (auto country : m_memberCountrys)
        m_unitsInArea += country->getUnits();

    //Versorungsrate berechnen
    m_supplyRatio = static_cast<float>(m_maxUnitSupply) / static_cast<float>(m_unitsInArea);
}
