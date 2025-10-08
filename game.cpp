#include "GameManager.h"
#include "LogManager.h"
#include "WorldManager.h"
#include "ResourceManager.h"
#include "EventKeyboard.h"
#include "vs-2022/Gate.h"

void loadResources() {
    if (RM.loadSprite("sprites/door.txt", "gate")) {
        LM.writeLog("Failed to load gates.txt");
        GM.shutDown();
        exit(1);
	}
}

int main(int, char**) {
  if (GM.startUp()) { LM.writeLog("GM startUp failed"); GM.shutDown(); return 1; }
  LM.setFlush(true);

  loadResources(); // <-- must happen before creating Gate

  Gate* gate = new Gate();
  const df::Box& bounds = WM.getBoundary();
  df::Vector center(bounds.getHorizontal() / 2.0f, bounds.getVertical() / 2.0f);
  gate->setPosition(center);

  LM.writeLog("Press SPACE to toggle gate.");
  GM.run();
  GM.shutDown();
  return 0;
}
