#include "Visitor.h"
#include "WorldManager.h"
#include "ResourceManager.h"
#include "Gate.h"
#include "EventStep.h"

static bool boxOverlap(const df::Box& a, const df::Box& b) {

	// AABB overlap found online Geeks4Geeks
	df::Vector ac = a.getCorner();
	df::Vector bc = b.getCorner();
	float aL = ac.getX(), aT = ac.getY();
	float aR = aL + a.getHorizontal() - 1;
	float aB = aT + a.getVertical() - 1;

	float bL = bc.getX(), bT = bc.getY();
	float bR = bL + b.getHorizontal() - 1;
	float bB = bT + b.getVertical() - 1;

	return !(aR < bL || aL > bR || aB < bT || aT > bB);
}

Visitor::Visitor(VisitorKind kind, Gate* gate, const std::string& sprite_label, float speed) : m_kind(kind), m_gate(gate) {

	setType("visitor");
	setSprite(sprite_label);
	setAltitude(1);

	// Movement to the right.
	setVelocity(df::Vector(speed, 0));

	if (m_kind == VisitorKind::WIZARD) {
		setSolidness(df::SPECTRAL);
	}
	else {
		setSolidness(df::HARD);
	}

	if (!m_gate) {
		LM.writeLog("Visitor: NULL gate pointer!");
	}
	else {
		m_goal_x = m_gate->getPosition().getX() + 10.0f; // 10 units past gate center
	}
	registerInterest(df::STEP_EVENT);
	registerInterest(df::COLLISION_EVENT);
}