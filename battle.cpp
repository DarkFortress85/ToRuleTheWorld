#include "battle.h"
#include "ui_battle.h"
#include <QMessageBox>

//Zufallszahlengenerator
QRandomGenerator CBattle::m_diceGenerator(QRandomGenerator::securelySeeded());

CBattle::CBattle(int attackUnits, CCountry* sourceCountry, CCountry* targetCountry, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CBattle), m_sourceCountry(sourceCountry), m_targetCountry(targetCountry), m_attackingUnitsTotal(attackUnits)
{
    ui->setupUi(this);
    this->setWindowTitle("Angriff!");
    //Einheitenwerte initialisieren
    m_attackingUnitsActive = m_attackingUnitsTotal;
    m_defendingUnitsTotal = m_targetCountry->getUnits();
    m_defendingUnitsActive = m_defendingUnitsTotal;


    //Labels initialisieren
    ui->headlineLabel->setText("Kampf um " + m_targetCountry->getName());
    updateBattleValueLabels();
    updateWaveValueLabels();


    //PushButtons deaktivieren
    ui->battleAcceptPB->setEnabled(false);


    //Connect der Public Slots
    QObject::connect(ui->assaultAttackPB,SIGNAL(clicked()),SLOT(assaultAttack()));
    QObject::connect(ui->reconAttackPB,SIGNAL(clicked()),SLOT(reconAttack()));
    QObject::connect(ui->normalAttackPB,SIGNAL(clicked()),SLOT(normalAttack()));
    QObject::connect(ui->retreatPB,SIGNAL(clicked()),SLOT(retreat()));
    QObject::connect(ui->battleAcceptPB,SIGNAL(clicked()),SLOT(waveAccept()));

}

CBattle::~CBattle()
{
    //Pointer auf nllptr stellen
    m_sourceCountry = nullptr;
    m_targetCountry = nullptr;
    delete ui;
}

void CBattle::retreat()
{
    //Verteidiger gewinnt
    m_defenderWin = true;
    //10% der noch aktiven Einheiten gehen verloren
    int LostForRetreat = m_attackingUnitsActive / 10;
    m_attackingUnitsLost += LostForRetreat;
    m_attackingUnitsActive -= LostForRetreat;
    //25% der unterdrückten Einheiten gehen verloren
    LostForRetreat = m_attackingUnitsSuppressed / 4;
    m_attackingUnitsLost += LostForRetreat;
    m_attackingUnitsSuppressed -= LostForRetreat;
    //Update Labels
    updateBattleValueLabels();
    //PushButtons richtig stellen
    setPBToStandard();
    //Text des Push-Buttons
    ui->battleAcceptPB->setText("Diesmal ziehen wir uns zurück!!");
}

void CBattle::assaultAttack()
{
    //PushButton richtig stellen
    setPBToStandard();

    //Einheiten ermitteln
    getUnitsForWave(50);

    //Würfel erstellen
    createDice(1,1,10,1,3,8);
    //Angreiferbonus bei Überzahl ermitteln
    double potenz = 0.33;
    double bonusBase = m_attackingWaveTotal / m_defendingWaveTotal;
    m_attBonus = pow(bonusBase,potenz);
    //Gewinner ermitteln
    bool attackerWonWave = false;
    if(m_attackerDice.last()*m_attBonus > m_defenderDice.last()) //Da Würfel sortiert, ist der letzte der größte
        attackerWonWave = true;

    //Verluste Berechnen
    int deviation = m_diceGenerator.bounded(-25,26);
    if (attackerWonWave) //Anreifer hat gewonnen
    {
        m_attackingWaveActive = m_attackingWaveTotal; //Angreifer hat keine Verluste
        m_defendingWaveLost = m_defendingWaveTotal * (75+deviation) / 100; //Bei Sturmangriff standartVerlustrate 75% mit Abweichung +/- 25%
        m_defendingWaveSuppressed = m_defendingWaveTotal - m_defendingWaveLost; //der Rest ist unterdrückt;
        ui->battleAcceptPB->setText("Ein glorreicher Sieg!");
    }
    else //Verteidiger hat gewonnen --> Spiegelverkehrt zu Angreifer hat gewonnen
    {
        m_defendingWaveAcitve = m_defendingWaveTotal;
        m_attackingWaveLost = m_defendingWaveTotal * (75+deviation) / 100; //Angreifer kann nur in Höhe der Anzahl der Verteidiger verlieren
        m_attackingWaveSuppressed = m_defendingWaveTotal - m_attackingWaveLost;
        m_attackingWaveActive = m_attackingWaveTotal - (m_attackingWaveSuppressed + m_attackingWaveLost);
        ui->battleAcceptPB->setText("..das musste in Kauf genommen werden...");
    }

    //Labels aktualisieren
    updateWaveValueLabels();
}

