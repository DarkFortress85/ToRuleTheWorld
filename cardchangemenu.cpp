
#include <QMessageBox>
#include <QDebug>
#include "cardchangemenu.h"
#include "ui_cardchangemenu.h"



CCardChangeMenu::CCardChangeMenu(CPlayer *player, QWidget *parent) : QDialog(parent), ui(new Ui::CCardChangeMenu),
    m_player(player), m_playersCards(player->getCards_ptr())
{
    ui->setupUi(this);
    this->setWindowTitle("Karten-Tauschen Menü");

    //Karten Erstellen und dem Layout hinzufügen
    fillCardLayout();

    //Tauschen Button in- aktiv stellen
    ui->changePB->setEnabled(changeCardsPossible());

    //Connect der Public Slots
    QObject::connect(ui->quitPB, SIGNAL(clicked()), SLOT(quit()));
    QObject::connect(ui->changePB, SIGNAL(clicked()), SLOT(changeCards()));
}

CCardChangeMenu::~CCardChangeMenu()
{
    m_player = nullptr;
    m_playersCards = nullptr;
    delete ui;    
}

void CCardChangeMenu::changeCards()
{
    if(m_combinedCards) //wenn von jeder Karte eines da ist
    {
        for(int i = 1;i<4;++i)
            m_playersCards->removeOne(i);
    }
    else
    {
        for(int i =0;i<3;++i) //sonst 3 Karten von Wert von m_WhatToChange tauschen
            m_playersCards->removeOne(m_whatToChange);
    }
    m_playersCards->squeeze();
    std::sort(m_playersCards->begin(),m_playersCards->end());
    m_player->addUnits(m_units); //Kartein eintauschen
    QMessageBox::information(this, "Tausch erfolgt!","Es wurden " + QString::number(m_units,10) + " Einheiten erstellt!", QMessageBox::Ok);
    clearCardLayout(); //Layout neu aufbauen
    fillCardLayout();
    ui->changePB->setEnabled(changeCardsPossible());
}

void CCardChangeMenu::quit()
{
    this->close();
}

bool CCardChangeMenu::changeCardsPossible()
{
    //initialisierung der Variablen
    int ones=0,twos=0,threes=0;
    m_combinedCards = false;
    m_whatToChange = 0;
    //prüfung ob mind 3 Karten vorhanden
    if(m_playersCards->size()<3)
    {
        m_units = 0;
        ui->unitsLabel->setNum(m_units);
        return false;
    }
    //Aufteilung der Karten nach Werten
    for(auto card : *m_playersCards)
    {
        if (card==1)
            ++ones;
        if (card==2)
            ++twos;
        if (card==3)
            ++threes;
    }
    //Prüfen ob von jeder Karte mind. 1 vorhanden
    if(ones>0 && twos>0 && threes>0)
    {
        m_units = 50;
        ui->unitsLabel->setNum(m_units);
        m_combinedCards = true;
        return true;
    }
    //sonst Prüfen ob von einer Karte 3 gleiche da sind
    if(ones>2)
    {
        m_units = 10;
        ui->unitsLabel->setNum(m_units);
        m_whatToChange = 1;
        return true;
    }
    if(twos>2)
    {
        m_units = 20;
        ui->unitsLabel->setNum(m_units);
        m_whatToChange = 2;
        return true;
    }
    if(threes>2)
    {
        m_units = 30;
        ui->unitsLabel->setNum(m_units);
        m_whatToChange = 3;
        return true;
    }

    return false; //sonst false
}

void CCardChangeMenu::fillCardLayout()
{
    //Karten Erstellen und dem Layout hinzufügen
    for (auto cards : *m_playersCards)
    {
        QLabel* cardText = new QLabel(QString::number(cards,10),this);
        cardText->setFrameStyle(QFrame::Box);
        cardText->setAlignment(Qt::AlignCenter);
        cardText->setFont(QFont(cardText->font().family(),50));
        ui->cardsLayout->addWidget(cardText);
        m_cardLayouts.append(cardText);
    }
}

void CCardChangeMenu::clearCardLayout()
{
    for(auto label : m_cardLayouts)
    {
        delete label;
    }
    m_cardLayouts.clear();
}
