#include "MenuState.h"
#include "Game.h"
#include "SpriteRenderer.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include <iostream>
#include <TGUI/TGUI.hpp>
#include "Player.h"
#include "Gamepad.h"
#include "State.h"
#include "TimerManager.h"
#include "StateManager.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

void MenuState::init(StateManager* manager, std::shared_ptr<sf::RenderWindow> window) 
{
	State::init(manager, window);
    mWindow = window.get();

    mMenuBuffer = false;
    mMenuTimer = 0;

    auto background = std::make_unique<GameObject>("title");
    auto renderer = std::make_unique<SpriteRenderer>("titlenice.png");
    background->addComponent(std::move(renderer));
    background->setPosition(sf::Vector2f(680,200));
    GameObjectManager::getInstance().add(std::move(background));

    // play button
    tgui::TextBox::Ptr play = tgui::TextBox::create();
    play->setText("Play");
    play->setSize(500, 200);
    play->setTextSize(50);
    play->setPosition(900, 450);
    play->getRenderer()->setTextColor(tgui::Color(205, 8, 41, 255));
    play->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));
    play->getRenderer()->setBorderColor(tgui::Color(0, 0, 0, 0));
    mGui.add(play, "play");

    // exit button
    tgui::TextBox::Ptr exit = tgui::TextBox::create();
    exit->setText("Exit");
    exit->setSize(500, 200);
    exit->setTextSize(50);
    exit->setPosition(905, 690);
    exit->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
    exit->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));
    exit->getRenderer()->setBorderColor(tgui::Color(0, 0, 0, 0));
    mGui.add(exit, "exit");

    // info button
    tgui::TextBox::Ptr info = tgui::TextBox::create();
    info->setText("Controls & Gameplay");
    info->setSize(800, 200);
    info->setTextSize(50);
    info->setPosition(690, 570);
    info->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
    info->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));
    info->getRenderer()->setBorderColor(tgui::Color(0, 0, 0, 0));
    mGui.add(info, "info");

    // p1 connected
    tgui::TextBox::Ptr p1connect = tgui::TextBox::create();
    p1connect->setText("P1 not connected");
    p1connect->setSize(500, 200);
    p1connect->setTextSize(40);
    p1connect->setPosition(110, 850);
    p1connect->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
    p1connect->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));
    p1connect->getRenderer()->setBorderColor(tgui::Color(0, 0, 0, 0));
    mGui.add(p1connect, "p1connect");

    // p2 connected
    tgui::TextBox::Ptr p2connect = tgui::TextBox::create();
    p2connect->setText("P2 not connected");
    p2connect->setSize(500, 200);
    p2connect->setTextSize(40);
    p2connect->setPosition(1410, 850);
    p2connect->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
    p2connect->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));
    p2connect->getRenderer()->setBorderColor(tgui::Color(0, 0, 0, 0));
    mGui.add(p2connect, "p2connect");
}

void MenuState::exit()
{
    mGamepad1 = nullptr;
    mGamepad2 = nullptr;
    mWindow = nullptr;
    mMenuSwitch = true;
    mMenuBufferTimer = 2.0f;
    mMadeGamepad1 = false;
    mMadeGamepad2 = false;
    mMenuBuffer = false;
    mMenuTimer = 10;
    mGui.removeAllWidgets();
    InputManager::getInstance().clear();
    GameObjectManager::getInstance().clear();
}

