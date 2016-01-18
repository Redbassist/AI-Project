#pragma once
#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include "stdafx.h"
#include "Globals.h"

class Bullet {
private:
	Texture texture;
	Sprite sprite; 
	Pvector pos;
	Pvector direction;
	float speed;
	float radius;
	int ttl;
public:
	Bullet(Pvector, Pvector);
	void LoadAssets();
	void Update();
	bool LifeCheck();
	void Movement();
	void Draw();
};

#endif#pragma once
