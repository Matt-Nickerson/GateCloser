// GameOver.h

// Engine includes.
#include "ViewObject.h"

class GameOver : public df::ViewObject {
private:
    int time_to_live;
    void step();
public:
    GameOver();
    ~GameOver();
    int draw(void) override;
    int eventHandler(const df::Event* p_e) override;
};