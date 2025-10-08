#pragma once
#include "Object.h"
#include "Event.h"
#include "EventCollision.h"
#include "LogManager.h"
#include "Gate.h"

enum class VisitorKind { GOOD, EVIL, WIZARD };

class Visitor : public df::Object {
public:
	Visitor(VisitorKind kind, Gate* gate, const std::string& sprite_label, float speed = 0.5f);

	int eventHandler(const df::Event* e) override;

private:
	VisitorKind m_kind;
	Gate* m_gate = nullptr;
	float m_goal_x = 0.0f;      
	GateState m_prev_gate_state = GateState::CLOSED;

	void onStep();
	void onCollision(const df::EventCollision* c);

	void correct(const char* msg);
	void wrong(const char* msg);

	bool overlapsGate() const;  
};
