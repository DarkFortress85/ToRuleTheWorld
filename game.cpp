#include "game.h"
#include "ui_game.h"
#include "setplayerunitsmenu.h"
#include "cardchangemenu.h"
#include "playermenu.h"
#include "areamenu.h"
#include "countrymenu.h"
#include "battle.h"
#include "gameover.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QScreen>
#include <QDesktopServices>
#include <QColor>




CGame::CGame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CGame)
{
    ui->setupUi(this);
    this->setWindowTitle("To Rule The World");

    //Map laden und view zuweisen
    m_map = new CMap;
    m_map->loadMap();
    ui->mapView->setScene(m_map->getScene());

    //Connect der Public slots
    for (auto country : m_map->getMap()) //alle Länder verbinden;
         connect(country, &CCountry::countryDoubleClicked, this, &CGame::handleCountryDoubleClicked);

    connect(ui->countryInitPB,SIGNAL(clicked()),this,SLOT(setGameInit()));
    connect(ui->quitGameButton, SIGNAL(clicked()), this,SLOT(quitGame()));
    connect(ui->startTurnButton, SIGNAL(clicked()), this,SLOT(startTurn()));
    connect(ui->changeCardsPB, SIGNAL(clicked()), this, SLOT(changeCards()));
    connect(ui->quitActionPB,SIGNAL(clicked()),this,SLOT(quitAction()));
    connect(ui->endSetButton,SIGNAL(clicked()),this,SLOT(endSet()));
    connect(ui->endTurnButton,SIGNAL(clicked()),this,SLOT(endTurn()));
    connect(ui->saveButton,SIGNAL(clicked()),this,SLOT(saveGame()));
    connect(ui->endGameButton,SIGNAL(clicked()),this,SLOT(endGame()));

    //PB Staus setzten und Spielstatus richtig stellen
    disableAllPB(true);
    ui->countryInitPB->setEnabled(true);
    ui->quitGameButton->setEnabled(true);
    ui->instructionLabel->setText("Bitte starten Sie das Spiel, in dem Sie auf \"Länderauswahl beginnen\" klicken");

    //größe Anpassen Widget und View anpassen
    setGeometry(QApplication::primaryScreen()->availableGeometry());
    ui->mapView->resize(this->width()-ui->mapView->x(),(this->height()-ui->mapView->y())-30);
    QColor backroundColor = Qt::blue;
    backroundColor.setAlpha(100);
    ui->mapView->setBackgroundBrush(QBrush(backroundColor));

}

CGame::~CGame()
{
    delete ui;
    delete m_map;
    for (auto i : m_players)
        delete i;
}

void CGame::setInfoText(QString &text)
{
    ui->instructionLabel->setText(text);
}

int CGame::getNumberOfCountryOwenedByPlayer(const CPlayer *player) const
{
    int countryCount = 0;
    for (auto country : m_map->getMap())
    {
        if(country->getOwner() == player)
            ++countryCount;
    }
    return countryCount;
}

