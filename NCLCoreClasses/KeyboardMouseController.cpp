/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#include "KeyboardMouseController.h"
#include "Mouse.h"
#include "Keyboard.h"

using namespace NCL;
float	KeyboardMouseController::GetAxis(uint32_t axis) const {

	if (axis == XAxisMouse) {
		return mouse.GetRelativePosition().x;
	}
	else if (axis == YAxisMouse) {
		return mouse.GetRelativePosition().y;
	}

	return 0.0f;
}

float	KeyboardMouseController::GetButtonAnalogue(uint32_t button) const {
	return GetButton(button);
}

bool	KeyboardMouseController::GetButton(uint32_t button)  const {
	if (button == LeftMouseButton) {
		return mouse.ButtonDown(NCL::MouseButtons::Left);
	}
	if (button == RightMouseButton) {
		return mouse.ButtonDown(NCL::MouseButtons::Right);
	}
	return 0.0f;
}