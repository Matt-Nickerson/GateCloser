#include "ScoreSystem.h"
#include "WorldManager.h"
#include "LogManager.h"
#include "GameOver.h"

ScoreSystem& ScoreSystem::get() {
    static ScoreSystem inst;
    return inst;
}

void ScoreSystem::init(int start_lives) {
    m_score = 0;
    m_lives = start_lives;
    m_streak = 0;
    m_mult = 1.0f;
    ensureHud();
    updateHud();
    gameOver = false;
}


int ScoreSystem::getLives() {
    return m_lives;
}

void ScoreSystem::ensureHud() {

    if (!m_voScore) {
        m_voScore = new df::ViewObject();
        m_voScore->setLocation(df::TOP_LEFT);
        m_voScore->setViewString("Score");
        m_voScore->setColor(df::YELLOW);
    }
    if (!m_voLives) {
        m_voLives = new df::ViewObject();
        m_voLives->setLocation(df::TOP_CENTER);
        m_voLives->setViewString("Lives");
        m_voLives->setColor(df::YELLOW);
    }
    if (!m_voMult) {
        m_voMult = new df::ViewObject();
        m_voMult->setLocation(df::TOP_RIGHT);
        m_voMult->setDrawValue(false);             
        m_voMult->setColor(df::YELLOW);
    }
}

void ScoreSystem::updateHud() {
    if (m_voScore) m_voScore->setValue(m_score);
    if (m_voLives) m_voLives->setValue(m_lives);
    if (m_voMult) {
		// Formatting found on stackOverflow
        char buf[32];
        snprintf(buf, sizeof(buf), "Mult x%.1f", m_mult);
        m_voMult->setViewString(buf);
    }
}

void ScoreSystem::onCorrect() {
    m_streak++;
    float target = 1.0f + MULT_STEP * m_streak;
    m_mult = (target > MULT_MAX) ? MULT_MAX : target;
}

void ScoreSystem::onWrong() {

    if (m_lives > 0) m_lives--;

    //If Lives Decreases Past Zero Call GameOver.
    if (m_lives <= 0 && !gameOver) {
        new GameOver;
        gameOver = true;
    }
    m_streak = 0;
    m_mult = 1.0f;
}

void ScoreSystem::addPoints(int base) {
    int pts = static_cast<int>(base * m_mult + 0.5f);
    m_score += pts;
}

void ScoreSystem::goodLetThrough() {
    onCorrect();
    addPoints(100);
    updateHud();
    LM.writeLog("Good let through. +%d (mult=%.1f)  score=%d lives=%d",
        100, m_mult, m_score, m_lives);
}

void ScoreSystem::evilBlocked() {
    onCorrect();
    addPoints(100);
    updateHud();
    LM.writeLog("Evil blocked. +%d (mult=%.1f)  score=%d lives=%d",
        100, m_mult, m_score, m_lives);
}

void ScoreSystem::wizardCrushed() {
    onCorrect();
    addPoints(150);
    updateHud();
    LM.writeLog("Wizard crushed. +%d (mult=%.1f)  score=%d lives=%d",
        150, m_mult, m_score, m_lives);
}

void ScoreSystem::wrongAction(const char* reason) {
    onWrong();
    updateHud();
    LM.writeLog("X %s  (lives=%d, mult=%.1f, score=%d)", reason, m_lives, m_mult, m_score);
}

void ScoreSystem::disableHud() {
    if (m_voScore) {
        WM.markForDelete(m_voScore);
        m_voScore = nullptr;
    }
    if (m_voLives) {
        WM.markForDelete(m_voLives);
        m_voLives = nullptr;
    }
    if (m_voMult) {
        WM.markForDelete(m_voMult);
        m_voMult = nullptr;
    }
}