bool CGame::loadGame(QTextStream &stream)
{
    //Variablen anlegen
    QString keyword,name,cardString;
    int actualPlayer, actualRound, id,red,green,blue,card,gameOverTurn, boolInt, owner,units;
    float supplyRatio;
    bool aktiveInGame;
    QVector<int> cards;
    QColor color;
    char trenner,character;
    //Game Attribute auslesen
    stream >> keyword;
    if (keyword != "<GameStart>")
           return false;
    stream >> actualPlayer;
    stream >> trenner;
    if (trenner != '|')
          return false;
    stream >> actualRound;
    m_actualRound = actualRound;
    stream >> trenner;
    if(trenner != ';')
        return false;
    stream >> keyword;
    if (keyword != "<GameEnd>")
        return false;
    //Spieler einlesen;
    stream >> keyword;
    if (keyword != "<PlayerStart>")
        return false;
    stream >> keyword;
    while (keyword != "<PlayerEnd>")
    {
        stream >> id; //Id auslesen
        stream >> trenner;
        if(trenner != '|')
            return false;
        stream >> character;
        while(character != '|')//name auslesen
        {
            name += character;
            stream >> character;
        }
        stream >> red; //Farb-Code auslesen
        stream >> trenner;
        if(trenner != ',')
            return false;
        stream >> green;
        stream >> trenner;
        if(trenner != ',')
            return false;
        stream >> blue;
        stream >> trenner;
        if(trenner != '|')
            return false;
        color.setRgb(red,green,blue); //Farbe setzen;
        //Cards auslesen;
        stream >> character;
        while(character != '|')//cardString
        {
            cardString += character;
            stream >> character;
        }
        if(!cardString.isEmpty())
        {
            QStringList cardValues = cardString.split(",");
            for (auto &value : cardValues)
            {
                card = value.toInt();
                cards.append(card);
            }
        }
        //Active in Game auslesen
        stream >> boolInt;
        aktiveInGame = boolInt;
        stream >> trenner;
        if (trenner != '|')
            return false;
        //GameOverTurn auslesen
        stream >> gameOverTurn;
        stream >> trenner;
        if (trenner != ';')
            return false;
        //Spieler erstellen
        m_players.append(new CPlayer(id,name,color,cards,aktiveInGame,gameOverTurn));
        //Variablen aufräumen
        name.clear();
        cardString.clear();
        cards.clear();
        stream >> keyword;
    }// Ende Schleife für Spieler
    stream >> keyword;
    //Länder auslesen
    if(keyword != "<CountryStart>")
        return false;
    stream >> keyword;
    while(keyword != "<CountryEnd>")
    {
        stream >> id; //Id auslesen;
        stream >> trenner;
        if (trenner != '|')
            return false;
        stream >> owner; //Owner auslesen;
        stream >> trenner;
        if (trenner != '|')
            return false;
        stream >> units; //Units auslesen;
        stream >> trenner;
        if(trenner != '|')
            return false;
        stream >> supplyRatio; //Supply-Ratio auslesen;
        stream >> trenner;
        if(trenner != ';')
            return false;
        stream >> keyword;
       //Ausgelesene Infos der Länder Bearbeiten
        CCountry* countryPtr=nullptr;
        for(auto country : m_map->getMap())//CountryPointer anhand ID bekommen
        {
            if(country->getID() == id)
            {
                countryPtr = country;
                break;
            }
        }
        CPlayer* ownerPtr=nullptr;
        for (auto player : m_players)//PlayerPointer anhand Owner-ID bekommen
        {
            if(player->getID() == owner)
            {
               ownerPtr = player;
               break;
            }
        }
        if(countryPtr==nullptr || ownerPtr==nullptr)
            return false;

        countryPtr->setOwner(ownerPtr);
        countryPtr->addUnits(units);
        countryPtr->setSupplyRatio(supplyRatio);
    }// Ender Schleife für Länder und ende des Text Stream

    //Pointer für Actual-Player-ID finden
    CPlayer* actualPlyrPtr = nullptr;
    for (auto player : m_players)
    {
        if(player->getID() == actualPlayer)
        {
            actualPlyrPtr = player;
            break;
        }
    }
    if(actualPlyrPtr ==nullptr)
        return false;
    m_actualPlayer = actualPlyrPtr;
    //Länderanzahl für Spieler aktualisieren
    for(auto player : m_players)
    {
        player->setNumberOfCountrysHold(getNumberOfCountryOwenedByPlayer(player));
    }
    //sonstige Game-Einstellungen
    createPlayerPB();
    m_map->setMapBlind();
    m_gameSate = waitForNewPlayer;
    setPBToGameState();
    m_areaManager.initAreas(m_map->getMap());
    return true;
}


