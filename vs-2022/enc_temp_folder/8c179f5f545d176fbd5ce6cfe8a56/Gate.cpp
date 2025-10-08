// Gate.cpp
#include "Gate.h"
#include "EventStep.h"
#include "EventKeyboard.h"
#include "ResourceManager.h"
#include "WorldManager.h"
#include <climits>

Gate::Gate() {
    setType("gate");
    setSprite("gate");                         // must match RM label
    firstFrame = 0;
    lastFrame = 3;

    frameIndex = firstFrame;
    getAnimation().setIndex(frameIndex);

    setAltitude(1);
    registerInterest(df::STEP_EVENT);
    registerInterest(df::KEYBOARD_EVENT);
}

void Gate::setOpen(bool open) {
    if (open && (state == GateState::CLOSED)) startOpening();
    else if (!open && (state == GateState::OPEN)) startClosing();
}

void Gate::startOpening() {
    state = GateState::OPENING;
    // If you were closing mid-way, just reverse direction from current frame.
}

void Gate::startClosing() {
    state = GateState::CLOSING;
}

int Gate::eventHandler(const df::Event* p_e) {
    if (p_e->getType() == df::STEP_EVENT) {
        step();
        return 1;
    }
    if (p_e->getType() == df::KEYBOARD_EVENT) {
        auto const* k = static_cast<const df::EventKeyboard*>(p_e);
        if (k->getKey() == df::Keyboard::SPACE && k->getKeyboardAction() == df::KEY_PRESSED) {
            if (state == GateState::CLOSED) startOpening();
            else if (state == GateState::OPEN) startClosing();
            return 1;
        }
    }
    return 0;
}

void Gate::step() {
    // Manual slowdown (so we can pause on endpoints).
    if (++slowdownCount < slowdown) return;
    slowdownCount = 0;

    switch (state) {
    case GateState::OPENING:
        if (frameIndex < lastFrame) {
            frameIndex++;
        }
        else {
            state = GateState::OPEN;      // reached end; now idle “open”
        }
        break;

    case GateState::CLOSING:
        if (frameIndex > firstFrame) {
            frameIndex--;
        }
        else {
            state = GateState::CLOSED;    // reached start; now idle “closed”
        }
        break;

    case GateState::OPEN:
    case GateState::CLOSED:
        // Do nothing; animation stays on the last (or first) frame.
        break;
    }

    // Tell Dragonfly which frame to draw.
    getAnimation().setIndex(frameIndex);
}

int Gate::draw() {
    // Default Object::draw() uses current animation/frame,
    // so this override is optional unless you want custom effects.
    return df::Object::draw();
}
