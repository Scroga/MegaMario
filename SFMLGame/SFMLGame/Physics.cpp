#include "Physics.h"
#include "Components.h"

#include <math.h>

Vec2f Physics::getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
	auto& posA = a->getComponent<CTransform>().position;
	auto& posB = b->getComponent<CTransform>().position;

	auto& halfSizeA = a->getComponent<CBoundingBox>().halfSize;
	auto& halfSizeB = b->getComponent<CBoundingBox>().halfSize;

	Vec2f delta((float)abs(posA.x - posB.x), (float)abs(posA.y - posB.y));

	float ox = (halfSizeA.x + halfSizeB.x) - delta.x;
	float oy = (halfSizeA.y + halfSizeB.y) - delta.y;

	return Vec2f(ox, oy);
}

Vec2f Physics::getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
	auto& prevPosA = a->getComponent<CTransform>().prevPosition;
	auto& prevPosB = b->getComponent<CTransform>().prevPosition;

	auto& halfSizeA = a->getComponent<CBoundingBox>().halfSize;
	auto& halfSizeB = b->getComponent<CBoundingBox>().halfSize;

	Vec2f delta((float)abs(prevPosA.x - prevPosB.x), (float)abs(prevPosA.y - prevPosB.y));

	float ox = (halfSizeA.x + halfSizeB.x) - delta.x;
	float oy = (halfSizeA.y + halfSizeB.y) - delta.y;

	return Vec2f(ox, oy);
}