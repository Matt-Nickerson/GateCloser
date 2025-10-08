#include "Gate.h"
#include "EventStep.h"
#include "EventKeyboard.h"
#include "ResourceManager.h"
#include "WorldManager.h"
#include "LogManager.h"
#include <chrono>

// At the top of your file, add:
constexpr float KEY_DELAY = 1.5f;

Gate::Gate() {
    setType("gate");

    // Start closed & idle.
    setSprite("gate_closed_idle");
    state = GateState::CLOSED;

    // Discover current anim info safely.
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

    // Refresh cached info for this sprite.
    auto* spr = getAnimation().getSprite();
    frameCount = spr ? spr->getFrameCount() : 1;
    lastFrame = frameCount - 1;
    prevIndex = getAnimation().getIndex();
}

void Gate::startClosing() {
    setSprite("gate_closing");
    state = GateState::CLOSING;

    auto* spr = getAnimation().getSprite();
    frameCount = spr ? spr->getFrameCount() : 1;
    lastFrame = frameCount - 1;
    prevIndex = getAnimation().getIndex();
}

int Gate::eventHandler(const df::Event* e) {
    if (e->getType() == df::STEP_EVENT) {
        step();
        // Decrement timer if active
        if (keyDelayTimer > 0.0f) {
            keyDelayTimer -= 1.0f / 30.0f; // Assuming 30 FPS, adjust if needed
            if (keyDelayTimer < 0.0f) keyDelayTimer = 0.0f;
        }
        return 1;
    }

    if (e->getType() == df::KEYBOARD_EVENT) {
        auto* k = static_cast<const df::EventKeyboard*>(e);
        const bool isSpace = (k->getKey() == df::Keyboard::SPACE) || (k->getKey() == ' ');
        const bool isPress = (k->getKeyboardAction() == df::KEY_PRESSED) || (k->getKeyboardAction() == df::KEY_DOWN);

        if (isSpace && isPress && keyDelayTimer <=0.0f) {
            // Only allow toggling when idle.
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
    // Let the engine advance frames; we just watch for completion.
    int cur = getAnimation().getIndex();

    switch (state) {
    case GateState::OPENING:
        // Finish when we reach the last frame OR detect wrap-around (in case it loops).
        if (cur >= lastFrame || cur < prevIndex) {
            setSprite("gate_open_idle");
            state = GateState::OPEN;
            // Re-cache for idle (usually 1 frame).
            auto* spr = getAnimation().getSprite();
            frameCount = spr ? spr->getFrameCount() : 1;
            lastFrame = frameCount - 1;
            cur = getAnimation().getIndex();
        }
        break;

    case GateState::CLOSING:
        if (cur >= lastFrame || cur < prevIndex) {
            setSprite("gate_closed_idle");
            state = GateState::CLOSED;
            auto* spr = getAnimation().getSprite();
            frameCount = spr ? spr->getFrameCount() : 1;
            lastFrame = frameCount - 1;
            cur = getAnimation().getIndex();
        }
        break;

    case GateState::OPEN:
    case GateState::CLOSED:
        // Idle: nothing to do.
        break;
    }

    prevIndex = cur;
}
