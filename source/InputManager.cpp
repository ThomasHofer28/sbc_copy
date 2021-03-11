#include <SFML/Window/Keyboard.hpp>
#include "InputManager.h"
#include <iostream>
#include <string>
#include "Gamepad.h"
#include <SFML/System/Time.hpp>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

bool InputManager::isKeyDown(sf::Keyboard::Key key)
{
	if (mKeysPressedThisFrame.find(key) == mKeysPressedThisFrame.end())
		return false;

	return mKeysPressedThisFrame[key] == true && mKeysPressedLastFrame[key] == false;
}

bool InputManager::isKeyUp(sf::Keyboard::Key key)
{
	if (mKeysPressedThisFrame.find(key) == mKeysPressedThisFrame.end())
		return false;

	return mKeysPressedThisFrame[key] == false && mKeysPressedLastFrame[key] == true;
}

bool InputManager::isKeyPressed(sf::Keyboard::Key key)
{
	return sf::Keyboard::isKeyPressed(key);
}

bool InputManager::isKeyReleased(sf::Keyboard::Key key)
{
	return !sf::Keyboard::isKeyPressed(key);
}

void InputManager::onKeyPressed(sf::Keyboard::Key key)
{
	auto it = mKeysPressedThisFrame.find(key);
	if (it == mKeysPressedThisFrame.end())
	{
		mKeysPressedThisFrame.insert(std::make_pair(key, true));
	}
	else
	{
		it->second = true;
	}
}

void InputManager::onKeyReleased(sf::Keyboard::Key key)
{
	auto it = mKeysPressedThisFrame.find(key);
	if (it != mKeysPressedThisFrame.end()) // alt-tabbing out of application causes unregistered key lookup
	{
		it->second = false;
	}
}

void InputManager::clear()
{
	mGamepads.clear();
}

Gamepad* InputManager::addGamepad()
{
	int gamepadIdx = mGamepads.size();
	if (!sf::Joystick::isConnected(gamepadIdx))
		return nullptr;

	auto gamepad = std::make_unique<Gamepad>(gamepadIdx);
	Gamepad* ret = gamepad.get();
	mGamepads.push_back(std::move(gamepad));
	return ret;
}

void InputManager::update()
{
	for (const auto& gamepad : mGamepads)
	{
		gamepad->update();
	}
}

void InputManager::postUpdate()
{
	auto it = mKeysPressedThisFrame.begin();

	while (it != mKeysPressedThisFrame.end())
	{
		mKeysPressedLastFrame[it->first] = mKeysPressedThisFrame[it->first];
		++it;
	}

	for (const auto& gamepad : mGamepads)
	{
		gamepad->postUpdate();
	}
}
