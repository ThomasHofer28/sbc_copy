#pragma once
#include "Component.h"

class FollowObject;

class Camera : public Component
{
public:
	void onStart() override;
	void update() override;

private:
	FollowObject* mFollow1;
	FollowObject* mFollow2;
	bool mPlayerSwitch;
};