#include "Object.h"
#include "EventMouse.h"

enum class GateState { CLOSED, OPENING, OPEN, CLOSING };

class Gate : public df::Object {
public:
	Gate();

	int eventHandler(const df::Event* p_e) override;
	int draw() override; 
	void setOpen(bool open);

private:
	GateState state = GateState::CLOSED;
	int firstFrame = 0;       
	int lastFrame = 0;         // set after loading sprite
	int frameIndex = 0;        // current frame
	int slowdown = 3;          // animation speed
	int slowdownCount = 0;     // tick counter

	void step();               // advance frame based on state
	void startOpening();
	void startClosing();
};
