#ifndef BATTLE_H
#define BATTLE_H

#include <QDialog>
#include <QRandomGenerator>
#include "country.h"

namespace Ui {
class CBattle;
}

class CBattle : public QDialog
{
    Q_OBJECT

private:
    static QRandomGenerator m_diceGenerator;

public:
    //De- Konstruktoren
    explicit CBattle(int attackUnits, CCountry* sourceCountry, CCountry* targetCountry, QWidget *parent = nullptr);
    ~CBattle(); //1-3-5 Regel
    CBattle(const CBattle&)=delete;
    CBattle& operator=(const CBattle) = delete;

    //getter - gibt das jeweilige Attribut zurück
    const bool getAttackerWin() const {return m_attackerWin;}
    const bool getDefenderWin() const {return m_defenderWin;}
    const int getAttackerActiveUnits() const {return m_attackingUnitsActive;}
    const int getAttackerSuppressedUnits() const {return m_attackingUnitsSuppressed;}
    const int getDefenderLostUnits() const {return m_defendingUnitsLost;}
    const int getAttackerLostUnits() const {return m_attackingUnitsLost;}

    //Methoden
public slots:
    void retreat(); //Berechnet Werte für Rückzug
    void assaultAttack(); //Berechnet einen Sturmangriff
    void normalAttack(); //Berechnet einen normalen Angriff
    void reconAttack(); //Berechnet eine Aufklärungsangriff
    void waveAccept(); //Aktualisiert die Werte nach einer Angriffswelle und schließt ggf. das Menü

private:
    //Attribute
    Ui::CBattle *ui;
    CCountry* m_sourceCountry; //Land von dem aus angegriffen wird
    CCountry* m_targetCountry; //Land das angegriffen wird
    int m_attackingUnitsTotal; //Summe angreifender Einheiten
    int m_attackingUnitsActive; //davon noch aktiv
    int m_attackingUnitsLost=0; //davon verloren
    int m_attackingUnitsSuppressed=0; //davon unterdrückt
    int m_defendingUnitsTotal; //Summe verteidigender Einheiten
    int m_defendingUnitsActive; //davon noch aktiv
    int m_defendingUnitsLost=0; //davon verloren
    int m_defendingUnitsSuppressed=0; //davon unterdrückt
    int m_attackingWaveTotal=0; //Summe angreifender Einheiten für aktuelle Angriffswelle
    int m_attackingWaveActive=0; //davon noch aktiv
    int m_attackingWaveLost=0; //davon verloren
    int m_attackingWaveSuppressed=0; //davon unterdrückt
    int m_defendingWaveTotal=0; //Summe verteidigende Einheiten für aktuelle Angriffswelle
    int m_defendingWaveAcitve=0; //davon noch aktiv
    int m_defendingWaveLost=0; //davon verloren
    int m_defendingWaveSuppressed=0; //davon unterdrückt
    QList<int>m_attackerDice; //Würfel des Angreifers
    QList<int>m_defenderDice; //Würfel des Verteidigers
    int m_wave = 1; //Wellenzähler - Schlacht wird spätestens beendet wenn X Wellen erreicht sind
    double m_attBonus = 0; //ggf. Bonus für Angreifer
    bool m_attackerWin = false; //Angreifer hat gewonnen
    bool m_defenderWin = false; //Verteidiger hat gewonnen

    //Methoden
    void createDice(int numAttDice, int AttDiceRangeMin, int AttDiceRangeMax, int numDefDice, int DefDiceRangeMin, int DefDiceRangeMax); //erstellt Würfel für Angreifer und Verteidiger
    void updateBattleValueLabels(); //schreibt die Einheitenwerte des Kampfes in die TextLabel
    void updateWaveValueLabels(); //schreibt die Einheitenwerte der Angriffswelle in die TextLabel
    void getUnitsForWave(int percentage); //Berechnet die Teilnehmenden Einheiten für eine Welle - Argument ist der %Anteil der TotalAttackingUnits für diese Welle (= max. Wert für Welle)
    void setPBToStandard(); //stellt die PushButtons auf die häufigste Konstellation

};

#endif // BATTLE_H
