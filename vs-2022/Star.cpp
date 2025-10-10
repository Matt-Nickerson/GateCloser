// Star.cpp

//System includes.
#include <stdlib.h>

// Engine includes.
#include "WorldManager.h"
#include "EventOut.h"
#include "DisplayManager.h"

// Game includes.
#include "Star.h"

//Star Constructor.
Star::Star() {

    //Type.
    setType("Star");

    //Solidness.
    setSolidness(df::SPECTRAL);

    //Randomize Velocity.
    setVelocity(df::Vector((float)-1.0 / (rand() % 10 + 1), 0));

    //Layer.
    setAltitude(0);

    //Randomize Position.
    df::Vector p((float)(rand() % (int)WM.getBoundary().getHorizontal()), (float)(rand() % (int)WM.getBoundary().getVertical()));
    setPosition(p);
}

//Draw Star.
int Star::draw() {
    return DM.drawCh(getPosition(), STAR_CHAR, df::MAGENTA);
}

//Star Event Handler.
int Star::eventHandler(const df::Event* p_e) {

    //Handle Out of Bounds Event.
    if (p_e->getType() == df::OUT_EVENT) {
        out();
        return 1;
    }

    //Ignore this Event.
    return 0;
}

//If Star moved off window, move back to far right.
void Star::out() {
    df::Vector p((float)(WM.getBoundary().getHorizontal() + rand() % 20), (float)(rand() % (int)WM.getBoundary().getVertical()));
    setPosition(p);
    setVelocity(df::Vector(-1.0 / (rand() % 10 + 1), 0));
}