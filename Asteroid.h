#pragma once
#ifndef ASTEROID_H
#define ASTEROID_H

#include <SFML/Graphics.hpp>
#include "stdafx.h"
#include "Globals.h"
#include "PowerUpManager.h"
#include "PowerUp.h"

class Asteroid {
private:
	Texture texture;
	Sprite sprite;
	Pvector pos;
	Pvector direction;
	float speed;
	float radius;
	float health;
	float rot;
public:
	Asteroid(Pvector);
	void LoadAssets();
	void Update();
	void WrapAround();
	void Movement();
	void Draw();
	float getRadius();
	Pvector getPos();
	Pvector getDirection();
	void setDirection(Pvector d);
	float getSpeed();
	bool LifeCheck();
	float getHealth();
	void setHealth(float h);
};

#endif