#include "..\include\CreditsState.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "SpriteRenderer.h"
#include "TimerManager.h"

void CreditsState::init(StateManager* manager, std::shared_ptr<sf::RenderWindow> window)
{
	State::init(manager, window);

    auto background = std::make_unique<GameObject>("background");
    auto renderer = std::make_unique<SpriteRenderer>("../assets/credits.png");
    background->addComponent(std::move(renderer));
    GameObjectManager::getInstance().add(std::move(background));
}

void CreditsState::exit()
{
    GameObjectManager::getInstance().clear();
}

void CreditsState::update(sf::Time delta)
{
    if (mJustOnce)
    { 
        TimerManager::getInstance().create(sf::seconds(2), [this]() {mManager->setState("menu"); });
        mJustOnce = false;
    }
}
