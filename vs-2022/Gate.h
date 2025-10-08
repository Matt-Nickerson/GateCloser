#pragma once
#include "Object.h"

enum class GateState { CLOSED, OPENING, OPEN, CLOSING };

class Gate : public df::Object {
public:
	Gate(bool isOpen);
	int eventHandler(const df::Event* e) override;
	GateState getState() const { return state; }
	df::Box getDoorBox() const { return getBox(); }


private:
	GateState state;

	int frameCount = 1;
	int lastFrame = 0;
	int prevIndex = 0;
	float keyDelayTimer = 0.0f; // seconds

	void step();
	void startOpening();
	void startClosing();
	void setSolidForState();
};
