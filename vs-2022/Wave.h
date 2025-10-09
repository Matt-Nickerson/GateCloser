//Wave.h
#include "Object.h"
#include "Gate.h"

//System
#include "vector"

class Wave : public df::Object {
	public:
		Wave(Gate* g1, Gate* g2, Gate* g3);

	private:
		Gate *m_p_g1;
		Gate *m_p_g2;
		Gate *m_p_g3;
		std::vector<Gate*> gateList;
		void threeEnemyNormalWave();
		void oneEnemyNormalWave();
		void crusherWave();
		void spawnWave();
		float speed;
};