void CBattle::normalAttack()
{
    //PushButton richtig stellen
    setPBToStandard();

    //Einheiten ermitteln
    getUnitsForWave(25);

    //Würfel erstellen
    createDice(1,1,7,1,3,8);

    //Angreiferbonus bei Überzahl ermitteln
    double potenz = 0.33;
    double bonusBase = m_attackingWaveTotal / m_defendingWaveTotal;
    m_attBonus = pow(bonusBase,potenz);
    //Gewinner ermitteln
    bool attackerWonWave = false;
    if(m_attackerDice.last()*m_attBonus > m_defenderDice.last()) //Da Würfel sortiert, ist der letzte der größte
        attackerWonWave = true;

    //Verluste Berechnen
    int deviation = m_diceGenerator.bounded(-25,26);
    if (attackerWonWave) //Anreifer hat gewonnen
    {
        m_attackingWaveActive = m_attackingWaveTotal; //Angreifer hat keine Verluste
        m_defendingWaveLost = m_defendingWaveTotal * (50+deviation) / 100; //Bei Sturmangriff standartVerlustrate 50% mit Abweichung +/- 25%
        m_defendingWaveSuppressed = m_defendingWaveTotal - m_defendingWaveLost; //der Rest ist unterdrückt;
        ui->battleAcceptPB->setText("Ein glorreicher Sieg!");
    }
    else //Verteidiger hat gewonnen --> Spiegelverkehrt zu Angreifer hat gewonnen
    {
        m_defendingWaveAcitve = m_defendingWaveTotal;
        m_attackingWaveLost = m_defendingWaveTotal * (50+deviation) / 100; //Angreifer kann nur in Höhe der Anzahl der Verteidiger verlieren
        m_attackingWaveSuppressed = m_defendingWaveTotal - m_attackingWaveLost;
        m_attackingWaveActive = m_attackingWaveTotal - (m_attackingWaveSuppressed + m_attackingWaveLost);
        ui->battleAcceptPB->setText("..das musste in Kauf genommen werden...");
    }

    //Labels aktualisieren
    updateWaveValueLabels();
}

void CBattle::reconAttack()
{
    //PushButton richtig stellen
    setPBToStandard();

    //Einheiten ermitteln
    getUnitsForWave(10);

    //Würfel erstellen
    createDice(1,1,5,1,2,7);

    //Angreiferbonus bei Überzahl ermitteln
    double potenz = 0.33;
    double bonusBase = m_attackingWaveTotal / m_defendingWaveTotal;
    m_attBonus = pow(bonusBase,potenz);
    //Gewinner ermitteln
    bool attackerWonWave = false;
    if(m_attackerDice.last()*m_attBonus > m_defenderDice.last()) //Da Würfel sortiert, ist der letzte der größte
        attackerWonWave = true;

    //Verluste Berechnen
    int deviation = m_diceGenerator.bounded(0,26);
    if (attackerWonWave) //Anreifer hat gewonnen
    {
        m_attackingWaveActive = m_attackingWaveTotal; //Angreifer hat keine Verluste
        m_defendingWaveLost = m_defendingWaveTotal * (75+deviation) / 100; //Bei Hinterhalt, sehr hohe Verlustrate für Verteidiger, wenn erfolgreich - da eher geringe Erfolgschancen für Angreifer
        m_defendingWaveSuppressed = m_defendingWaveTotal - m_defendingWaveLost; //der Rest ist unterdrückt;
        ui->battleAcceptPB->setText("Ein glorreicher Sieg!");
    }
    else //Verteidiger hat gewonnen --> Spiegelverkehrt zu Angreifer hat gewonnen
    {
        m_defendingWaveAcitve = m_defendingWaveTotal;
        m_attackingWaveLost = m_defendingWaveTotal * deviation / 100; //Angreifer kann nur in Höhe der Anzahl der Verteidiger verlieren - wenn Verteidiger erfolgreich, dann eher geringe Verlustquote bei Angreifer
        m_attackingWaveSuppressed = m_defendingWaveTotal - m_attackingWaveLost;
        m_attackingWaveActive = m_attackingWaveTotal - (m_attackingWaveSuppressed + m_attackingWaveLost);
        ui->battleAcceptPB->setText("..das musste in Kauf genommen werden...");
    }

    //Labels aktualisieren
    updateWaveValueLabels();
}

