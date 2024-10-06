#pragma once

#include "Animation.h"
#include "Assets.h"

class Component
{
public:
	bool has = false;
};

class CTransform : public Component
{
public:
	Vec2f position		= { 0.0f, 0.0f };
	Vec2f prevPosition	= { 0.0f, 0.0f };
	Vec2f scale			= { 1.0f, 1.0f };
	Vec2f velocity		= { 0.0f, 0.0f };
	float angle			= 0;

	CTransform() {}
	CTransform(const Vec2f& p) 
		: position(p), prevPosition(p) {}
	CTransform(const Vec2f& p, const Vec2f& v, const Vec2f& sc, float a)
		: position(p), prevPosition(p), velocity(v), scale(sc), angle(a) {}

};

class CLifeSpan : public Component 
{
public:
	int lifespan = 0;
	int frameCreated = 0;

	CLifeSpan() {}
	CLifeSpan(int duration, int frame) 
		: lifespan(duration), frameCreated(frame) {}

};

class CInput : public Component
{
public:
	bool up			= false;
	bool down		= false;
	bool left		= false;
	bool right		= false;
	bool shoot		= false;

	CInput() {}
};


class CBoundingBox : public Component
{
public:
	Vec2f size;
	Vec2f halfSize;
	
	CBoundingBox() {}
	CBoundingBox(const Vec2f& s) 
		: size(s), halfSize(s.x / 2.0f, s.y / 2.0f) {}
};

class CAnimation : public Component
{
public:
	Animation animation;
	bool repeat = false;

	CAnimation() {}
	CAnimation(const Animation& a, bool r) 
		: animation(a), repeat(r) {}
};

class CGravity : public Component
{
public:
	float gravity = 1.0f;
	
	CGravity() {}
	CGravity(float g) 
		: gravity(g) {}
};

class CState : public Component
{
public:
	std::string state	= "stay";
	bool onGround		= false;
	bool canShoot		= true;
	bool canJump		= true;

	CState() {}
	CState(const std::string& s)
		: state(s) {}
};
