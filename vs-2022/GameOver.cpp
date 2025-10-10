// GameOver.cpp

// Engine includes.
#include "EventStep.h"
#include "WorldManager.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "ScoreSystem.h"

// Game includes.
#include "GameOver.h"

//GameOver Constructor.
GameOver::GameOver() {

    //Link to "gameover" sprite.
    if (setSprite("gameover") == 0) time_to_live = getAnimation().getSprite()->getFrameCount() * getAnimation().getSprite()->getSlowdown();
    else time_to_live = 0;

    //Type.
    setType("GameOver");

    //Put in center of window.
    setLocation(df::CENTER_CENTER);

    //Register for Events.
    registerInterest(df::STEP_EVENT);

    // Play "explosion" sound.
    df::Sound* p_sound = RM.getSound("explosion");
    if (p_sound)
        p_sound->play();

    // Play "scream" sound.
    df::Sound* p_sound1 = RM.getSound("scream");
    if (p_sound1)
        p_sound1->play();

    //Stop Wave Spawning
    df::ObjectList object_list = WM.getAllObjects(true);
    for (int i = 0; i < object_list.getCount(); i++) {
        df::Object* p_o = object_list[i];
        if (p_o->getType() == "WaveSpawner") {
            WM.markForDelete(p_o);
        }

    }
}

//GameOver event Handler.
int GameOver::eventHandler(const df::Event* p_e) {

    //Handle Step.
    if (p_e->getType() == df::STEP_EVENT) {
        step();
        return 1;
    }

    //Ignore this event.
    return 0;
}

//Count down to end of "message"
void GameOver::step() {
    time_to_live--;
    if (time_to_live <= 0) {
        WM.markForDelete(this);
    }
}

//When object exits, indicate game over.
GameOver::~GameOver() {
    //Remove Objects, re-activate GameStart.
    df::ObjectList object_list = WM.getAllObjects(true);
    for (int i = 0; i < object_list.getCount(); i++) {
        df::Object* p_o = object_list[i];
        if (p_o->getType() == "gate" || p_o->getType() == "Wave" || p_o->getType() == "visitor" || p_o->getType() == "ViewObjects") WM.markForDelete(p_o);
        if (p_o->getType() == "GameStart") {

            //Re-enable Start Screen.
            p_o->setActive(true);

            ScoreSystem::get().disableHud();
        }

    }
}
//Override default draw so as not to display "value".
int GameOver::draw() {
    return df::Object::draw();
}