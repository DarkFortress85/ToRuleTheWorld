#include "map.h"
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QPointF>
#include <QPen>


CMap::CMap()
{
    m_scene = new QGraphicsScene();
}


CMap::~CMap()
{
    //deletes
    //Reihenfolge beachten - Country vor Scene
    for (auto it : m_map)
        delete it;
    delete m_scene;

}

void CMap::loadMap()
{
    // Datei öffenen und Stream erstellen
    QFile infile(":/map/map.txt");
    infile.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream stream(&infile);

    // Variablen anlegen
    QString keyWord;
    QChar character;
    int id=0, neighbour;
    QString name;
    QList<int> neighbour_list;
    QPointF namePos, iconPos, coordinate_point, wayPoint1, wayPoint2;
    int nameRot;
    QPolygonF polygon;
    float x, y;

    //Kopf weglesen:
    while (keyWord != "<mapStart>")
        stream >> keyWord;

    //erstes KeyWord einlesen - "<countryStart>"
    stream >> keyWord; // <countryStart> einlesen

    //schleife bis "<mapEnd>" für Länder
    while (keyWord != "<mapEnd>")
    {
        if (keyWord != "<countryStart>")
            qDebug() << "Formatfehler Land-Beginn";
        else
            stream >> keyWord; // "id:" einlesen

        if (keyWord != "id:")
            qDebug() << "Formatfehler Block ID";
        else
        {
            stream >> character;
            if(!character.isSpace())
                qDebug() << "Formatfehler in ID";
            else
            {
                stream >> id;
                stream >> character;
                if(character != ';')
                    qDebug() << "Formatfehler Trenner nach ID";
            }
        }

        stream >> keyWord; // "name:" einlesen
        if (keyWord != "name:")
            qDebug() << "Formatfehler Block name";
        else
        {
            name.clear();
            stream >> character;
            if(!character.isSpace())
                qDebug() << "Formatfehler in name";
            else
            {
                while(character != ';')
                {
                    stream >> character;
                    if (character != ';')
                        name += character;
                }

            }
        }

        stream >> keyWord; // "neighbours:" einlesen
        if (keyWord != "neighbours:")
            qDebug() << "Formatfehler Block neighbours";
        else
        {
            neighbour_list.clear();
            stream >> character;
            if (!character.isSpace())
                qDebug() << "Formatfehler in neighbours";
            else
            {
                while (character != ';')
                {
                    stream >> neighbour;
                    neighbour_list.append(neighbour);
                    stream >> character;
                }
            }
        }

        stream >> keyWord; //"namePos:" einlesen
        if (keyWord != "namePos:")
            qDebug() << "Formatfehler Block namePos:";
        else
        {
            stream >> character;
            if(!character.isSpace())
                qDebug() << "Formatfehler in namePos";
            stream >> x;
            stream >> character;
            if (character != ',')
                qDebug() << "Formatfehler Koordinatentrenner namePos";
            stream >> y;
            stream >> character;
            if (character != ';')
                qDebug() << "Formatfehler Trenner nach namePos";
            namePos.setX(x);
            namePos.setY(y);
        }

        stream >> keyWord; //"nameRot:" einlesen
        if (keyWord != "nameRot:")
            qDebug() << "Formatfehler Block nameRot:";
        else
        {
            stream >> character;
            if(!character.isSpace())
                qDebug() << "Formatfehler in nameRot:";
            stream >> nameRot;
            stream >> character;
            if (character != ';')
                qDebug() << "Formatfehler Trenner nach nameRot:";
        }

        stream >> keyWord; //"iconPos:" einlesen
        if (keyWord != "iconPos:")
            qDebug() << "Formatfehler Block iconPos:";
        else
        {
            stream >> character;
            if(!character.isSpace())
                qDebug() << "Formatfehler in iconPos:";
            stream >> x;
            stream >> character;
            if (character != ',')
                qDebug() << "Formatfehler Koordinatentrenner iconPos";
            stream >> y;
            stream >> character;
            if (character != ';')
                qDebug() << "Formatfehler Trenner nach iconPos";
            iconPos.setX(x);
            iconPos.setY(y);
        }

        stream >> keyWord; //"coordinates:" einlesen
        if (keyWord != "coordinates:")
            qDebug() << "Formatfehler Block coordinates";
        else
        {
            polygon.clear();
            stream >> character;
            if(!character.isSpace())
                qDebug() << "Formatfehler in coordinates";
            else
            {
                while (character != ';')
                {
                    stream >> x;
                    stream >> character;
                    if(character != ',')
                        qDebug() << "Formatfehler Trenner x/y-Koordinaten";
                    stream >> y;
                    coordinate_point.setX(x);
                    coordinate_point.setY(y);
                    polygon<<coordinate_point;
                    stream >> character;
                }
            }
        }

        stream >> keyWord; //"countryEnd" enlesen und Land anlegen!
        if (keyWord != "<countryEnd>")
            qDebug() << "Formatfehler Land-Ende";
        else
            m_map.append(new CCountry(polygon,name,id,neighbour_list,namePos,nameRot,iconPos));

        stream >> keyWord; //beginn nächste Runde

    } // Ende Haupt-While-Schleife für Länder

    //Wege einlesen
    // Zwischeninformationen weglesen
    while (keyWord != "<waysStart>")
        stream >> keyWord;

    stream >>keyWord; // erstes Keyword "way:" einlesen

    // Hauptschleife für Wege
    while (keyWord != "<waysEnd>")
    {
        if (keyWord != "way:")
            qDebug()<< "Formatfehler Beginn ways:";
        else
        {
            //erste Koordinate einlesen
            stream >> character;
            if(!character.isSpace())
                qDebug() << "Formatfehler in ways";
            stream >> x;
            stream >> character;
            if (character != ',')
                qDebug() << "Formatfehler Koordinatentrenner ways";
            stream >> y;
            stream >> character;
            if (!character.isSpace())
                qDebug() << "Formatfehler Trenner für 2. Koordinate";
            wayPoint1.setX(x);
            wayPoint1.setY(y);
            //zweite Korrdinate einlesen
            stream >> x;
            stream >> character;
            if (character != ',')
                qDebug() << "Formatfehler Koordinatentrenner ways 2. Koordinate";
            stream >> y;
            stream >> character;
            if (character != ';')
                qDebug() << "Formatfehler Trenner nach Ways";
            wayPoint2.setX(x);
            wayPoint2.setY(y);
            //QGraphicsLineItem erstellen
            m_ways.append(new QGraphicsLineItem(QLineF(wayPoint1,wayPoint2)));
        }
        stream >> keyWord;
    }

    infile.close(); //File schließen



    //Rahmensträke festlegen und Länder der scene hinzufügen
    for (auto country : m_map)
    {
        country->setPen(QPen(Qt::darkGray,4));
        m_scene->addItem(country);
    }

    //Lieniensträrke festelegen und Wege der scene hinzufügen
    for (auto way : m_ways)
    {
        way->setPen(QPen(Qt::darkGray,5));
        m_scene->addItem(way);
    }

    //Anzahl der Länder initiieren
    m_amountOfCountrys = m_map.size();

    //NachbarPointer initiieren
    convertNeighbourIntToNeighbourPtr();
}

