// GameStart.h
// 

// Engine includes.

#ifndef __GAME_START_H__
#define __GAME_START_H__


#include "ViewObject.h"

class GameStart : public df::ViewObject {
private:
    void start();
public:
    GameStart();
    int eventHandler(const df::Event* p_e) override;
    int draw(void) override;
};
#endif // __GAME_START_H__