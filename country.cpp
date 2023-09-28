#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QMessageBox>

#include "country.h"

CCountry::CCountry(const QPolygonF& polygon, const QString& name, const int id, const QList<int>& neighbours, const QPointF& namePos, const int& nameRot, const QPointF& iconPos)
    : m_ID(id), m_name(name),m_neighbours(neighbours),m_namePos(namePos),m_nameRot(nameRot),m_iconPos(iconPos)
{
    //Polygon zuweisen:
    setPolygon(polygon);

    //Attribute zuweisen:
    //m_name = name;
    //m_ID = id;
    //m_neighbours = neighbours;
    //m_namePos = namePos;
    //m_nameRot = nameRot;
    //m_iconPos = iconPos;

    //Icon erstellen
    m_unitIcon = new QGraphicsItemGroup(this);
    m_unitIcon->setZValue(1); //soll immer im Vorderung sein
    unitsText = new QGraphicsTextItem(QString::number(m_units),m_unitIcon);
    supplyStatusText = new QGraphicsTextItem(QString(""),m_unitIcon);
    supplyRatioText = new QGraphicsTextItem(QString(""),m_unitIcon);
    moveText = new QGraphicsTextItem("Move",m_unitIcon);
    attacText = new QGraphicsTextItem("Attack",m_unitIcon);
    soilderIcon = new QGraphicsPixmapItem(QPixmap(":/map/unitIcon.png"),m_unitIcon);
    m_unitIcon->addToGroup(soilderIcon);
    m_unitIcon->addToGroup(unitsText);
    m_unitIcon->addToGroup(supplyStatusText);
    m_unitIcon->addToGroup(supplyRatioText);
    m_unitIcon->addToGroup(moveText);
    m_unitIcon->addToGroup(attacText);
    m_unitIcon->setPos(m_iconPos);
    m_unitIcon->setVisible(false);

    //Icon anpassen
    soilderIcon->setScale(0.1);
    unitsText->setPos(soilderIcon->boundingRect().width()*soilderIcon->scale(),0);
    supplyRatioText->setPos(unitsText->x()+unitsText->boundingRect().width(),0);
    supplyStatusText->setPos(supplyRatioText->x()+supplyRatioText->boundingRect().width(),0);
    moveText->setPos(0,unitsText->boundingRect().height());
    attacText->setPos(moveText->boundingRect().width(),unitsText->boundingRect().height());
    setMoveStatus(true);
    setAttackStatus(true);


    //Name erstellen
    QGraphicsTextItem* nameText = new QGraphicsTextItem(m_name, this);
    nameText->setPos(m_namePos);
    nameText->setRotation(m_nameRot);

    //Initialfarbe
    this->setBrush(QBrush(QColor(Qt::white)));

}

CCountry::~CCountry()
{
}

void CCountry::addUnits(int units) //keine neue Versorgungsberechnung
{
    m_units += units;
    unitsText->setPlainText(QString::number(m_units));
    supplyRatioText->setPos(unitsText->x()+unitsText->boundingRect().width(),0); //Position der Versorungsrate anpassen (Einheitenzahl 1-3 Stellig)
}

void CCountry::removeUnits(int units) //keine neue Versorgungsberechnung
{
    m_units -=units;
    unitsText->setPlainText(QString::number(m_units));
    supplyRatioText->setPos(unitsText->x()+unitsText->boundingRect().width(),0);//Position der Versorungsrate anpassen (Einheitenzahl 1-3 Stellig)
}

void CCountry::createUnits(int units) //neue Versorgungsberechnung zur Info (kein Verschleiß)
{
    m_units +=units;
    unitsText->setPlainText(QString::number(m_units));
    m_area->spreadSupplyRatio(true);
    supplyRatioText->setPos(unitsText->x()+unitsText->boundingRect().width(),0);//Position der Versorungsrate anpassen (Einheitenzahl 1-3 Stellig)
}

void CCountry::destroyUnits(int units) //neue Versorgungsberechnung zur Info (kein Verschleiß)
{
    m_units -=units;
    unitsText->setPlainText(QString::number(m_units));
    m_area->spreadSupplyRatio(true);
    supplyRatioText->setPos(unitsText->x()+unitsText->boundingRect().width(),0);//Position der Versorungsrate anpassen (Einheitenzahl 1-3 Stellig)
}