void CGame::handleCountryDoubleClicked(CCountry *country) //Aktion bei Doppelklick auf Land - je nach Spielstatus
{
    switch (m_gameSate)
    {
    case countryInitial:
        m_sourceCountry = country;
        setInitCountry();
        break;
    case generalSet:
        m_sourceCountry = country;
        setPlayerUnits();
        break;
    case areaSet:
        country->createUnits(m_unitsForAction); //LandAttribute anpassen
        country->setAttackStatus(false);
        country->setMoveStatus(false);
        m_actualArea->removeUnitsToSet(m_unitsForAction); //Gameattribute anpassen
        m_actualArea=nullptr;
        m_unitsForAction=0;
        m_map->setMapForSelection(m_actualPlayer); //Map anpassen
        m_map->setMapToGeneralView();
        m_gameSate=generalSet; //Gamestatus anpassen
        setPBToGameState();
        updateAreaPBFont(); //Area-PBs anpassen
        break;
    case main:
        m_sourceCountry = country;
        openCountryMenu();
        break;
    case stratMovement:
        country->setAttackStatus(false);
    case tactMovement:
        m_sourceCountry->removeUnits(m_unitsForAction);
        country->addUnits(m_unitsForAction);
        country->setMoveStatus(false);
        m_unitsForAction=0;
        m_map->setMapForSelection(m_actualPlayer);
        m_map->setMapToGeneralView();
        m_gameSate=main;
        setPBToGameState();
        break;
    case attack:
        m_targetCountry = country;
        openAttackMenu();
    default:
        break;
    }

}

void CGame::setGameInit()
{
    //Setzt Voraussetzungen für die Länderauswahl
    m_gameSate = countryInitial;
    m_actualPlayer = m_players.first(); //Spieler Setzen
    setPBToGameState();
    ui->actPlayerLabel->setText("Aktueller Spieler: " + m_actualPlayer->getName());
    ui->actPlayerLabel->setStyleSheet(QString("color: %1").arg(m_actualPlayer->getColor().name()));
    for (auto country : m_map->getMap()) //Länder auswählbar machen
        country->setClickable(true);

    //PlayerButtons erstellen und verbinden
    createPlayerPB();
}


void CGame::startTurn()
{
    nextPlayer(); //aktueller Spieler wird gesetzt und ggf Runde erhöht
    if(m_actualPlayer==m_players.first())
    {
        ++m_actualRound;
        ui->actTurnLabel->setText("Aktuelle Runde: " +QString::number(m_actualRound));
    }
    m_actualPlayer->createCard();
    m_gameSate = generalSet; //Spielstatus wird gesetzt
    setPBToGameState(); //PushButtons werden angepasst
    m_map->setMapForSelection(m_actualPlayer); //Mappe wird angepasst    
    if(!m_actualPlayer->getAreaState()) //Wenn Areastatus nicht korrekt, dann neue Areas anlegen
        m_areaManager.renewAreaForPlayer(m_map->getMap(),m_actualPlayer);
    m_areaManager.newTurnSupplyRatio(m_actualPlayer); //Versorungsraten berechnen
    m_areaManager.newTurnUnitsToSet(m_actualPlayer); //neue Einheiten berechnen
    updateAreaPB(); //Area Push-Buttons aktualisieren
}

