// GameStart.h
// Engine includes.
#include "ViewObject.h"
#include "Gate.h"

class GameStart : public df::ViewObject {
private:
    void start();
    bool canPlay;
public:
    GameStart();
    bool getCanPlay();
    void setCanPlay(bool canPlay);
    int eventHandler(const df::Event* p_e) override;
    int draw(void) override;
};