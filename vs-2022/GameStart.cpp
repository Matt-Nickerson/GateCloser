// GameStart.cpp

// Engine includes.
#include "LogManager.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

// Game includes.
#include "GameStart.h"
#include "Gate.h"
#include "WaveSpawner.h"
#include "ScoreSystem.h"

//GameStart Constructor
GameStart::GameStart(){

    //Type.
    setType("GameStart");

    //Sprite.
    setSprite("gamestart");

    //Put in center of window.
    setLocation(df::CENTER_CENTER);

    //Register Events.
    registerInterest(df::KEYBOARD_EVENT);

    //Initalize Variables
    canPlay = false;
}

//GameStart Event Handler
int GameStart::eventHandler(const df::Event* p_e) {

    //Keyboard Event.
    if (p_e->getType() == df::KEYBOARD_EVENT) {
        df::EventKeyboard* p_keyboard_event = (df::EventKeyboard*)p_e;
        switch (p_keyboard_event->getKey()) {
        case df::Keyboard::SPACE:    //play
            start();
            break;
        case df::Keyboard::Q:   //quit
            GM.setGameOver();
            break;
        }
        return 1;
    }

    //Ignore this event.
    return 0;
}

//Set Can Play.
void GameStart::setCanPlay(bool canplay) {
    canPlay = canplay;
}

//Get Can Play.
bool GameStart::getCanPlay() {
    return canPlay;
}

// Populate Game Once Game Started.
void GameStart::start() {

    //Set Starting Lives.
    ScoreSystem::get().init(3);

    //Calulate Positions of Gates.
    const df::Box& view = WM.getView();
    float rightX = view.getCorner().getX() + view.getHorizontal() - 12;
    float centerY = view.getCorner().getY() + view.getVertical() / 2.0f;

    //Spawn Gates at Positions.
    auto* gate1 = new Gate(false, df::Vector(rightX, centerY - 8));
    auto* gate2 = new Gate(true, df::Vector(rightX, centerY));
    auto* gate3 = new Gate(false, df::Vector(rightX, centerY + 8));

    //Spawn Wave.
    new WaveSpawner(gate1, gate2, gate3);

    //Player can Play.
    canPlay = true;

    //When game starts, become inactive.
    setActive(false);
}

//Override default draw so as not to display "value".
int GameStart::draw() {
    return df::Object::draw();
}