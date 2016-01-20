#pragma once
#ifndef MISSILE_H
#define MISSILE_H

#include <SFML/Graphics.hpp>
#include "stdafx.h"
#include "Globals.h"
#include "Player.h"

class Missile {
private:
	float prevRotation;
	float rotation;
	float targetRotation;
	Pvector position;
	Texture texture;
	Sprite sprite;
	Pvector pos;
	Pvector direction;
	float speed;
	float radius;
	bool destroyed;
	Player* player;
	int ttl;
public:
	Missile(Pvector, Player& p);
	void LoadAssets();
	void Update();
	bool LifeCheck();
	void Movement();
	void Draw();
	Pvector getPos();
	float getRadius();
	void setDestroyed(bool);
	float CurveAngle(float from, float to, float step);
	Pvector Slerp(Pvector from, Pvector to, float step);
	Pvector normalize(Pvector source);
	float degreeToRadian(float angle);
	float radiansToDegrees(float angle);
	float dotProduct(Pvector v1, Pvector v2);
};

#endif
