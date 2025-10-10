#include "GameManager.h"
#include "LogManager.h"
#include "WorldManager.h"
#include "ResourceManager.h"
#include "EventKeyboard.h"
#include "vs-2022/WaveSpawner.h"
#include "vs-2022/Gate.h"
#include "vs-2022/Visitor.h"
#include <iostream>
#include "ObjectList.h"
#include "vs-2022/ScoreSystem.h"
#include "math.h";
#include "vs-2022/GameStart.h"
#include "vs-2022/Star.h"

#include "vs-2022/GameOver.h"

void loadResources(void);
void populateWorld(void);


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

	//Populate Game World with some objects.
	populateWorld();

	//Run Game until game loop is over.
	GM.run();

	//Shut Everything Down.
	GM.shutDown();
	return 0;
}

//Populate World With Game Objects!
void populateWorld(void) {

	// Create some Stars.
	for (int i = 0; i < 32; i++)
		new Star;


	new GameStart;
}


//Load Sprites.
void loadResources(void) {
	RM.loadSprite("sprites/door_opening.txt", "gate_opening");
	RM.loadSprite("sprites/door_closing.txt", "gate_closing"); 
	RM.loadSprite("sprites/door_open_idle.txt", "gate_open_idle");
	RM.loadSprite("sprites/door_closed_idle.txt", "gate_closed_idle");
	RM.loadSprite("sprites/good_visitor.txt", "good_visitor");
	RM.loadSprite("sprites/evil_visitor.txt", "evil_visitor");
	RM.loadSprite("sprites/wizard_visitor.txt", "wizard_visitor");
	RM.loadSprite("sprites/gamestart-spr.txt", "gamestart");
	RM.loadSprite("sprites/gameover-spr.txt", "gameover");
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