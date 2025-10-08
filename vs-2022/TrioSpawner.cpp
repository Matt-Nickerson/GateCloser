// TrioSpawner.cpp
#include "TrioSpawner.h"
#include "GameManager.h"
#include "WorldManager.h"
#include "EventStep.h"
#include "ScoreSystem.h"

// Your existing function signature: spawnVisitors(Gate* gate, float speed)
extern void spawnVisitors(Gate* gate, float speed);

TrioSpawner::TrioSpawner(Gate* g1, Gate* g2, Gate* g3, int interval_ms)
    : m_g1(g1), m_g2(g2), m_g3(g3), m_interval_ms(interval_ms) {
    setType("trio_spawner");
    setSolidness(df::SPECTRAL);
    registerInterest(df::STEP_EVENT);
}

int TrioSpawner::eventHandler(const df::Event* e) {
    if (e->getType() == df::STEP_EVENT) {
        // stop spawning when out of lives
        if (ScoreSystem::get().lives() <= 0) {
            WM.markForDelete(this);
            return 1;
        }
        // accumulate time using engine frame time (ms)
        m_elapsed_ms += GM.getFrameTime();

        if (m_elapsed_ms >= m_interval_ms) {
            // spawn a trio, then reset timer
            spawnVisitors(m_g1, 5.0f);
            spawnVisitors(m_g2, 15.0f);
            spawnVisitors(m_g3, 25.0f);
            m_elapsed_ms = 0;
        }
        return 1;
    }
    return 0;
}