void CCountry::handleNewSupplyRatio(float newSupplyRatio)
{
     supplyRatioText->setPlainText(QString::number(newSupplyRatio,'f',2)); //Anzeige aktualisieren

    //Wenn neue Versorungsrate mind 1
    if(newSupplyRatio >= 1)
    {       
        supplyStatusText->setPlainText("");
    }
    //Wenn neue Versorgungsrate darunter
    else
    {
        if(m_supplyRatio>=1) //wenn die Versorungsrate in der letzten Runde noch OK war dann keinen Verschleiß berechnen
        {
            supplyStatusText->setPlainText("!");
            supplyStatusText->setPos(supplyRatioText->x()+supplyRatioText->boundingRect().width(),0);
        }
        else
        {
            if(m_supplyRatio>newSupplyRatio) //Wenn Versorungsrate schon schlecht war und noch schlechter geworden ist, dann mit alter Rate berechnen
            {
                int attrition = (m_units - (m_units*m_supplyRatio))+1; //Verschleiß berechnen
                m_units -= attrition; //verschleiß von Einheiten abziehen
                unitsText->setPlainText(QString::number(m_units));
                supplyStatusText->setPlainText("!!");
                supplyStatusText->setPos(supplyRatioText->x()+supplyRatioText->boundingRect().width(),0);
                QMessageBox::information(nullptr,"Verschleiß",m_name + " hat " + QString::number(attrition) + " Einheiten Verschleiß aufgrund von Versorungsrate " +
                QString::number(m_supplyRatio,'f',2) + "erlitten!", QMessageBox::Ok);

            }
            else //Wenn Versorungsrate schon schlecht war aber besser geworden ist, dann mit neuer Rate berechnen
            {
                int attrition = (m_units - (m_units*newSupplyRatio))+1; //Verschleiß berechnen
                m_units -=attrition;  //verschleiß von Einheiten abziehen
                unitsText->setPlainText(QString::number(m_units));
                supplyStatusText->setPlainText("!!");
                supplyStatusText->setPos(supplyRatioText->x()+supplyRatioText->boundingRect().width(),0);
                QMessageBox::information(nullptr,"Verschleiß",m_name + " hat " + QString::number(attrition) + " Einheiten Verschleiß aufgrund von Versorungsrate " +
                QString::number(newSupplyRatio,'f',2) + "erlitten!", QMessageBox::Ok);
            }
        }
    }
    //Attribute updaten
    m_supplyRatio = newSupplyRatio;    
}

void CCountry::handleNewSupplyRatioInfoOnly(float newSupplyRatio)
{
    supplyRatioText->setPlainText(QString::number(newSupplyRatio,'f',2)); //Update der Karte
}

void CCountry::setOwner(CPlayer *owner)
{
    m_owner = owner;
    this->setBrush(QBrush(QColor(m_owner->getColor())));
    m_unitIcon->setVisible(true);
}

void CCountry::setMoveStatus(bool status) //Bewegungsstatus setzen
{
    m_unitsCanMove = status;
    if(status)
       moveText->setDefaultTextColor(QColor(Qt::green));
    else
        moveText->setDefaultTextColor(QColor(Qt::red));
}

void CCountry::setAttackStatus(bool status) //Angriffsstatus setzen
{
    m_unitsCanAttack = status;
    if(status)
       attacText->setDefaultTextColor(QColor(Qt::green));
    else
        attacText->setDefaultTextColor(QColor(Qt::red));
}

void CCountry::setUnitsStatusVisability(bool visible) //Einheitenstatus anzeigen/nicht anzeigen
{
    supplyStatusText->setVisible(visible);
    supplyRatioText->setVisible(visible);
    moveText->setVisible(visible);
    attacText->setVisible(visible);
}


void CCountry::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) //Doppelklick Signal wir emittiert
{
    if (m_isClickable && event->button() == Qt::LeftButton)
        emit countryDoubleClicked(this);
}


QTextStream& operator<<(QTextStream& out, const CCountry& country)
{
    out << country.m_ID << "|" << country.m_owner->getID() << "|" << country.m_units << "|" << country.m_supplyRatio << ";\n";
    return out;
}
