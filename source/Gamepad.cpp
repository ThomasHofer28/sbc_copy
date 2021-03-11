#include "Gamepad.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

Gamepad::Gamepad(size_t idx)
{
	auto iden = sf::Joystick::getIdentification(idx);
	mType = static_cast<Gamepad::Type>(iden.vendorId);

	if (mType == Xbox) // xbox
	{
		mButtonMap.insert(std::make_pair<Button, int>(North, 3));
		mButtonMap.insert(std::make_pair<Button, int>(South, 0));
		mButtonMap.insert(std::make_pair<Button, int>(East, 1));
		mButtonMap.insert(std::make_pair<Button, int>(West, 2));

		//will be handled separately
		mButtonMap.insert(std::make_pair<Button, int>(LTrigger, -1)); 
		mButtonMap.insert(std::make_pair<Button, int>(RTrigger, -1));

		mAxesMap.insert(std::make_pair<Axis, sf::Joystick::Axis>(RStickHorizontal, sf::Joystick::Axis::U));
		mAxesMap.insert(std::make_pair<Axis, sf::Joystick::Axis>(RStickVertical, sf::Joystick::Axis::V));
		mAxesMap.insert(std::make_pair<Axis, sf::Joystick::Axis>(LStickHorizontal, sf::Joystick::Axis::X));
		mAxesMap.insert(std::make_pair<Axis, sf::Joystick::Axis>(LStickVertical, sf::Joystick::Axis::Y));
	}
	else if (mType == Playstation) 
	{
		mButtonMap.insert(std::make_pair<Button, int>(North, 3));
		mButtonMap.insert(std::make_pair<Button, int>(South, 1));
		mButtonMap.insert(std::make_pair<Button, int>(East, 2));
		mButtonMap.insert(std::make_pair<Button, int>(West, 0));
		mButtonMap.insert(std::make_pair<Button, int>(LTrigger, 6));
		mButtonMap.insert(std::make_pair<Button, int>(RTrigger, 7));
		mAxesMap.insert(std::make_pair<Axis, sf::Joystick::Axis>(RStickHorizontal, sf::Joystick::Axis::Z));
		mAxesMap.insert(std::make_pair<Axis, sf::Joystick::Axis>(RStickVertical, sf::Joystick::Axis::R));
		mAxesMap.insert(std::make_pair<Axis, sf::Joystick::Axis>(LStickHorizontal, sf::Joystick::Axis::X));
		mAxesMap.insert(std::make_pair<Axis, sf::Joystick::Axis>(LStickVertical, sf::Joystick::Axis::Y));
	}
	else 
	{ 
		throw "vendor id not found: " + std::to_string(mType);
	}

	mIdx = idx;
}

bool Gamepad::isButtonPressed(Button button)
{
	return sf::Joystick::isButtonPressed(mIdx, mButtonMap[button]);
}

bool Gamepad::isTriggerPressed(Button trigger)
{
	if (mType == Xbox)
	{
		float deadZone = 0.1f;
		float zValue = sf::Joystick::getAxisPosition(mIdx, sf::Joystick::Axis::Z);

		if (trigger == LTrigger)
		{
			return zValue > deadZone;
		}
		else if (trigger == RTrigger)
		{
			return zValue < 0;
		}
	}
	else if (mType == Playstation)
	{
		return sf::Joystick::isButtonPressed(mIdx, mButtonMap[trigger]);
	}
}

bool Gamepad::isButtonDown(Button button)
{
	auto& mapThisFrame = mGamepadButtonsPressedThisFrame;
	auto& mapLastFrame = mGamepadButtonsPressedLastFrame;

	if (mapThisFrame.find(button) == mapThisFrame.end())
		return false;

	return mapThisFrame[button] == true && mapLastFrame[button] == false;
}

bool Gamepad::isButtonUp(Button button)
{
	auto& mapThisFrame = mGamepadButtonsPressedThisFrame;
	auto& mapLastFrame = mGamepadButtonsPressedLastFrame;

	if (mapThisFrame.find(button) == mapThisFrame.end())
		return false;

	return mapThisFrame[button] == false && mapLastFrame[button] == true;

}

float Gamepad::getAxis(Axis axis)
{
	float value = sf::Joystick::getAxisPosition(mIdx, mAxesMap[axis]);

	if (abs(value) < 10)
		return 0;

	return value / 100.f;
}

void Gamepad::update()
{
	for (int buttonInt = Gamepad::Button::North; buttonInt-1 != Gamepad::Button::LTrigger; buttonInt++)
	{
		Gamepad::Button button = static_cast<Gamepad::Button>(buttonInt);

		bool isPressedThisFrame = false;

		if (button == LTrigger || button == RTrigger)
		{
			isPressedThisFrame = isTriggerPressed(button);
		}
		else
		{
			isPressedThisFrame = isButtonPressed(button);
		}

		mGamepadButtonsPressedThisFrame[button] = isPressedThisFrame;
	}
}

void Gamepad::postUpdate()
{
	auto it = mGamepadButtonsPressedThisFrame.begin();

	while (it != mGamepadButtonsPressedThisFrame.end())
	{
		mGamepadButtonsPressedLastFrame[it->first] = mGamepadButtonsPressedThisFrame[it->first];
		++it;
	}
}

size_t Gamepad::getIdx()
{
	return mIdx;
}
