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

void loadResources() {
	RM.loadSprite("sprites/door_opening.txt", "gate_opening");
	RM.loadSprite("sprites/door_closing.txt", "gate_closing"); 
	RM.loadSprite("sprites/door_open_idle.txt", "gate_open_idle");
	RM.loadSprite("sprites/door_closed_idle.txt", "gate_closed_idle");
	RM.loadSprite("sprites/good_visitor.txt", "good_visitor");
	RM.loadSprite("sprites/evil_visitor.txt", "evil_visitor");
	RM.loadSprite("sprites/wizard_visitor.txt", "wizard_visitor");
}
void spawnVisitors(Gate* gate, float laneY) {
	// Create
	auto* good = new Visitor(VisitorKind::GOOD, gate, "good_visitor", 0.30f);
	auto* evil = new Visitor(VisitorKind::EVIL, gate, "evil_visitor", 0.45f);
	auto* wizard = new Visitor(VisitorKind::WIZARD, gate, "wizard_visitor", 0.65f);

	// Place inside the visible view, with small offsets so they don't overlap.
	const df::Box& view = WM.getView();
	const float leftX = view.getCorner().getX() + 2;

	// Same lane, staggered so they don't collide at spawn:
	good->setPosition({ leftX + 0, laneY - 1 });
	evil->setPosition({ leftX - 2, laneY + 0 });
	wizard->setPosition({ leftX - 4, laneY + 1 });  // wizard can overlap safely, but still stagger
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
	float rowGap = 8.0f;                               

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

void populateWorld() {
	createGates();
}

int main(int, char**) {
  if (GM.startUp()) { LM.writeLog("GM startUp failed"); GM.shutDown(); return 1; }
  LM.setFlush(true);

  loadResources();
  ScoreSystem::get().init(3);
  populateWorld();

  LM.writeLog("Press SPACE to toggle gate.");
  GM.run();
  GM.shutDown();
  return 0;
}
