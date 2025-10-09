//Wave.cppp
#include "Object.h"
#include "Event.h"
#include "Gate.h"
#include "Wave.h"
#include "Visitor.h"

//Wave Constructor.
Wave::Wave(Gate* g1, Gate* g2, Gate* g3) {

	//Initalize Gate Pointers.
	m_p_g1 = g1;
	m_p_g2 = g2;
	m_p_g3 = g3;

	//Set Type
	setType("Wave");

	spawnWave();
}


void Wave::spawnWave() {
	new Visitor(VisitorKind::GOOD, m_p_g1, 1);
	new Visitor(VisitorKind::EVIL, m_p_g2, 1);
	new Visitor(VisitorKind::WIZARD, m_p_g3, 1);
}