void CMap::setMapForSelection(CPlayer *player)
{
    for (auto country : m_map) //für jedes Land
    {
        if (country->getOwner() == player) //wenn Land der Spieler ist, dann auswählbar
            country->setClickable(true);
        else
            country->setClickable(false);
    }
    setMapIconVisablility(player); //für Spieler sind die Icons-Sichtbar
}

void CMap::setMapForAreaSelection(CArea *area)
{
    for (auto country : m_map) //für jedes Land
    {
        if (country->getArea() == area) //wenn Land in der Area ist, dann auswählbar
            country->setClickable(true);
        else
            country->setClickable(false);
    }
}

void CMap::setMapToAreaView(CArea *area)
{
    for (auto country : m_map) // wenn Land nicht in Area, dann 50% transparent
    {
        if (country->getArea() != area)
            country->setOpacity(0.5);
    }

}

void CMap::setMapToGeneralView() //alle Länder auf 0% transparent zurückstellen
{
    for (auto country : m_map)
        country->setOpacity(1);
}

void CMap::setMapToTacticalMovement(CCountry *sourceCountry)
{
    //Alle Länder nicht Auwählbar machen und transparent stellen
    for (auto country : m_map)
    {
        country->setOpacity(0.5);
        country->setClickable(false);
    }
    //Nachbarländer wenn selber Besitzer wieder zurückstellen
    for (auto country : sourceCountry->getNeighbourPtr())
    {
        if(country->getOwner() == sourceCountry->getOwner())
        {
            country->setOpacity(1);
            country->setClickable(true);
        }
    }
}