void CBattle::waveAccept()
{
    //Pusbutton für 2 Zwecke - entweder Akzeptieren einer Welle wenn noch nichts entschieden (erstes if)
    //oder Beenden des Dialoges wenn Schlacht vorbei
    if(!m_attackerWin && !m_defenderWin) //wenn noch nichts entschieden
    {
        //Werte von Wave in Total übertragen
        //Angreifer
        m_attackingUnitsSuppressed += m_attackingWaveSuppressed;
        m_attackingWaveSuppressed = 0;
        m_attackingUnitsLost += m_attackingWaveLost;
        m_attackingWaveLost = 0;
        m_attackingWaveTotal = 0;
        m_attackingWaveActive = 0;
        m_attackingUnitsActive = m_attackingUnitsTotal - (m_attackingUnitsSuppressed + m_attackingUnitsLost);
        //Verteidiger
        m_defendingUnitsSuppressed += m_defendingWaveSuppressed;
        m_defendingWaveSuppressed = 0;
        m_defendingUnitsLost += m_defendingWaveLost;
        m_defendingWaveLost = 0;
        m_defendingWaveTotal = 0;
        m_defendingWaveAcitve = 0;
        m_defendingUnitsActive = m_defendingUnitsTotal - (m_defendingUnitsSuppressed + m_defendingUnitsLost);

        //Würfel aufräumen
        m_defenderDice.clear();
        m_attackerDice.clear();
        for(auto label : ui->attackerDiceLayout->parentWidget()->findChildren<QLabel*>()) //Anfreiferwürfel
        {
            ui->attackerDiceLayout->removeWidget(label);
            delete label;
        }

        for(auto label : ui->defenderDiceLayout->parentWidget()->findChildren<QLabel*>()) //Verteidigerwürfel
        {
            ui->defenderDiceLayout->removeWidget(label);
            delete label;
        }


        //check ob wer gewonnen hat
        if(m_defendingUnitsActive < 1) //Verteidiger hat keine aktiven Einheiten mehr -> Angreifer gewinnt
        {
            m_attackerWin = true;
            ui->battleAcceptPB->setText("Das Land ist unser!!");
            QMessageBox::information(this,"Sieg","Wir haben gewonnen!",QMessageBox::Ok);
        }
        if(m_attackingUnitsActive < 1 || (m_wave==5 && m_defendingUnitsActive > 0)) //Angreifer hat keine Aktiven einheiten mehr oder schon in der 5ten Welle und Verteidiger hat noch Einheiten
        {                                                                          //-->Verteidiger gewinnt
            m_defenderWin = true;
            ui->battleAcceptPB->setText("Wir müssen uns zurückziehen...");
            QMessageBox::information(this,"Rückzug","Rückzug!! Wir haben verloren!",QMessageBox::Ok);
        }

        //PushButtons richtig stellen und ggf. Wellenzähler weiterstellen
        //Wenn es noch weiter geht
        if(!m_attackerWin && !m_defenderWin)
        {
            ui->battleAcceptPB->setEnabled(false);
            ui->assaultAttackPB->setEnabled(true);
            ui->reconAttackPB->setEnabled(true);
            ui->normalAttackPB->setEnabled(true);
            ui->retreatPB->setEnabled(true);
            ++m_wave;
        }
        else //wenn es nicht mehr weiter geht
        {
            setPBToStandard();
        }
        //Labels aktualisieren
        updateWaveValueLabels();
        updateBattleValueLabels();
    }
    else //zur Bestätigung der Schlacht
    {
        this->close();
    }
}

