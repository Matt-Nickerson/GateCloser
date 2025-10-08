#pragma once
#include "Object.h"
#include "Event.h"
#include "Gate.h"

class TrioSpawner : public df::Object {
public:
	TrioSpawner(Gate* g1, Gate* g2, Gate* g3, int interval_ms = 1000);

	int eventHandler(const df::Event* e) override;

private:
	Gate* m_g1;
	Gate* m_g2;
	Gate* m_g3;
	int   m_interval_ms;
	int   m_elapsed_ms = 0;
};
