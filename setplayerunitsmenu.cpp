#include "setplayerunitsmenu.h"
#include "ui_setplayerunitsmenu.h"

CSetPlayerUnitsMenu::CSetPlayerUnitsMenu(CCountry *country, CPlayer *player,QGraphicsScene *scene, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSetPlayerUnitsMenu)
{
    ui->setupUi(this);
    this->setWindowTitle("Spielereinheiten setzen");
    //Attribute zuweisen
    m_country = country;
    m_player = player;

    //View befüllen und auf aktuelles Land zentrieren
    ui->countryView->setScene(scene);
    ui->countryView->centerOn(m_country->getIconPos());
    ui->countryView->setInteractive(false);
    ui->countryView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->countryView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    //SpinBox mit min und max befüllen
    ui->unitsSB->setRange(0,m_player->getPlayerUnits());

    //Verfügbare Einheiten befüllen:
    ui->unitsAvailableAmountLable->setText(QString::number(m_player->getPlayerUnits(),10));

    //"Setzen" Button inaktiv stellen, und mit value changed Signal der Spin-Box verbinden (aktiv bei mehr als 0)
    ui->setPB->setDisabled(true);
    connect(ui->unitsSB, &QSpinBox::valueChanged,this,[=](int value) {ui->setPB->setEnabled(value > 0);});

    //connect der public slots:
    QObject::connect(ui->quitPB, SIGNAL(clicked()),SLOT(quit()));
    QObject::connect(ui->setPB, SIGNAL(clicked()), SLOT(setUnits()));

}


CSetPlayerUnitsMenu::~CSetPlayerUnitsMenu()
{
    m_country = nullptr;
    m_player = nullptr;
    delete ui;
}

void CSetPlayerUnitsMenu::quit()
{
    this->close();
}

void CSetPlayerUnitsMenu::setUnits()
{
    m_player->removeUnits(ui->unitsSB->value());
    m_country->createUnits(ui->unitsSB->value());
    this->close();
}
