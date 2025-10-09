//Wave.cppp
#include "Object.h"
#include "Event.h"
#include "Gate.h"
#include "Wave.h"
#include "Visitor.h"

//System includes.
#include <stdlib.h>
#include <vector>

//Wave Constructor.
Wave::Wave(Gate* g1, Gate* g2, Gate* g3) {

	//Initalize Gate Pointers.
	m_p_g1 = g1;
	m_p_g2 = g2;
	m_p_g3 = g3;

    //Gate List
    gateList = {m_p_g1, m_p_g2,m_p_g3 };

	//Set Type
	setType("Wave");

    speed = 0.5;

    spawnWave();
}

void Wave::spawnWave() {
    int randomNumber = rand() % 10;

    if (randomNumber < 3) {
        crusherWave();
    }
    else {
        threeEnemyNormalWave();
    }
}

void Wave::oneEnemyNormalWave() {
    int randEnemyType = rand() % 2;
    int randGate = rand() % 3;

    if (randEnemyType == 0) new Visitor(VisitorKind::GOOD, gateList[randGate], speed);
    if (randEnemyType == 1) new Visitor(VisitorKind::EVIL, gateList[randGate], speed);
}

void Wave::threeEnemyNormalWave() {
    bool validWave = false;
    int chanceOneEnemyWave = -1;

    while (!validWave) {
        if (chanceOneEnemyWave == 0) {
            oneEnemyNormalWave();
            validWave = true;

        }
        else {
            std::vector<int> potentialWave = {};

            chanceOneEnemyWave = rand() % 10;

            for (int i = 0; i < 3; i++) {

                int randomNumber = rand() % 15;

                if (randomNumber < 5)
                {
                    potentialWave.push_back(1);
                }
                else if (randomNumber < 10)
                {
                    potentialWave.push_back(2);
                }
                else if (randomNumber < 15)
                {
                    potentialWave.push_back(0);
                }
            }

            if (potentialWave[1] == potentialWave[2] && potentialWave[1] != 0)
            {
            }
            else if (potentialWave[1] == potentialWave[0] && potentialWave[1] != 0)
            {
            }
            else if (potentialWave[0] == 1 && potentialWave[2] == 2)
            {
            }
            else if (potentialWave[0] == 2 && potentialWave[2] == 1)
            {
            }
            else
            {
                validWave = true;
                for (int i = 0; i < 3; i++) {
                    if (potentialWave[i] == 1) new Visitor(VisitorKind::GOOD, gateList[i], speed);
                    if (potentialWave[i] == 2) new Visitor(VisitorKind::EVIL, gateList[i], speed);
                }

            }

        }
    }
}

void Wave::crusherWave()
{
    int randomNumber = rand() % 4;

    switch (randomNumber)
    {
        case 0:
            new Visitor(VisitorKind::WIZARD, m_p_g1, speed);
            break;
        case 1:
            new Visitor(VisitorKind::WIZARD, m_p_g2, speed);
            break;
        case 2:
            new Visitor(VisitorKind::WIZARD, m_p_g3, speed);
            break;
        case 3:
            new Visitor(VisitorKind::WIZARD, m_p_g1, speed);
            new Visitor(VisitorKind::WIZARD, m_p_g3, speed);
            break;
    }


}