void CBattle::createDice(int numAttDice, int AttDiceRangeMin, int AttDiceRangeMax, int numDefDice, int DefDiceRangeMin, int DefDiceRangeMax)
{
    //Angreiferwürfel erstellen und dem Layout zuordnen
    for (int i = 0;i<numAttDice;++i)
    {
        int dice = m_diceGenerator.bounded(AttDiceRangeMin,AttDiceRangeMax+1);
        m_attackerDice.append(dice);
    }
    std::sort(m_attackerDice.begin(),m_attackerDice.end());

    for (auto dice : m_attackerDice)
    {
        QLabel* attDiceLabel = new QLabel(QString::number(dice));
        attDiceLabel->setAlignment(Qt::AlignCenter);
        attDiceLabel->setFrameStyle(QFrame::Box);
        attDiceLabel->setFont(QFont(attDiceLabel->font().family(),50));
        attDiceLabel->setFixedSize(80,80);
        ui->attackerDiceLayout->addWidget(attDiceLabel);
    }

    //Verteidigerwürfel erstellen und dem Layout zuordnen
    for (int i = 0;i<numDefDice;++i)
    {
        int dice = m_diceGenerator.bounded(DefDiceRangeMin,DefDiceRangeMax);
        m_defenderDice.append(dice);
    }
    std::sort(m_defenderDice.begin(),m_defenderDice.end());

    for (auto dice : m_defenderDice)
    {
        QLabel* defDiceLabel = new QLabel(QString::number(dice));
        defDiceLabel->setAlignment(Qt::AlignCenter);
        defDiceLabel->setFrameStyle(QFrame::Box);
        defDiceLabel->setFont(QFont(defDiceLabel->font().family(),50));
        defDiceLabel->setFixedSize(80,80);
        ui->defenderDiceLayout->addWidget(defDiceLabel);
    }
}

void CBattle::updateBattleValueLabels() //Labelwerte für Schlacht aktualisieren
{
    ui->attackerUnitsTotalValue->setNum(m_attackingUnitsTotal);
    ui->defenderUnitsTotalValue->setNum(m_defendingUnitsTotal);
    ui->attackerUnitsAcitveValue->setNum(m_attackingUnitsActive);
    ui->defenderUnitsActiveValue->setNum(m_defendingUnitsActive);
    ui->attackerUnitsSuppressedValue->setNum(m_attackingUnitsSuppressed);
    ui->defenderUnitsSuppressedValue->setNum(m_defendingUnitsSuppressed);
    ui->attackerUnitsLostValue->setNum(m_attackingUnitsLost);
    ui->defenderUnitsLostValue->setNum(m_defendingUnitsLost);
    ui->waveValue->setNum(m_wave);
}

void CBattle::updateWaveValueLabels() //Labelwerte für Welle aktualisieren
{
    ui->attackerWaveTotalValue->setNum(m_attackingWaveTotal);
    ui->defenderWaveTotalValue->setNum(m_defendingWaveTotal);
    ui->attackerWaveAcitveValue->setNum(m_attackingWaveActive);
    ui->defenderWaveAcitveValue->setNum(m_defendingWaveAcitve);
    ui->attackerWaveSuppressedValue->setNum(m_attackingWaveSuppressed);
    ui->defenderWaveSuppressedValue->setNum(m_defendingWaveSuppressed);
    ui->attackerWaveLostValue->setNum(m_attackingWaveLost);
    ui->defenderWaveLostValue->setNum(m_defendingWaveLost);
    ui->bonusValue->setNum(m_attBonus);
    ui->waveValue->setNum(m_wave);
}

void CBattle::getUnitsForWave(int percentage)
{
    //Anzahl angreifender Einheiten ermitteln
    if (m_attackingUnitsTotal * percentage/100 < m_attackingUnitsActive) //Wenn die grundsätzliche Anzahl (Total * %) nicht mehr erreicht werden kann die noch Aktiven nehmen
        m_attackingWaveTotal = m_attackingUnitsTotal * percentage / 100;
    else
        m_attackingWaveTotal = m_attackingUnitsActive;

    if(m_attackingWaveTotal == 0) //Wenn durch Berechnung mit % Null herauskommt, dann mind. 1 Einheit
        m_attackingWaveTotal =1;

    //Anzahl verteidigener Einheiten ermitteln
    if (m_defendingUnitsActive > m_attackingWaveTotal) //Wenn noch genügend aktive Verteidiger vorhanden, dann so viele wie Angreifer, sonst die übrigen
        m_defendingWaveTotal = m_attackingWaveTotal;
    else
        m_defendingWaveTotal = m_defendingUnitsActive;

}

void CBattle::setPBToStandard()
{
    ui->battleAcceptPB->setEnabled(true);
    ui->assaultAttackPB->setEnabled(false);
    ui->reconAttackPB->setEnabled(false);
    ui->normalAttackPB->setEnabled(false);
    ui->retreatPB->setEnabled(false);
}
