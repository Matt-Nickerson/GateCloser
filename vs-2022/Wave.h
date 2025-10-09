//Wave.h
#include "Object.h"
#include "Gate.h"

class Wave : public df::Object {
	public:
		Wave(Gate* g1, Gate* g2, Gate* g3);

	private:
		Gate *m_p_g1;
		Gate *m_p_g2;
		Gate *m_p_g3;
		void spawnWave();
		void threeEnemyNormalWave();
};
