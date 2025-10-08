#include "GameManager.h"
#include "LogManager.h"
#include "WorldManager.h"
#include "ResourceManager.h"
#include "EventKeyboard.h"
#include "vs-2022/Gate.h"
#include "vs-2022/Visitor.h"
#include <iostream>
#include "ObjectList.h"

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
	auto* v1 = new Visitor(VisitorKind::GOOD, gate, "good_visitor", 0.30f);
	auto* v2 = new Visitor(VisitorKind::EVIL, gate, "evil_visitor", 0.45f);
	auto* v3 = new Visitor(VisitorKind::WIZARD, gate, "wizard_visitor", 0.65f);

	const df::Box& view = WM.getView();
	float leftX = view.getCorner().getX() + 2;

	v1->setPosition({ leftX, laneY });
	v2->setPosition({ leftX, laneY });
	v3->setPosition({ leftX, laneY + 2 }); // slight stagger so you can see them
}

void clearVisitors() {
	// Mark all visitors for deletion.
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

	auto* gate1 = new Gate();
	gate1->setPosition({ rightX, centerY - rowGap });

	auto* gate2 = new Gate();
	gate2->setPosition({ rightX, centerY });

	auto* gate3 = new Gate();
	gate3->setPosition({ rightX, centerY + rowGap });

	// Spawn per-lane visitors aligned with the gates.
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

  loadResources(); // <-- must happen before creating Gate
  populateWorld();

  LM.writeLog("Press SPACE to toggle gate.");
  GM.run();
  GM.shutDown();
  return 0;
}
