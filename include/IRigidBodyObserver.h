#pragma once
// (c) Coding: Femb�ck Samuel, Hofer Thomas
// FH - Salzburg

class Collider;

class IRigidBodyObserver
{
public:
	virtual void onCollision(Collider* other) = 0;
};