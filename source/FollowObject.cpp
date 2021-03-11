#include "FollowObject.h"
#include "GameObjectManager.h"
#include <iostream>

FollowObject::~FollowObject()
{
	mWindow.setView(mWindow.getDefaultView());
}

FollowObject::FollowObject(std::string name, sf::RenderWindow& window) :
	mWindow(window)
{
	mView.setSize(500, 500);
	mTarget = GameObjectManager::getInstance().getObject(name);
}

void FollowObject::update()
{
	mView.setCenter(mTarget->getPosition());
	mWindow.setView(mView);
}

std::shared_ptr<Component> FollowObject::parseFromXML(NLTmxMapObject& mapObject)
{
	std::map<string, string> map = mapObject.properties;
	std::map<string, string>::iterator it;
	std::shared_ptr<FollowObject> follower;

	if ((it = map.find("target")) != map.end())
	{
		follower = std::make_shared<FollowObject>(it->second, mWindow);
	}
	
	return follower;
}
