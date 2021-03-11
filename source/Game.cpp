#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "InputManager.h"
#include <iostream>
#include "GameObjectManager.h"
#include "MenuState.h"
#include "GameState.h"
#include "EndState.h"
#include "IntroState.h"
#include "InfoState.h"
#include "CreditsState.h"
#include "ContentLoader.h"
#include "RenderManager.h"
#include "StateManager.h"
#include "PhysicsManager.h"
#include "SoundManager.h"
#include <TGUI/TGUI.hpp>
#include "TimerManager.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

void Game::run()
{
    if (!init())
        return;

    while (mWindow->isOpen())
    {
        sf::Event event;

        while (mWindow->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                mWindow->close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                InputManager::getInstance().onKeyPressed(event.key.code);
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                InputManager::getInstance().onKeyReleased(event.key.code);
            }

            StateManager::getInstance().onEvent(event);
        }

        update();
        draw();
    }
   
    shutdown();
}

bool Game::init()
{
    mWindow = std::make_shared<sf::RenderWindow>();
    mWindow->create(sf::VideoMode(1920, 1080), "Satanic Bird Cult", sf::Style::Fullscreen); //sf::Style::Fullscreen

    StateManager::getInstance().setWindow(mWindow);
    auto introState = new IntroState;
    StateManager::getInstance().registerState("intro", introState);
    auto menuState = new MenuState;
    StateManager::getInstance().registerState("menu", menuState);
    auto gameState = new GameState;
    StateManager::getInstance().registerState("game", gameState);
    auto endState = new EndState;
    StateManager::getInstance().registerState("end", endState);
    auto infoState = new InfoState;
    StateManager::getInstance().registerState("info", infoState);
    auto creditsState = new CreditsState;
    StateManager::getInstance().registerState("credits", creditsState);

    StateManager::getInstance().setState("intro");
    
    return true;
}

void Game::update()
{
    if (InputManager::getInstance().isKeyDown(sf::Keyboard::Escape))
    {
        mWindow->close();
    }

	static sf::Clock clock;
	sf::Time delta = clock.restart();

    SoundManager::getInstance().update();
    InputManager::getInstance().update();
    StateManager::getInstance().getCurrentState()->update(delta);
    TimerManager::getInstance().update();
    GameObjectManager::getInstance().update(delta);
    InputManager::getInstance().postUpdate();
    PhysicsManager::getInstance().update(delta);
}

void Game::draw()
{
    mWindow->clear(sf::Color(0, 0, 0));
    RenderManager::getInstance().draw(*mWindow);
    StateManager::getInstance().getCurrentState()->draw(*mWindow);
    mWindow->display();
}

void Game::shutdown()
{
    StateManager::getInstance().getCurrentState()->exit();
    for (auto state : StateManager::getInstance().getStates())
    {
        delete state.second;
        state.second = nullptr;
    }

    GameObjectManager::getInstance().clear();
}

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    Game game;
    game.run();
    _CrtDumpMemoryLeaks();
    return 0;
}