void CGame::endSet()
{
    bool endSetPossible = true;
    if(m_actualPlayer->getNumberOfCards()==5) //Prüfen ob 5 Karten vorhanden - dann muss getauscht werden
    {
        endSetPossible = false;
        QMessageBox::information(this,"Karten eintauschen","Sie haben bereits 5 Tauschkarten. Sie müssen Karten tauschen", QMessageBox::Ok);
    }
    if(m_actualPlayer->getPlayerUnits()>0) //Prüfen ob Spieler noch persönliche Einheiten zu setzen hat
    {
        endSetPossible = false;
        QMessageBox::information(this,"Persönliche Einheiten setzen","Sie haben noch Spielereinheiten. Diese müssen zuerst gesetzt werden", QMessageBox::Ok);
    }
    if(endSetPossible)
    {
        for(auto area : m_areaManager.getAreas()) //Prüfen ob in einer der Areas noch Einheiten zu setzen sind
        {
            if(area->getOwner()==m_actualPlayer)
            {
                if(area->getUnitsToSet()>0)
                {
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("Area Einheiten");
                    msgBox.setText("Sie haben in Area "+ area->getBaseCountry()->getName() + " noch nicht gesetzte Einheiten. Wollen Sie trotzdem fortfahren?");
                    msgBox.addButton("Ja",QMessageBox::YesRole);
                    msgBox.addButton("Nein",QMessageBox::NoRole);
                    if(msgBox.exec() == QMessageBox::RejectRole)
                    {
                        endSetPossible = false;
                        break;
                    }
                }
            }
        }
    }
    if(endSetPossible)
    {
        m_gameSate = main;
        setPBToGameState();
        m_map->setMapForSelection(m_actualPlayer);
    }
}

void CGame::endTurn()
{
    //Einheiten Stati wieder auf auf normal setzen
    for(auto country : m_map->getMap())
    {
        if (country->getOwner() == m_actualPlayer)
        {
            country->setAttackStatus(true);
            country->setMoveStatus(true);
        }
    }
    //Map auf Spielstatus anpassen
    m_map->setMapBlind();
    m_gameSate = waitForNewPlayer;
    setPBToGameState();
}

void CGame::changeCards()
{
    CCardChangeMenu* cardMenu = new CCardChangeMenu(m_actualPlayer);
    cardMenu->exec();
    delete cardMenu;
}

void CGame::openAreaMenu(CArea *area)
{
    m_actualArea = area;
    m_map->setMapToAreaView(area);
    ui->mapView->centerOn(m_actualArea->getBaseCountry()->getIconPos());
    CAreaMenu* areaMenu = new CAreaMenu(this,area,this);
    areaMenu->exec();
    setPBToGameState();
    delete areaMenu;
}

void CGame::quitAction()
{
    switch(m_gameSate)
    {
    case areaSet:
        m_unitsForAction=0;
        m_actualArea=nullptr;
        m_map->setMapToGeneralView();
        m_map->setMapForSelection(m_actualPlayer);
        m_gameSate=generalSet;
        setPBToGameState();
        break;
    case attack:
    case stratMovement:
    case tactMovement:
        m_unitsForAction=0;
        m_map->setMapToGeneralView();
        m_map->setMapForSelection(m_actualPlayer);
        m_gameSate=main;
        setPBToGameState();
        break;
    default:
        break;
    }
}

void CGame::playerMenu(CPlayer *player)
{
    CPlayerMenu* playerMenu =  new CPlayerMenu(m_map->getAmountOfCountrys(),player,this);
    playerMenu->exec();
    delete playerMenu;
}

void CGame::endGame()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Partie Beenden");
    msgBox.setText("Wollen Sie die Partie tatsächlich auswerten und beenden?");
    msgBox.addButton("Ja",QMessageBox::YesRole);
    msgBox.addButton("Nein",QMessageBox::NoRole);
    if(msgBox.exec() == QMessageBox::AcceptRole) //Wenn unser auf Ja klickt
    {
        //Spielerattribute aktualisieren
        for(auto player : m_players)
        {
            if(player->getActiveInGame()) //nur wenn Spieler noch aktiv im Spiel ist
            {
                //Game Over Runde
                player->setGameOverTurn(m_actualRound);
                //Anzahl Einheiten
                int units = 0;
                for(auto country : m_map->getMap())
                {
                    if(country->getOwner() == player)
                    {                        
                        units+=country->getUnits();
                    }
                }
                player->setTotalNumberOfUnits(units);
            }
        }
        CGameOver* gameOver = new CGameOver(m_players);
        gameOver->exec();
        this->close();
    }
}

