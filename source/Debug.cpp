#include "Debug.h"
#include <iostream>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

void Debug::log(std::string name, sf::Vector2f vec)
{
	std::cout << "<" << name << "> x: " << vec.x << ", y: " << vec.y << std::endl;
}
