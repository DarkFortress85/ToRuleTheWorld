#ifndef CMAP_H
#define CMAP_H

#include <QList>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include "country.h"



class CMap
{
public:
    //De- Konstruktoren
    CMap();
    ~CMap(); //1-3-5 Regel
    CMap(const CMap&)=delete;
    CMap& operator=(const CMap) = delete;

    //Methoden:
    void loadMap();  //parsen der txt Datei und CCountrys erstellen
    void setMapForSelection(CPlayer* player); //Aktivert alle Länder des aktuellen Spielers
    void setMapForAreaSelection(CArea* area); //Aktiviert alle Länder dieser Area
    void setMapToAreaView(CArea* area); //Verändert die Sichtbarkeit für alle Länder außer der Paramter-Area
    void setMapToGeneralView(); //Stellt die Sichtbarkeit für alle Länder zurück
    void setMapToTacticalMovement(CCountry* sourceCountry); //Stellt die Sichtbarket und Auswählbarkeit für taktische Verlegung
    void setMapToAttack(CCountry* sourceCountry); //Stellt die Sichtbarkeit und Auswählbarkeit für Angriff
    void setMapToStrategicalMovement(CCountry* sourceCountry); //Stellt die Sichtbarkeit und Auswählbarkeit für Strategische Verlegung
    void setMapBlind(); //Alle Icons werden unsichtbar und Länder nicht auswählbar

    //getter  - gibt das jeweilige Attribut zurück
    QGraphicsScene* getScene() const {return m_scene;}
    const QList<CCountry*>& getMap() const {return m_map;}
    const int getAmountOfCountrys() const {return m_amountOfCountrys;}

private:
    //Attribute
    //int m_MapID=0; //noch nicht in Verwendung, relevant für später wenn verschiedene Landkarten gespielt werden können - dient zur Kontrolle ob bei einem Load-Game auch die richtige Karte geladen wurde
    QList<CCountry*> m_map; //Liste der Länder aus der die Map besteht und die zur scene hinzugefügt werden
    QList<QGraphicsLineItem*> m_ways; //Liste der der Wege über das Meer - zur Visualisierung der Nachbarbeziehungen bei Meeresverbindung
    QGraphicsScene* m_scene; //Scene in die Länder hinzugegfügt werden (für die Game-View)
    int m_amountOfCountrys=0; //Anzahl der Länder der Map

    //Methoden:
    void convertNeighbourIntToNeighbourPtr(); //konvertiert die Nachbar IDs in die Pointer der Länder und gibt sie den Ländern;
    void setMapIconVisablility(CPlayer* player); //für den Spieler werden alle EInheiteninformationen angezeigt
};

#endif // CMAP_H