void CGame::quitGame()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Spiel beenden");
    msgBox.setText("Wollen Sie das Spiel tatsächlich beenden? Nicht gespeicherte Spielzüge gehen verloren!");
    msgBox.addButton("Ja",QMessageBox::YesRole);
    msgBox.addButton("Nein",QMessageBox::NoRole);
    if(msgBox.exec() == QMessageBox::AcceptRole) //Wenn unser auf Ja klickt
        this->close();
}

void CGame::saveGame()
{
    //Verzeichniss und Dateinamen auswählen
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save Datei"),"/home/NewSaveGame", tr("Text Dateien (*.txt)"));
    if (!fileName.isEmpty())
    {
        QFile saveFile(fileName);
        if (saveFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        {
            QTextStream out (&saveFile);
            //Spieldaten selbst schreiben
            out << "<GameStart>\n" << *this << "<GameEnd>\n";
            //Spielerdaten schreiben
            out << "<PlayerStart>\n";
            for (auto player : m_players)
            {
                out << "Player: ";
                out << *player;
            }
            out << "<PlayerEnd>\n";
            //Länderdaten schreiben
            out << "<CountryStart>\n";
            for (auto country : m_map->getMap())
            {
                out << "Country: ";
                out << *country;
            }
            out << "<CountryEnd>\n";
        }
    }
}

void CGame::setInitCountry()
{
    //Anpassen der CCountry-Attribute des gewählten Landes
    m_sourceCountry->setOwner(m_actualPlayer);
    m_sourceCountry->addUnits(20);
    m_sourceCountry->setAttackStatus(true);
    m_sourceCountry->setMoveStatus(true);
    m_sourceCountry->setClickable(false);

    bool allDone = true; //Prüfen ob alle Länder besetzt sind
    for (auto country : m_map->getMap())
    {
        if (country->getOwner() == nullptr)
        {
            allDone = false;
            break;
        }
    }
    if (allDone) //wenn alle besetzt dann Zug-Beginnen Button aktiv gestellt und auf letzten Spieler der Liste gewechselt und Länderanzahl für Spieler berechnen
    {
        ui->startTurnButton->setEnabled(true);
        ui->instructionLabel->setText("Bitte den ersten Zug durch klicken des 'Zug Beginnen' Button starten.");
        m_actualPlayer = m_players.last();
        m_areaManager.initAreas(m_map->getMap());
        for (auto player : m_players)
        {
            player->setNumberOfCountrysHold(getNumberOfCountryOwenedByPlayer(player));
        }
    }
    else //sonst Spielerwechsel
    {
        nextPlayer();
        ui->instructionLabel->setText("Spieler " + m_actualPlayer->getName() + " ,bitte wählen Sie ihr Land durch Doppelklick aus!");
    }
}

void CGame::disableAllPB(bool status)
{
    QList<QPushButton*>buttons = this->findChildren<QPushButton*>();
    for (auto PB : buttons)
        PB->setDisabled(status);
}


void CGame::nextPlayer()
{
    bool playerFound = false;
    //Prüfen ob noch ein gültiger Spieler nach dem aktuellen Spieler kommt
    for(int i = m_actualPlayer->getID();i<m_players.size();++i)
    {
        if(m_players[i]->getActiveInGame())
        {
            playerFound = true;
            m_actualPlayer = m_players[i];
            break;
        }
    }
    //sonst den ersten gültigen Spieler von Beginn an Suchen
    if(!playerFound)
    {
        for(int i = 0;i<=m_players.size();++i)
        {
            if(m_players[i]->getActiveInGame())
            {
                m_actualPlayer = m_players[i];
                break;
            }
        }
    }
    ui->actPlayerLabel->setText("Aktueller Spieler: " + m_actualPlayer->getName());
    ui->actPlayerLabel->setStyleSheet(QString("color: %1").arg(m_actualPlayer->getColor().name()));
}

void CGame::setPBToGameState() // setzt Status der PushButtons je nach Game Status
{
    QList<QPushButton *>areaButtons = ui->areaLayout->parentWidget()->findChildren<QPushButton*>();

    switch(m_gameSate)
    {
    case generalSet:
        ui->startTurnButton->setEnabled(false);
        ui->endTurnButton->setEnabled(false);
        ui->quitActionPB->setEnabled(false);
        ui->endSetButton->setEnabled(true);
        ui->changeCardsPB->setEnabled(true);
        ui->saveButton->setEnabled(false);
        ui->endSetButton->setEnabled(true);
        ui->quitActionPB->setEnabled(false);
        ui->endGameButton->setEnabled(false);
        for(auto areaPB : areaButtons)
            areaPB->setEnabled(true);
        ui->instructionLabel->setText("Durch Doppelklick auf eines Ihrer Länder, gelangen Sie in das Menü um die Spielereinheiten zu sezten,\n "
                                      "und durch Klicken auf einen der Area-Buttons gelangen Sie in das Menü zum sezen der Area-Einheiten. ");
        break;
    case areaSet:
        ui->startTurnButton->setEnabled(false);
        ui->endTurnButton->setEnabled(false);
        ui->quitActionPB->setEnabled(false);
        ui->endSetButton->setEnabled(false);
        ui->changeCardsPB->setEnabled(false);
        ui->saveButton->setEnabled(false);
        ui->endSetButton->setEnabled(false);
        ui->quitActionPB->setEnabled(true);
        for(auto areaPB : areaButtons)
            areaPB->setEnabled(false);
        ui->instructionLabel->setText("Bitte wählen Sie das Land, in das die Einheiten gesetzt werde sollen durch Doppelcklick aus.\n"
                                      "Mit dem Button 'Aktion abbrechen' können Sie das setzen der Einheiten abbrechen.");
        break;
    case tactMovement:
    case stratMovement:
    case attack:
        ui->startTurnButton->setEnabled(false);
        ui->endTurnButton->setEnabled(false);
        ui->quitActionPB->setEnabled(false);
        ui->endSetButton->setEnabled(false);
        ui->changeCardsPB->setEnabled(false);
        ui->saveButton->setEnabled(false);
        ui->endSetButton->setEnabled(false);
        ui->quitActionPB->setEnabled(true);
        for(auto areaPB : areaButtons)
            areaPB->setEnabled(false);
        break;
    case main:
        ui->startTurnButton->setEnabled(false);
        ui->endTurnButton->setEnabled(true);
        ui->quitActionPB->setEnabled(false);
        ui->endSetButton->setEnabled(false);
        ui->changeCardsPB->setEnabled(false);
        ui->saveButton->setEnabled(false);
        ui->endSetButton->setEnabled(false);
        ui->quitActionPB->setEnabled(false);
        for(auto areaPB : areaButtons)
            areaPB->setEnabled(true);
        ui->instructionLabel->setText("Durch Doppelklick auf eines Ihrer Länder gelangen Sie in das Ländermenü.");
        break;
    case waitForNewPlayer:
        ui->countryInitPB->setEnabled(false);
        ui->startTurnButton->setEnabled(true);
        ui->endTurnButton->setEnabled(false);
        ui->quitActionPB->setEnabled(false);
        ui->endSetButton->setEnabled(false);
        ui->changeCardsPB->setEnabled(false);
        ui->saveButton->setEnabled(true);
        ui->endSetButton->setEnabled(false);
        ui->quitActionPB->setEnabled(false);
        ui->endGameButton->setEnabled(true);
        for(auto areaPB : areaButtons)
            areaPB->setEnabled(false);
        ui->instructionLabel->setText("Der nächste Spieler startet durch klicken auf 'Zug Beginnen'.");
        break;
    case countryInitial:
        ui->countryInitPB->setEnabled(false);
        ui->instructionLabel->setText("Spieler " + m_actualPlayer->getName() + " ,bitte wählen Sie ihr Land durch Doppelklick aus!");
        break;
    default:
        break;
    }
}

void CGame::setPlayerUnits() //öffent das Menü um Spielereinheiten zu setzen
{
    CSetPlayerUnitsMenu* playerUnits = new CSetPlayerUnitsMenu(m_sourceCountry,m_actualPlayer,m_map->getScene(),this);
    playerUnits->exec();
    delete playerUnits;
}

void CGame::updateAreaPB()
{
    //Bestehende AreaPB entfernen
    QList<QPushButton *>areaButtons = ui->areaLayout->parentWidget()->findChildren<QPushButton *>();
    for (auto PB : areaButtons)
    {
        ui->areaLayout->removeWidget(PB);
        delete PB;
    }
    //neue Push-Buttons erstellen
    for (auto area : m_areaManager.getAreas())
    {
        if(area->getOwner() == m_actualPlayer)
        {
            QPushButton* button = new QPushButton(this);
            button->setText(area->getBaseCountry()->getName());
            if(area->getUnitsToSet()>0) //Schrift fett, wenn Einheiten zu setzten sind
                button->setFont(QFont("Arial",9,QFont::Bold));
            else
                button->setFont(QFont("Arial",9));
            button->setSizePolicy(QSizePolicy::Expanding,button->sizePolicy().verticalPolicy());
            button->setProperty("area",QVariant::fromValue(area)); //Area* dem PB hinzufügen
            ui->areaLayout->addWidget(button,Qt::AlignTop);
            connect(button,&QPushButton::clicked,this,[area,this](){openAreaMenu(area);}); //connect mit slot funktion openAreaMenu und übergabe des pointers
        }
    }
}

void CGame::updateAreaPBFont()
{
    QList<QPushButton *>areaButtons = ui->areaLayout->parentWidget()->findChildren<QPushButton *>(); //Liste aller PushButtons im areaLayout
    for (auto PB : areaButtons)
    {
        CArea* area = qvariant_cast<CArea*>(PB->property("area")); //hinterlegte Area-Pointer aus den PushButtons holen
        if(area->getUnitsToSet()>0) //je nach Anzahl der zu setztenden Einheiten entweder fett oder normal
            PB->setFont(QFont("Arial",9,QFont::Bold));
        else
            PB->setFont(QFont("Arial",9));
    }
}

void CGame::openCountryMenu()
{
    CCountryMenu* countryMenu = new CCountryMenu(this,this);
    countryMenu->exec();
    setPBToGameState();
    delete countryMenu;
}

void CGame::openAttackMenu()
{
    CPlayer* targetCountryOwner = m_targetCountry->getOwner();
    CBattle* battle = new CBattle(m_unitsForAction,m_sourceCountry,m_targetCountry,this);
    battle->exec();
    //Auswirkungen des Kampfes erstellen
    //Wenn Angreifer gewonnen hat
    if(battle->getAttackerWin())
    {
        m_targetCountry->getOwner()->setAreaState(false); //Area State für alten Besiter auf false;
        m_targetCountry->setOwner(m_actualPlayer); //neuen Besitzer setzen
        m_targetCountry->removeUnits(m_targetCountry->getUnits()); //bestehende Einheiten auf null setzen
        m_targetCountry->addUnits(battle->getAttackerSuppressedUnits()); //Unterdrückte Einheiten in das neue Land setzen
        m_targetCountry->addUnits(battle->getAttackerActiveUnits()); //Aktive Einheiten in das neue Land setzen
        m_sourceCountry->removeUnits(m_unitsForAction); //in den Kampf gezogene einheiten, vom Basisland entfernen
        m_areaManager.renewAreaForPlayer(m_map->getMap(),m_actualPlayer); //Areas für aktiven Spieler erneuern
        updateAreaPB(); //Area Push-Buttons aktualisieren
        m_targetCountry->setAttackStatus(false);
        m_targetCountry->setMoveStatus(false);
        //Länderanzahl für Spieler aktualisieren
        targetCountryOwner->setNumberOfCountrysHold(getNumberOfCountryOwenedByPlayer(targetCountryOwner));
        m_actualPlayer->setNumberOfCountrysHold(getNumberOfCountryOwenedByPlayer(m_actualPlayer));
        //check ob Verteidier ausgeschieden
        if(targetCountryOwner->getNumberOfCountrysHold() == 0)
        {
            QMessageBox::information(this,"Spieler ausgeschieden!","Spieler "+m_targetCountry->getOwner()->getName()+ " ist ausgeschieden!");
            targetCountryOwner->setActiveInGameState(false);
            targetCountryOwner->setGameOverTurn(m_actualRound);
        }
    }
    //Wenn Verteidiger gewonnen hat
    if(battle->getDefenderWin())
    {
        m_targetCountry->removeUnits(battle->getDefenderLostUnits()); //Verlorene Einheiten des Verteidigers abziehen
        m_sourceCountry->destroyUnits(battle->getAttackerLostUnits()); //Verlorene Einheiten des Angreifers abziehen
        m_sourceCountry->setAttackStatus(false); //Angreifer kann nich noch mal angreifen
        m_sourceCountry->setMoveStatus(false); //Angreifer kann nicht noch mal ziehen
    }

    //Game Status wieder herstellen
    m_gameSate = main;
    m_map->setMapToGeneralView();
    m_map->setMapForSelection(m_actualPlayer);
    setPBToGameState();    

    //Check ob Game Over - macht nur sinn, wenn Angreifer gewonnen
    bool gameOverPossible = battle->getAttackerWin();
    delete battle;
    if(gameOverPossible)
    {
        if((m_actualPlayer->getNumberOfCountrysHold()*100)/m_map->getAmountOfCountrys() > 75)
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Partie Beenden");
            msgBox.setText("Spieler " +m_actualPlayer->getName() +" hat 75% der Länder erobert. Wollen Sie das Spiel auswerten und beenden?");
            msgBox.addButton("Ja",QMessageBox::YesRole);
            msgBox.addButton("Nein",QMessageBox::NoRole);
            if(msgBox.exec() == QMessageBox::AcceptRole) //Wenn unser auf Ja klickt
            {
                //Spielerattribute aktualisieren
                for(auto player : m_players)
                {
                    if(player->getActiveInGame()) //nur wenn Spieler noch aktiv im Spiel ist
                    {
                        //Game Over Runde
                        player->setGameOverTurn(m_actualRound);
                        //Anzahl Einheiten
                        int units = 0;
                        for(auto country : m_map->getMap())
                        {
                            if(country->getOwner() == player)
                            {
                                units+=country->getUnits();
                            }
                        }
                        player->setTotalNumberOfUnits(units);
                    }
                }
                CGameOver* gameOver = new CGameOver(m_players);
                gameOver->exec();
                this->close();
            }
        }
    }
 }

void CGame::createPlayerPB() //PlayerButtons erstellen und verbinden
{
    for(auto player : m_players)
    {
        QPushButton* button = new QPushButton(this);
        button->setText(player->getName());
        button->setSizePolicy(QSizePolicy::Expanding,button->sizePolicy().verticalPolicy());
        button->setStyleSheet(QString("color: %1").arg(player->getColor().name()));
        button->setProperty("player",QVariant::fromValue(player)); //dem PushButton den Player* hinzufügen
        ui->playerLayout->addWidget(button,Qt::AlignTop); //Button dem Layout hinzufügen
        connect(button, &QPushButton::clicked,this,[player,this](){playerMenu(player);}); //connect mit playerMenu
    }
}

QTextStream& operator<<(QTextStream& out, const CGame& game)
{
    out << game.m_actualPlayer->getID() << "|" << game.m_actualRound << ";\n";
    return out;
}
