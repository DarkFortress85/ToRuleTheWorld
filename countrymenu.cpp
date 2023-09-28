#include "countrymenu.h"
#include "ui_countrymenu.h"

CCountryMenu::CCountryMenu(CGame *game, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CCountryMenu), m_game(game)
{
    ui->setupUi(this);
    this->setWindowTitle("Ländermenü");
    m_country = m_game->getSourceCountry();

    //View befüllen und auf aktuelles Land zentrieren
    ui->countryView->setScene(m_game->getMap()->getScene());
    ui->countryView->centerOn(m_country->getIconPos());
    ui->countryView->setInteractive(false);
    ui->countryView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->countryView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    //Country Label Text setzen
    ui->countryLabel->setText("Gewähltes Land: " +m_country->getName());

    //SpinBox mit min und max befüllen
    ui->unitAmountSpinBox->setRange(0,m_country->getUnits()-1);

    //Checken welche Aktionen aufgrund der Nachbarländer möglich/sinnvoll sind
    bool attackPossible = false;
    bool tactMovePossible = false;
    bool stratMovePossible = false;

    for(auto neighbour : m_country->getNeighbourPtr()) //taktische Verlegung
    {
        if(neighbour->getOwner() == m_country->getOwner())
        {
            tactMovePossible = true;
            break;
        }
    }

    for(auto neighbour : m_country->getNeighbourPtr()) //Angriff
    {
        if(neighbour->getOwner() != m_country->getOwner())
        {
            attackPossible = true;
            break;
        }
    }

    if(m_country->getArea()->getMemberCountrys().size() > 1) //strategische Verlegung
        stratMovePossible = true;


    //Steuerungsbuttons inaktiv stellen, und mit value changed Signal der Spin-Box verbinden (aktiv bei mehr als 0) je nach Status
    ui->strategicalMovemntPB->setDisabled(true);
    ui->tacticalMovemntPB->setDisabled(true);
    ui->attackPB->setDisabled(true);     
    if(m_country->getMoveStatus() && tactMovePossible)
        connect(ui->unitAmountSpinBox,&QSpinBox::valueChanged,this,[=](int value) {ui->tacticalMovemntPB->setEnabled(value>0);});

    if(m_country->getMoveStatus() && stratMovePossible)
        connect(ui->unitAmountSpinBox,&QSpinBox::valueChanged,this,[=](int value) {ui->strategicalMovemntPB->setEnabled(value>0);});

    if(m_country->getAttackStatus() && attackPossible)
        connect(ui->unitAmountSpinBox,&QSpinBox::valueChanged,this,[=](int value) {ui->attackPB->setEnabled(value>0);});

    //connect der Public Slots
    QObject::connect(ui->strategicalMovemntPB,SIGNAL(clicked()),SLOT(stratMovement()));
    QObject::connect(ui->tacticalMovemntPB,SIGNAL(clicked()),SLOT(tactMovement()));
    QObject::connect(ui->attackPB,SIGNAL(clicked()),SLOT(attack()));
    QObject::connect(ui->quitPB,SIGNAL(clicked()),SLOT(quit()));
}

void CCountryMenu::quit()
{
    this->close();
}

void CCountryMenu::tactMovement() //CGame in den richtigen Zustand versetzen
{
    m_game->setUnitsForAction(ui->unitAmountSpinBox->value());
    m_game->getMap()->setMapToTacticalMovement(m_country);
    m_game->setGameState(CGame::tactMovement);
    QString infoText("Wählen Sie das Zielland für die TAKTISCHE Verlegung durch Doppelklick darauf aus.\n"
                     "Mit dem Button 'Aktion abbrechen' können Sie die taktische Verlegung abbrechen.");
    m_game->setInfoText(infoText);
    this->close();
}

void CCountryMenu::stratMovement() //CGame in den richtigen Zustand versetzen
{
    m_game->setUnitsForAction(ui->unitAmountSpinBox->value());
    m_game->getMap()->setMapToStrategicalMovement(m_country);
    m_game->setGameState(CGame::stratMovement);
    QString infoText("Wählen Sie das Zielland für die STRATEGISCHE Verlegung durch Doppelklick darauf aus.\n"
                     "Mit dem Button 'Aktion abbrechen' können Sie die strategische Verlegung abbrechen.");
    m_game->setInfoText(infoText);
    this->close();
}

void CCountryMenu::attack() //CGame in den richtigen Zustand versetzen
{
    m_game->setUnitsForAction(ui->unitAmountSpinBox->value());
    m_game->getMap()->setMapToAttack(m_country);
    m_game->setGameState(CGame::attack);
    QString infoText("Wählen Sie das Zielland für den ANGRIFF durch Doppelklick darauf aus.\n"
                     "Mit dem Button 'Aktion abbrechen' können Sie den Angriff abbrechen.");
    m_game->setInfoText(infoText);
    this->close();
}

CCountryMenu::~CCountryMenu()
{
    m_game = nullptr;
    m_country = nullptr;
    delete ui;
}
