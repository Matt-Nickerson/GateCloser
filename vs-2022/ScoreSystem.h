#pragma once
#include "ViewObject.h"

class ScoreSystem {
public:
	static ScoreSystem& get();


	void init(int start_lives = 3);

	// Rule outcomes
	void goodLetThrough();     
	void evilBlocked();        
	void wizardCrushed();     
	void wrongAction(const char* reason); 

	// Accessors.
	int   lives() const { return m_lives; }
	int   score() const { return m_score; }
	float multiplier() const { return m_mult; }

private:
	// state
	int   m_score = 0;
	int   m_lives = 3;
	int   m_streak = 0;  // consecutive correct
	float m_mult = 1.0f; // current multiplier

	const float MULT_STEP = 0.5f;  // +0.5 per correct
	const float MULT_MAX = 5.0f;  // cap
	// HUD
	df::ViewObject* m_voScore = nullptr;
	df::ViewObject* m_voLives = nullptr;
	df::ViewObject* m_voMult = nullptr;

	// helpers
	void addPoints(int base);
	void onCorrect();           // updates streak & multiplier
	void onWrong();             // reset streak/mult, lose life
	void updateHud();
	void ensureHud();
};
