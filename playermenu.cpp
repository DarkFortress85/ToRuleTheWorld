#include "playermenu.h"
#include "ui_playermenu.h"

CPlayerMenu::CPlayerMenu(const int numOfMapCountrys, const CPlayer* player, QWidget *parent) : QDialog(parent), ui(new Ui::CPlayerMenu), m_player(player)
{
    ui->setupUi(this);
    this->setWindowTitle("Spielermenü");

    ui->nameInfo->setText(m_player->getName()); //Spielernamen zuweisen
    int numOfPlayerCountrys = m_player->getNumberOfCountrysHold(); //Anzahl der Länder des Spielers
    ui->numberOfCountrysInfo->setNum(numOfPlayerCountrys);
    int percentageOfMap = (numOfPlayerCountrys *100) / numOfMapCountrys; //Anteil des Spielers an der Map
    ui->percentageInfo->setNum(percentageOfMap);

    connect(ui->quitPB, SIGNAL(clicked()), SLOT(quit()));
}

CPlayerMenu::~CPlayerMenu()
{
    m_player = nullptr; //nulling-out
    delete ui;
}

void CPlayerMenu::quit()
{
    this->close();
}
