#include "Gate.h"
#include "EventStep.h"
#include "EventKeyboard.h"
#include "ResourceManager.h"
#include "WorldManager.h"
#include "LogManager.h"
#include <chrono>

constexpr float KEY_DELAY = 0.5f;

Gate::Gate(bool isOpen) {
    setType("gate");

    // Start closed and idle
    setSprite("gate_closed_idle");
    if (isOpen) {
        state = GateState::OPEN;
    }
    else {
        state = GateState::CLOSED;
    }

    auto* spr = getAnimation().getSprite();
    frameCount = spr ? spr->getFrameCount() : 1;
    lastFrame = frameCount - 1;
    prevIndex = getAnimation().getIndex();

    registerInterest(df::STEP_EVENT);
    registerInterest(df::KEYBOARD_EVENT);
    keyDelayTimer = 0.0f;
}

void Gate::startOpening() {
    setSprite("gate_opening");
    state = GateState::OPENING;
    setSolidForState();

    // Refresh cached info for this sprite
}

void Gate::startClosing() {
    setSprite("gate_closing");
    state = GateState::CLOSING;
    setSolidForState();
}

int Gate::eventHandler(const df::Event* e) {
    if (e->getType() == df::STEP_EVENT) {
        step();
        // Decrease timer
        if (keyDelayTimer > 0.0f) {
            keyDelayTimer -= 1.0f / 30.0f;
            if (keyDelayTimer < 0.0f) keyDelayTimer = 0.0f;
        }
        return 1;
    }

    if (e->getType() == df::KEYBOARD_EVENT) {
        auto* k = static_cast<const df::EventKeyboard*>(e);
        const bool isSpace = (k->getKey() == df::Keyboard::SPACE) || (k->getKey() == ' ');
        const bool isPress = (k->getKeyboardAction() == df::KEY_PRESSED) || (k->getKeyboardAction() == df::KEY_DOWN);

        if (isSpace && isPress && keyDelayTimer <=0.0f) {
            // Only allow toggling when idle
            keyDelayTimer = 0.5f;
            if (state == GateState::CLOSED) {
                LM.writeLog("Gate: OPENING");
                startOpening();

            }
            else if (state == GateState::OPEN) {
                LM.writeLog("Gate: CLOSING");
                startClosing();
            }
            return 1;
        }
    }
    return 0;
}

void Gate::step() {
    int cur = getAnimation().getIndex();

    switch (state) {
    case GateState::OPENING:
        // Only switch to idle after the last frame has been displayed
        if (cur < lastFrame) {
            // Animation still playing
        } else {
            setSprite("gate_open_idle");
            state = GateState::OPEN;
            auto* spr = getAnimation().getSprite();
            frameCount = spr ? spr->getFrameCount() : 1;
            lastFrame = frameCount - 1;
            cur = getAnimation().getIndex();
            setSolidForState();
        }
        break;

    case GateState::CLOSING:
        if (cur < lastFrame) {
            // Animation still playing
        } else {
            setSprite("gate_closed_idle");
            state = GateState::CLOSED;
            auto* spr = getAnimation().getSprite();
            frameCount = spr ? spr->getFrameCount() : 1;
            lastFrame = frameCount - 1;
            cur = getAnimation().getIndex();
            setSolidForState();
        }
        break;

    case GateState::OPEN:
    case GateState::CLOSED:
        // Idle: nothing to do
        break;
    }

    prevIndex = cur;
}
void Gate::setSolidForState() {
    switch (state) {
    case GateState::CLOSED:
    case GateState::CLOSING:
        setSolidness(df::HARD);   // blocks
        break;
    case GateState::OPEN:
    case GateState::OPENING:
        setSolidness(df::SOFT);   // lets things pass
        break;
    }
}
