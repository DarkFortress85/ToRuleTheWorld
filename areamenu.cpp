#include "areamenu.h"
#include "ui_areamenu.h"
#include "country.h"

CAreaMenu::CAreaMenu(CGame* game, CArea *area, QWidget *parent) : QDialog(parent),
  ui(new Ui::CAreaMenu),m_area(area),m_game(game)
{
    ui->setupUi(this);
    this->setWindowTitle("Areamenü");

    //View befüllen und auf BaseCountry der Area zentrieren
    ui->areaView->setScene(m_game->getMap()->getScene());
    ui->areaView->centerOn(m_area->getBaseCountry()->getIconPos());
    ui->areaView->setInteractive(false);


    //Restlichen Infos befüllen
    ui->areaSizeInfo->setText(QString::number(m_area->getMemberCountrys().size()));
    ui->unitsInAreaInfo->setText(QString::number(m_area->getUnitsInArea()));
    ui->availableUnitsInfo->setText(QString::number(m_area->getUnitsToSet()));
    ui->supplyRatioInfo->setText(QString::number(m_area->getSupplyRatio(),'f',2));
    ui->maxAddUnitsInfo->setText(QString::number(m_area->getMaxUnitSupply()-m_area->getUnitsInArea()));

    //SpinBox nur bei entsprechendem GameState anzeigen
    if(m_game->getGameState() != CGame::generalSet)
        ui->setUnitsLayout->setVisible(false);

    //SpinBox mit min und max befüllen
    ui->setUnitsSB->setRange(0,m_area->getUnitsToSet());

    //"Zielland wählen" Button inaktiv stellen, und mit value changed Signal der Spin-Box verbinden (aktiv bei mehr als 0)
    ui->setPB->setDisabled(true);
    connect(ui->setUnitsSB, &QSpinBox::valueChanged,this, [=](int value) {ui->setPB->setEnabled(value >0);});

    //Connect der PushButtons
    QObject::connect(ui->quitPB,SIGNAL(clicked()),SLOT(quit()));
    QObject::connect(ui->setPB,SIGNAL(clicked()),SLOT(selectTargetCountry()));
    QObject::connect(ui->maxValuePB,SIGNAL(clicked()),SLOT(setSpinBoxMax()));
}

void CAreaMenu::quit()
{
    m_game->getMap()->setMapToGeneralView();
    this->close();
}

void CAreaMenu::selectTargetCountry() //Game-Werte entprechend anpassen
{
    m_game->setUnitsForAction(ui->setUnitsSB->value());
    m_game->setGameState(CGame::areaSet);
    m_game->getMap()->setMapForAreaSelection(m_area);
    this->close();
}

void CAreaMenu::setSpinBoxMax()
{
    ui->setUnitsSB->setValue(m_area->getUnitsToSet());
}



CAreaMenu::~CAreaMenu()
{
    m_game = nullptr;
    m_area = nullptr;
    delete ui;
}
