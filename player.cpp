#include "player.h"

QRandomGenerator CPlayer::m_cardGenerator(QRandomGenerator::securelySeeded());

void CPlayer::createCard() //erstellt eine Karte zwischen 1 und 3
{    
    int card = m_cardGenerator.bounded(1,4);
    m_cards.append(card);
    std::sort(m_cards.begin(),m_cards.end());

}

QTextStream& operator<<(QTextStream& out, const CPlayer& player)
{
    QString colorString = QString("%1,%2,%3").arg(player.m_color.red()).arg(player.m_color.green()).arg(player.m_color.blue()); //Farbe in RGB Wert umwandeln
    QString cardString; //Karten herauslesen
    if(!player.m_cards.isEmpty()) //wenn Karten vorhanden
    {
        for (int i=0;i<player.m_cards.size()-1;++i) //wenn nicht das letzte, dann mit ',' getrennt
        {
            cardString += QString::number(player.m_cards[i]);
            cardString += ',';
        }

        cardString += QString::number(player.m_cards.last()); //für das letzte Element
    }


    out << player.m_playerID << "|" << player.m_playerName << "|" << colorString << "|" << cardString << "|" << player.m_activeInGame << "|" << player.m_playerGameOverTurn << ";\n";
    return out;
}

bool CPlayer::operator<(const CPlayer& otherPlayer) const
{    //erste Kriterium -> Game Over Turn
    if(m_playerGameOverTurn > otherPlayer.m_playerGameOverTurn)
        return true;
    //wenn Game Over Turn gleich, dann Anzahl der gehaltenen Länder
    if(m_playerGameOverTurn == otherPlayer.m_playerGameOverTurn && m_numberOfCountrysHold > otherPlayer.m_numberOfCountrysHold)
        return true;
    //wenn auch gleich, dann Anzahl der Einheiten
    if(m_playerGameOverTurn == otherPlayer.m_playerGameOverTurn && m_numberOfCountrysHold == otherPlayer.m_numberOfCountrysHold && m_totalNumberOfUnits > otherPlayer.m_totalNumberOfUnits)
        return true;
    //sonst ID (größere ID gewinnt, weil später in das Spiel eingestiegen)
    if(m_playerGameOverTurn == otherPlayer.m_playerGameOverTurn && m_numberOfCountrysHold == otherPlayer.m_numberOfCountrysHold &&
            m_totalNumberOfUnits == otherPlayer.m_totalNumberOfUnits && m_playerID > otherPlayer.m_playerID)
        return true;

    return false;
}



