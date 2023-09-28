#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>

#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "playerinit.h"


CMainMenu::CMainMenu(CGame* game, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CMainMenu)
{
    ui->setupUi(this);
    this->setWindowTitle("To Rule The World - Hauptmenü");
    //slots Verbinden
    QObject::connect(ui->newGameButton,SIGNAL(clicked()), SLOT(newGame()));
    QObject::connect(ui->loadGameButton,SIGNAL(clicked()), SLOT(loadGame()));
    QObject::connect(ui->quitGameButton, SIGNAL(clicked()), SLOT(quitGame()));

    m_game = game;
}

CMainMenu::~CMainMenu()
{
    delete ui;
}

void CMainMenu::newGame()
{
    CPlayerInit* playerInit = new CPlayerInit(this); //Player Init-Aufrufen
    playerInit->show();
    playerInit->exec();

    if(playerInit->getPlayers().size() > 1) //mind. 2 Spieler eingetragen - Spiel wird gestartet!
    {        
        m_game->setPlayer(playerInit->getPlayers());
        m_startGame = true;
        this->close();
    }    
}

void CMainMenu::loadGame()
{
    QString file = QFileDialog::getOpenFileName(this,tr("Spiel Laden"),"",tr("Text Dateien (*.txt)")); //File vom User abfragen
    if (!file.isEmpty())
    {
        QFile loadFile(file);
        if(loadFile.open(QIODevice::ReadOnly | QIODevice::Text)) //wenn öffnen erfolgreich
        {
            QTextStream in(&loadFile); //Textstream erstellen
            if(m_game->loadGame(in)) //wenn laden der Datei möglich war
            {
                m_startGame = true;
                this->close();
            }
            else //wenn laden der Datei nicht möglich war
            {
                QMessageBox::information(this,"Laden fehlgeschlagen","Die Save-Datei konnte aufgrund eines Formatfehlers nicht geladen werden",QMessageBox::Ok);
            }
        }
        else //wenn Datei nicht geöffnet werden konnte
        {
            QMessageBox::information(this,"Öffnen fehlgeschlagen","Die Save-Datei konnte nicht geöffnet werden",QMessageBox::Ok);
        }
        loadFile.close();
    }

}

void CMainMenu::quitGame()
{
    QMessageBox::information(this, "Abbrechen", "Schade dass Sie To Rule The World veralssen", QMessageBox::Ok);
    this->close();
}

