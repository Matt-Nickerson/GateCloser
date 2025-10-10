#include "Walkway.h"

Walkway::Walkway(df::Vector position) {
	
    //Set Sprite
    setSprite("walkway");

    //Set Type.
    setType("gate");

    //Set Position
    setPosition(position);

    //Solidness.
    setSolidness(df::SPECTRAL);

    //Alititude
    setAltitude(1);

}