void MenuState::update(sf::Time delta)
{
    mMenuBufferTimer -= delta.asSeconds();
    if (!mMenuBuffer && mMenuBufferTimer < 0)
    {
        mMenuBuffer = true;
    }

    mMenuTimer -= delta.asSeconds();
    if (mMenuTimer < 0)
    {
        mMenuTimer = 0.4f;
        mMenuSwitch = true;
    }

    // instance gamepad once, only if connection is available
    if (!mMadeGamepad1 && sf::Joystick::isConnected(0))
    {
        mGamepad1 = InputManager::getInstance().addGamepad();
        mMadeGamepad1 = true;
    }
    if (!mMadeGamepad2 && sf::Joystick::isConnected(1))
    {
        mGamepad2 = InputManager::getInstance().addGamepad();
        mMadeGamepad2 = true;
    }

    // ui menu controll
    tgui::TextBox::Ptr play = mGui.get<tgui::TextBox>("play");
    tgui::TextBox::Ptr info = mGui.get<tgui::TextBox>("info");
    tgui::TextBox::Ptr exit = mGui.get<tgui::TextBox>("exit");

    // show ui connection p1 p2
    tgui::TextBox::Ptr p1 = mGui.get<tgui::TextBox>("p1connect");
    tgui::TextBox::Ptr p2 = mGui.get<tgui::TextBox>("p2connect");

    if (sf::Joystick::isConnected(0) && sf::Joystick::isConnected(1))
    {
        p1->getRenderer()->setTextColor(tgui::Color(205, 8, 41, 255));
        p2->getRenderer()->setTextColor(tgui::Color(205, 8, 41, 255));
        p1->setText("P1 is connected");
        p2->setText("P2 is connected");
    }
    else if ((sf::Joystick::isConnected(0) && !sf::Joystick::isConnected(1)) 
        || (!sf::Joystick::isConnected(0) && sf::Joystick::isConnected(1)))
    {
        p1->getRenderer()->setTextColor(tgui::Color(205, 8, 41, 255));
        p2->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
        p1->setText("P1 is connected");
        p2->setText("P2 not connected");
    }
    else 
    {
        p1->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
        p2->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
        p1->setText("P1 not connected");
        p2->setText("P2 not connected");
    }

    // menu controlling
    float updateMenuControl = 0;

    if (sf::Joystick::isConnected(0) && mMenuSwitch)
    {
        updateMenuControl += mGamepad1->getAxis(Gamepad::Axis::LStickVertical);
    }
    if (sf::Joystick::isConnected(1) && mMenuSwitch)
    {
        updateMenuControl += mGamepad2->getAxis(Gamepad::Axis::LStickVertical);
    }

    if (mMenuSwitch)
    {
        if (updateMenuControl < -0.3f)
        {
            mMenuControl--;
            mMenuSwitch = false;
        }
        if (updateMenuControl > 0.3f)
        {
            mMenuControl++;
            mMenuSwitch = false;
        }

        if (mMenuControl == 4)
        {
            mMenuControl = 1;
        }
        if (mMenuControl == 0)
        {
            mMenuControl = 3;
        }
    }

    if (mMenuControl == 1 && mMenuBuffer)
    {
        //play
        play->getRenderer()->setTextColor(tgui::Color(205, 8, 41, 255));
        info->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
        exit->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
        if (sf::Joystick::isConnected(0) && sf::Joystick::isConnected(1) 
            && (mGamepad1->isButtonDown(Gamepad::Button::South) || mGamepad2->isButtonDown(Gamepad::Button::South)))
        {
            mMenuBuffer = false;
            mManager->setState("game");
        }
    }
    else if (mMenuControl == 2 && mMenuBuffer)
    {
        // info
        play->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
        info->getRenderer()->setTextColor(tgui::Color(205, 8, 41, 255));
        exit->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
        if ((sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 0))
            || (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 1))
            || (sf::Joystick::isConnected(1) && sf::Joystick::isButtonPressed(1, 0))
            || (sf::Joystick::isConnected(1) && sf::Joystick::isButtonPressed(1, 1)))
        {
            mMenuBuffer = false;
            mManager->setState("info");
        }
    }
    else if (mMenuControl == 3 && mMenuBuffer)
    {
        // exit
        play->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
        info->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
        exit->getRenderer()->setTextColor(tgui::Color(205, 8, 41, 255));
        if ((sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 0))
            || (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 1))
            || (sf::Joystick::isConnected(1) && sf::Joystick::isButtonPressed(1, 0))
            || (sf::Joystick::isConnected(1) && sf::Joystick::isButtonPressed(1, 1)))
        {
            mMenuBuffer = false;
            mWindow->close();
        }
    }
    
}