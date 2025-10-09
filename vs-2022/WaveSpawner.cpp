//WaveSpawner.cpp
#include "WaveSpawner.h"
#include "EventStep.h"
#include "Wave.h"

//WaveSpawner Constructor.
WaveSpawner::WaveSpawner(Gate* g1, Gate* g2, Gate* g3){

	//Initalize Gate Pointers
	m_p_g1 = g1;
	m_p_g2 = g2;
	m_p_g3 = g3;

	//Set Type.
	setType("WaveSpawner");

	//Set Soldiness.
	setSolidness(df::SPECTRAL);

	//Step Event.
	registerInterest(df::STEP_EVENT);

	//Initalize Variables.
	initialWaveCooldown = 55;
	currentWaveCooldown = 0;
}

//WaveSpawner Step Event.
void WaveSpawner::step(void) {

	currentWaveCooldown--;
	if (currentWaveCooldown <= 0) {
		new Wave(m_p_g1, m_p_g2, m_p_g3);
		currentWaveCooldown = initialWaveCooldown;
	}
}

int WaveSpawner::eventHandler(const df::Event* e) {

	//Handle Step.
	if (e->getType() == df::STEP_EVENT) {
		step();
		return 1;
	}

	//Ignore this Event.
	return 0;
}

