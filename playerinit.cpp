#include <QMessageBox>


#include "playerinit.h"
#include "ui_playerinit.h"

CPlayerInit::CPlayerInit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPlayerInit)
{
    ui->setupUi(this);
    this->setWindowTitle("Spielerauswahl");

    QObject::connect(ui->starteGameButton, SIGNAL(clicked()), SLOT(playerInit()));
    QObject::connect(ui->quitButton, SIGNAL(clicked()), SLOT(quitPlayerInit()));
}

CPlayerInit::~CPlayerInit()
{
    delete ui;
}

void CPlayerInit::quitPlayerInit()
{
    this->close();
}

void CPlayerInit::playerInit()
{
    //Überprüfen ob mind. 2 Spieler-Namen eingegeben wurden
    int numNames = 0;
    for (auto lineEdit : this->findChildren<QLineEdit*>())
    {
        QString name = lineEdit->text().simplified();
        if (!name.isEmpty())
            ++numNames;
    }
    //Wenn weniger als 2 Namen eingeben
    if(numNames<2)
    {
        QMessageBox::information(this, "Zu wenige Spieler", "Bitte mindestens 2 Spielernamen eingeben", QMessageBox::Ok);
    }
    else //Namen und Farben auslesen und Spieler anlegen
    {
        int id=0;
        QString name;
        QColor color;
        for (auto lineEdit : this->findChildren<QLineEdit*>())
            if(!lineEdit->text().simplified().isEmpty())
            {
                ++id; //Id festlegen
                name = lineEdit->text(); //Namen aus lineEdit auslesen
                color = lineEdit->palette().color(QPalette::Text); //Farbe auslesen
                CPlayer* new_player = new CPlayer(id,name,color); //delete im Dekonstruktor von CGame
                m_players.append(new_player);
            }
        this->close();
    }

}
