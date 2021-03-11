#pragma once
#include <SFML/System/Vector2.hpp>
#include <map>
#include <memory>
#include <vector>
#include <string>
#include <SFML/Window/Joystick.hpp>


class Gamepad
{

public:

	enum Button 
	{
		North,
		South,
		East,
		West,
		RShoulder,
		LShoulder,
		RTrigger,
		LTrigger
	};

	enum Axis 
	{
		RStickVertical,
		RStickHorizontal,
		LStickVertical,
		LStickHorizontal
	};

	enum Type
	{
		Xbox = 1118,
		Playstation = 1356
	};

	Gamepad(size_t idx);
	bool isButtonPressed(Button button);
	bool isButtonDown(Button button);
	bool isButtonUp(Button button);
	float getAxis(Axis axis);
	void update();
	void postUpdate();
	size_t getIdx();

private:
	bool isTriggerPressed(Button trigger); // only used internally
	std::map<Button, int> mButtonMap;
	std::map<Axis, sf::Joystick::Axis> mAxesMap;
	Type mType;
	size_t mIdx;
	std::map<Button, bool> mGamepadButtonsPressedThisFrame;
	std::map<Button, bool> mGamepadButtonsPressedLastFrame;
};