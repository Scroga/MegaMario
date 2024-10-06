#pragma once

#include "Vec2.h"
#include "Entity.h"

#include <iostream>

class Physics
{
public:

	static Vec2f getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
	static Vec2f getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);

};