#include "gameover.h"
#include "ui_gameover.h"


CGameOver::CGameOver(const QList<CPlayer*>& players,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CGameOver),m_players(players)
{
    ui->setupUi(this);
    this->setWindowTitle("Game Over");
    m_ranks << "Feldmarschall" << "General" << "Oberst" << "Leutnant" << "Wachtmeister" << "Rekrut";


    //Spielerliste sortieren
    std::sort(m_players.begin(),m_players.end(),[](CPlayer* plyr1, CPlayer* plyr2) {return *plyr1 < *plyr2;});
    //Befüllen der Layouts:
    for(int i =0;i<m_players.size();++i)
    {
        QLabel* rankItem = new QLabel(this);
        rankItem->setText(m_ranks[i]);
        ui->rankLayout->addWidget(rankItem);
        QLabel* nameItem = new QLabel(this);
        nameItem ->setText(m_players[i]->getName());
        ui->nameLayout->addWidget(nameItem);
        QLabel* turnItem = new QLabel(this);
        turnItem->setText(QString::number(m_players[i]->getGameOverTurn()));
        ui->roundLayout->addWidget(turnItem);
        QLabel* countryItem = new QLabel(this);
        countryItem->setText(QString::number(m_players[i]->getNumberOfCountrysHold()));
        ui->countryLayout->addWidget(countryItem);
        QLabel* unitsItem = new QLabel(this);
        unitsItem->setText(QString::number(m_players[i]->getTotalNumberOfUnits()));
        ui->unitsLayout->addWidget(unitsItem);
    }

    //PushButton verbinden
    QObject::connect(ui->quitPB,SIGNAL(clicked()),SLOT(quit()));
}

CGameOver::~CGameOver()
{
    delete ui;
}

void CGameOver::quit()
{
    this->close();
}
