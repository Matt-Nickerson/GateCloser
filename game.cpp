#include "GameManager.h"
#include "LogManager.h"
#include "WorldManager.h"
#include "ResourceManager.h"
#include "EventKeyboard.h"
#include "vs-2022/Gate.h"
#include "vs-2022/Visitor.h"
#include <iostream>
#include "ObjectList.h"
#include "vs-2022/ScoreSystem.h"
#include "math.h";
#include "vs-2022/TrioSpawner.h"


void loadResources(void);
void populateWorld(void);
void spawnWaves(void);

int main(int, char**) {

	//Start up game manager.
	if (GM.startUp()) { 
		LM.writeLog("GM startUp failed"); 
		GM.shutDown(); 
		return 1; 
	}

	//Set flush of logfile during development(when done, make false).
	LM.setFlush(true);

	//Load Game Resources.
	loadResources();

	//Start Wave Spawning.
	spawnWaves();

	//Populate Game World with some objects.
	populateWorld();

	//Run Game until game loop is over.
	GM.run();

	//Shut Everything Down.
	GM.shutDown();
	return 0;
}

void populateWorld(void) {

	//Set Starting Lives.
	ScoreSystem::get().init(3);
}


void loadResources(void) {
	RM.loadSprite("sprites/door_opening.txt", "gate_opening");
	RM.loadSprite("sprites/door_closing.txt", "gate_closing"); 
	RM.loadSprite("sprites/door_open_idle.txt", "gate_open_idle");
	RM.loadSprite("sprites/door_closed_idle.txt", "gate_closed_idle");
	RM.loadSprite("sprites/good_visitor.txt", "good_visitor");
	RM.loadSprite("sprites/evil_visitor.txt", "evil_visitor");
	RM.loadSprite("sprites/wizard_visitor.txt", "wizard_visitor");
}


void spawnVisitors(Gate* gate, float /*unused*/) {
	// Decide if this gate is the middle lane (same Y as view center).
	const df::Box& view = WM.getView();
	const float centerY = view.getCorner().getY() + view.getVertical() / 2.0f;	
	const float gateY = gate->getPosition().getY();
	const bool isMiddle = (std::fabs(gateY - centerY) < 0.5f); // small tolerance

	// Choose type: never spawn GOOD in middle lane.
	VisitorKind kind;
	if (isMiddle) {
		kind = (rand() % 2 == 0) ? VisitorKind::EVIL : VisitorKind::WIZARD;
	}
	else {
		int r = rand() % 3;
		kind = (r == 0) ? VisitorKind::GOOD
			: (r == 1) ? VisitorKind::EVIL
			: VisitorKind::WIZARD;
	}

	// Create with per-kind speed/label.
	const char* label = nullptr;
	float speed = 0.35f;
	switch (kind) {
	case VisitorKind::GOOD:   label = "good_visitor";   speed = 0.35f; break;
	case VisitorKind::EVIL:   label = "evil_visitor";   speed = 0.35f; break;
	case VisitorKind::WIZARD: label = "wizard_visitor"; speed = 0.35f; break;
	}
	Visitor* v = new Visitor(kind, gate, label, speed);

	// Position: left edge of the visible view, on the EXACT same Y as the gate.
	const float leftX = view.getCorner().getX() + 2;
	const float jitterX = -2.0f * (rand() % 3); // {0, -2, -4} to reduce stacking
	v->setPosition(df::Vector(leftX + jitterX, gateY));
}

void clearVisitors() {
	// Mark all visitors for deletion
	auto list = WM.getAllObjects(true);
	for (int i = 0; i < list.getCount(); ++i) {
		auto* obj = list[i];
		if (obj->getType() == "visitor") {
			WM.markForDelete(obj);
		}
	}
}

void createGates() {
	const df::Box& view = WM.getView();                 
	float rightX = view.getCorner().getX() + view.getHorizontal() - 12;
	float centerY = view.getCorner().getY() + view.getVertical() / 2.0f; 
	float rowGap = 3.0f;                               

	auto* gate1 = new Gate(false);
	gate1->setPosition({ rightX, centerY - rowGap });

	auto* gate2 = new Gate(true);
	gate2->setPosition({ rightX, centerY });

	auto* gate3 = new Gate(false);
	gate3->setPosition({ rightX, centerY + rowGap });

	// Spawn per-lane visitors aligned with the gates
	spawnVisitors(gate1, centerY - rowGap);
	spawnVisitors(gate2, centerY);
	spawnVisitors(gate3, centerY + rowGap);
}

void spawnWaves() {
	const df::Box& view = WM.getView();
	float rightX = view.getCorner().getX() + view.getHorizontal() - 12;
	float centerY = view.getCorner().getY() + view.getVertical() / 2.0f;
	float rowGap = 8.0f;

	auto* gate1 = new Gate(false); gate1->setPosition({ rightX, centerY - rowGap });
	auto* gate2 = new Gate(true); gate2->setPosition({ rightX, centerY });
	auto* gate3 = new Gate(false); gate3->setPosition({ rightX, centerY + rowGap });

	// Create a spawner that fires every 4000 ms
	new TrioSpawner(gate1, gate2, gate3, 4000);
}