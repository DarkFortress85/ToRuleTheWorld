#ifndef CCOUNTRY_H
#define CCOUNTRY_H

#include <QGraphicsPolygonItem>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QList>
#include <QString>
#include <QPointF>
#include <QGraphicsItemGroup>
#include <QPolygonF>
#include <QObject>
#include <QPixmap>
#include <QTextStream>
#include "area.h"
#include "player.h"


class CCountry : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT

public:
    //De- Konstruktoren
    CCountry(const QPolygonF& polygon, const QString& name, const int id, const QList<int>& neighbours, const QPointF& namePos, const int& nameRot, const QPointF& iconPos);
    ~CCountry(); //1-3-5 Regel
    CCountry(const CCountry&)=delete;
    CCountry& operator=(const CCountry) = delete;

    //Methoden
    void addUnits(int units); //zieht Einheiten in das Land - keine neue Versorgungsberechnung
    void removeUnits(int units); ///zieht Einheiten aus dem Land - keine neue Versorgungsberechnung
    void createUnits(int units); //neue Einheiten werden erschaffen - neue Versorungsberechnung für Info-Icon wird durchgeführt
    void destroyUnits(int units); //Einheiten werden zerstört - neue Versorungsberchnung für Info-Icon wird durchgefürt
    void handleNewSupplyRatio(float newSupplyRatio); //Berechnet Verschleiß und stellt Icon-Informationen richtig
    void handleNewSupplyRatioInfoOnly(float newSupplyRatio); //stellt nur Icon-Information über Versorung richtig

    friend QTextStream& operator<<(QTextStream& out, const CCountry& country); //Schreibt essentielle Attribute für Save-Datei


    //getter - gibt das jeweilige Attribut zurück
    const bool isClickable() const {return m_isClickable;}
    const QString& getName() const {return m_name;}
    CPlayer* getOwner() const {return m_owner;}
    const QPointF& getIconPos() const {return m_iconPos;}
    const QList<int>& getNeighbourInt() const {return m_neighbours;}
    const QList<CCountry*>& getNeighbourPtr() const {return m_neighboursPtr;}
    const int getID() const {return m_ID;}
    const CArea* getArea() const {return m_area;}
    const int getUnits() const {return m_units;}
    const bool getMoveStatus() const {return m_unitsCanMove;}
    const bool getAttackStatus() const {return m_unitsCanAttack;}


    //setter
    void setClickable(bool clickable) {m_isClickable= clickable;} // setzt den "Klickstatus" - wird beim MouseClickEvent überprüft
    void setOwner(CPlayer* owner); //setzt den Spieler, dem das Land gehört
    void setNeighbourPtr(CCountry*& neighbourPtr) {m_neighboursPtr.append(neighbourPtr);} //setzt die Pointer der Nachbarländer
    void setMoveStatus(bool status); //setzt m_unitsCanMove Status und Move-Text Farbe
    void setAttackStatus(bool status); //setzt m_unitsCanAttack Status und Attack-Text Farbe
    void setSupplyRatio(float supplyRatio) {m_supplyRatio = supplyRatio;}; //setzt Supply-Ratio (nur beim Laden von Spielstand zu verwenden)
    void setIconVisability(bool visible) {m_unitIcon->setVisible(visible);} //Sichtbarkeite des Units-Icons setzen
    void setArea(CArea* area) {m_area = area;} //Setzt die Area zu der das Land gehört
    void setUnitsStatusVisability(bool visible); //Sichtbarkeit des Versorungsstatus und der Move/Attack Stati


signals:
    void countryDoubleClicked(CCountry* country); //Sendet das Siganl, dass das Land doppelt angeklickt wurde

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override; //MouseEvent überschrieben -> countryDoubleClicked wird gesendet

private:
    //Attribute
    const int m_ID; //fortlaufende ID
    const QString m_name; //Name für Karte
    const QList<int> m_neighbours; //ID der Nachbarländer
    QList<CCountry*> m_neighboursPtr; //Pointer der Nachbarländer
    CPlayer* m_owner = nullptr; //Pointer des derzeitigen Besitzers
    CArea* m_area = nullptr; //Pointer der derzeitigen Area zu der dieses Land gehört
    const QPointF m_namePos; //Koordinaten des Namnes für die Karte
    const int m_nameRot; //Grade der Rotierung des Namnes auf der Karte
    QGraphicsItemGroup* m_unitIcon; //Einheiten Icon und Infos
    const QPointF m_iconPos; //Koordindaten des Icons für die Karte
    int m_units=0; //Anzahl einheiten in diesem Land
    bool m_unitsCanMove = true; //Status ob Einheiten noch ziehen können
    bool m_unitsCanAttack = true; //Status ob Einheiten noch angreifen können
    bool m_isClickable = false; //Status ob das Land anklickbar sein soll
    float m_supplyRatio = 1; //Aktuelle Versorgungsrate für Berechnung

    //Bestandteile des Icons als eigene Variable definiert, wegen leichterm späteren Zugriff
    QGraphicsTextItem* unitsText; //Anzahl der Einheiten
    QGraphicsTextItem* moveText; //Wort "Move" mit Farbe für Status
    QGraphicsTextItem* attacText; //Wort "Attack" mit Farbe für Status
    QGraphicsPixmapItem* soilderIcon; //die Pixmap für das SoldatenIcon
    QGraphicsTextItem* supplyStatusText; //Versorungsstatus der Einheiten (! bzw. !!)
    QGraphicsTextItem* supplyRatioText; //Versorungsrate der Einheiten

};

#endif // CCOUNTRY_H
