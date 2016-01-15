#pragma once
#ifndef ASTEROID_H
#define ASTEROID_H

#include <SFML/Graphics.hpp>
#include "stdafx.h"
#include "Globals.h"

class Asteroid {
private:
	Texture texture;
	Sprite sprite;
	Pvector pos;
	Pvector direction;
	float speed;
public:
	Asteroid(Pvector);
	void LoadAssets();
	void Update();
	void Movement();
	void Draw();
};

#endif