void CMap::setMapToAttack(CCountry *sourceCountry)
{
    //Alle Länder nicht Auwählbar machen und transparent stellen
    for (auto country : m_map)
    {
        country->setOpacity(0.5);
        country->setClickable(false);
    }
    //Nachbarländer wenn anderer Besitzer wieder zurückstellen
    for (auto country : sourceCountry->getNeighbourPtr())
    {
        if(country->getOwner() != sourceCountry->getOwner())
        {
            country->setOpacity(1);
            country->setClickable(true);
        }
    }
}

void CMap::setMapToStrategicalMovement(CCountry *sourceCountry)
{
    //Alle Länder nicht Auwählbar machen und transparent stellen
    for (auto country : m_map)
    {
        country->setOpacity(0.5);
        country->setClickable(false);
    }
    //Länder der Aera, außer das sourceCountry wieder zurückstellen
    for (auto country : sourceCountry->getArea()->getMemberCountrys())
    {
        if(country !=sourceCountry)
        {
            country->setOpacity(1);
            country->setClickable(true);
        }
    }

}

void CMap::setMapBlind()
{
    //Alle Länder nicht Auwählbar machen und transparent stellen
    for (auto country : m_map)
    {
        country->setIconVisability(false);
        country->setClickable(false);
    }
}

void CMap::setMapIconVisablility(CPlayer *player) //allen eigenen Ländern und Nachbarländern die Icons anzeigen
{
    //inital alle auf unsichtbar setzen
    for (auto country : m_map)
    {
        country->setIconVisability(false);
    }
    for (auto country : m_map) //Procedere für jedes Land der Map
    {
        if (country->getOwner() == player) //prüfen ob Land dem aktuellen spieler gehört - wenn ja
        {
            country->setIconVisability(true); //wenn eigenes Land dann sichtbar stetzen
            country->setUnitsStatusVisability(true);
            for (auto neighbourIDPtr : country->getNeighbourPtr()) //dann bei allen Nachbarn auf sichtbar setzten
            {
                if (neighbourIDPtr->getOwner()!=player)
                {
                    neighbourIDPtr->setIconVisability(true);
                    neighbourIDPtr->setUnitsStatusVisability(false);
                }
            }
        }
    }
}

void CMap::convertNeighbourIntToNeighbourPtr()
{
    for (auto country : m_map) //Procedere für jedes Land der Map
    {
        for (auto neighbourID : country->getNeighbourInt()) //Nachbarn als Int holen und für jeden Nachbar-INT den Pointer des Landes mit dieser ID finden
        {
            for (auto country2 : m_map) //Die ID jedes Landes vergleichen, ob sie mit der aktuellen neighbourID zusammenstimmt
            {
                if(country2->getID() == neighbourID)
                {
                    country->setNeighbourPtr(country2); //wenn ja, dann den Pointer dieses Landes in die Liste eintragen und
                    break; //und diese Schleife abbrechen
                }
            }
        }
    }
}
