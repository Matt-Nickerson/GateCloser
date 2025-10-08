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
	setAltitude(2);

	// Movement to the right.
	setVelocity(df::Vector(speed, 0));

	if (m_kind == VisitorKind::WIZARD) {
		setSolidness(df::SOFT);
	}
	else {
		setSolidness(df::SOFT);
	}

	if (!m_gate) {
		LM.writeLog("Visitor: NULL gate pointer!");
	}
	else {
		const df::Box& bounds = WM.getBoundary();
		m_goal_x = bounds.getCorner().getX() + bounds.getHorizontal() - 2;
	}
	registerInterest(df::STEP_EVENT);
	registerInterest(df::COLLISION_EVENT);
}

int Visitor::eventHandler(const df::Event* e) {
	if (e->getType() == df::STEP_EVENT) {
		onStep();
		return 1;
	}
	if (e->getType() == df::COLLISION_EVENT) {
		auto* c = static_cast<const df::EventCollision*>(e);
		onCollision(c);
		return 1;
	}
	return 0;
}


bool Visitor::overlapsGate() const {
	if (!m_gate) return false;
	return boxOverlap(getBox(), m_gate->getDoorBox());
}

void Visitor::onStep() {
	if (!m_gate) {
		return;
	}
	GateState gate_state = m_gate->getState();
	if ((m_prev_gate_state != GateState::CLOSING) && (gate_state == GateState::CLOSING) && overlapsGate()) {
		if (m_kind == VisitorKind::WIZARD) {
			correct("Wizard crushed by closing door (correct stop).");
		}
		else if (m_kind == VisitorKind::EVIL) {
			correct("Evil crushed by closing door (also acceptable).");
		}
		else { // GOOD
			wrong("Closed door on GOOD visitor (wrong).");
		}
		WM.markForDelete(this);
		m_prev_gate_state = gate_state;
		return;
	}

	m_prev_gate_state = gate_state;

	if (getPosition().getX() >= m_goal_x) {
		if (m_kind == VisitorKind::GOOD) {
			correct("Good let through (correct).");
		}
		else if (m_kind == VisitorKind::EVIL) {
			wrong("Evil let through (wrong).");
		}
		else { // WIZARD
			wrong("Wizard let through (wrong; should only be stopped by shutting on them).");
		}
		WM.markForDelete(this);
	}
}
void Visitor::onCollision(const df::EventCollision* c) {
	if (!m_gate) return;

	df::Object* other =
		(c->getObject1() == this) ? c->getObject2() :
		(c->getObject2() == this) ? c->getObject1() : nullptr;

	if (!other || other->getType() != "gate")
		return;

	GateState gate_state = m_gate->getState();

	// Wizard: ignore CLOSED/OPENING/OPEN collisions
	if (m_kind == VisitorKind::WIZARD) {
		if (gate_state == GateState::CLOSING && overlapsGate()) {
			// Edge case: collision while closing (treat as crush)
			correct("Wizard crushed by closing door (correct).");
			WM.markForDelete(this);
		}
		return;
	}

	// Good/Evil logic:
	switch (gate_state) {
	case GateState::CLOSED:
		if (m_kind == VisitorKind::GOOD) {
			wrong("Blocked GOOD at closed door (wrong).");
		}
		else { 
			correct("Blocked EVIL at closed door (correct).");
		}
		WM.markForDelete(this);
		break;

	case GateState::CLOSING:
		// Door shut on them
		if (m_kind == VisitorKind::GOOD) {
			wrong("Door shut on GOOD (wrong).");
		}
		else { // EVIL
			correct("Door shut on EVIL (correct).");
		}
		WM.markForDelete(this);
		break;

	case GateState::OPEN:
	case GateState::OPENING:
		// Shouldn't collide if gate is SOFT
		break;
	}
}
void Visitor::correct(const char* msg) {
	LM.writeLog("OK", msg);
}
void Visitor::wrong(const char* msg) {
	LM.writeLog("Lose a life", msg);
}
