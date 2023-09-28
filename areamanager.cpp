#include "areamanager.h"

CAreaManager::~CAreaManager()
{
    for (auto area : m_areas)
        delete area;
}

void CAreaManager::initAreas(QList<CCountry *> countrys)
{
    for (auto country : countrys) //für jedes Land prüfen ob schon Area - wenn nullptr dann Area-Suche beginnen
    {
        if(country->getArea() == nullptr)
        {
            m_areas.append(new CArea(country,country->getOwner(),country->getName()));
        }
    }
}

void CAreaManager::renewAreaForPlayer(QList<CCountry *> countrys, CPlayer *player)
{
    //Area aus Ländern entfernen
    for (auto country : countrys)
    {
        if(country->getOwner() == player)        
            country->setArea(nullptr);
    }

    //Area selbst entfernen
    for (int i = 0;i<m_areas.size();++i)
    {
        if(m_areas[i]->getOwner()==player)
        {
            delete m_areas[i];
            m_areas[i]=nullptr;
        }

    }
    //Liste bereinigen
    m_areas.removeAll(nullptr);
    m_areas.squeeze();

    //neue Areas erstellen
    for (auto country : countrys)
    {
        if(country->getOwner() == player && country->getArea() == nullptr)
        {
            m_areas.append(new CArea(country,country->getOwner(),country->getName()));
        }
    }
    //Area-Status des spielers setzen
    player->setAreaState(true);
}

void CAreaManager::newTurnSupplyRatio(CPlayer *player) //Versorgungsraten für neue Runde berechnen
{
    for (auto area : m_areas)
    {
        if(area->getOwner() == player)
        {
            area->spreadSupplyRatio(false); //in diesem Fall echte Berechnung der Versorungsraten
        }
    }
}

void CAreaManager::newTurnUnitsToSet(CPlayer *player) //neue Einheiten für neue Runde berechnen
{
    for (auto area : m_areas)
    {
        if(area->getOwner() == player)
        {
            area->calculateUnitsToSet();
        }
    }
}
