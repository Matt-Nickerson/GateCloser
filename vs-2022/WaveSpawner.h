//WaveSpawner.h
#include "Object.h"
#include "Event.h"
#include "Gate.h"

class WaveSpawner : public df::Object {
public:
	WaveSpawner(Gate* g1, Gate* g2, Gate* g3);
	int eventHandler(const df::Event* e) override;

private:
	Gate* m_p_g1;
	Gate* m_p_g2;
	Gate* m_p_g3;
	int initialWaveCooldown;
	int currentWaveCooldown;;
	void step(void);
};
