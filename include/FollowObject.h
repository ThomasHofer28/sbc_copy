#pragma once
#include "Component.h"
#include "NLTmxMap.h"

class FollowObject : public Component
{
public:
	~FollowObject() override;
	FollowObject() = delete;
	FollowObject(std::string name, sf::RenderWindow& window);
	void update() override;
	std::shared_ptr<Component> parseFromXML(NLTmxMapObject& mapObject);

private:
	sf::RenderWindow& mWindow;
	sf::View mView;
	GameObject* mTarget;
};