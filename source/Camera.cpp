#include "Camera.h"
#include "GameObject.h"
#include "FollowObject.h"
#include "InputManager.h"
#include <GameObjectManager.h>

void Camera::onStart()
{
	auto components = mGo->getComponents<FollowObject>();
	mFollow1 = components[0];
	mFollow2 = components[1];


}

void Camera::update()
{
	if (InputManager::getInstance().isKeyDown(sf::Keyboard::V))
	{
		mPlayerSwitch = !mPlayerSwitch;
	}

	if (mPlayerSwitch)
	{
		mFollow1->setEnabled(false);
		mFollow2->setEnabled(true);
	}
	else
	{
		mFollow1->setEnabled(true);
		mFollow2->setEnabled(false);
	}
}
