#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include "Gamepad.h"

class InputManager
{
public:

	static InputManager& getInstance()
	{
		static InputManager mInstance;
		return mInstance;
	}

	~InputManager() = default;
	InputManager(const InputManager& p) = delete;
	InputManager& operator=(InputManager const&) = delete;

	bool isKeyDown(sf::Keyboard::Key);
	bool isKeyUp(sf::Keyboard::Key);
	bool isKeyPressed(sf::Keyboard::Key);
	bool isKeyReleased(sf::Keyboard::Key);
	void onKeyPressed(sf::Keyboard::Key);
	void onKeyReleased(sf::Keyboard::Key);
	void clear();
	Gamepad* addGamepad();
	void update();
	void postUpdate();

private:
	InputManager() = default;
	std::map<sf::Keyboard::Key, bool> mKeysPressedLastFrame;
	std::map<sf::Keyboard::Key, bool> mKeysPressedThisFrame;
	std::vector<std::unique_ptr<Gamepad>> mGamepads;
};