#pragma once
#ifndef POWERUP_H
#define POWERUP_H

#include <SFML/Graphics.hpp>
#include "stdafx.h"
#include "Globals.h"

class PowerUp {
private:
	Texture texture;
	Sprite sprite;
	Pvector pos;
	float radius;
	int ttl;
public:
	PowerUp(Pvector);
	void LoadAssets();
	void Update();
	void Draw();
	float getRadius();
	Pvector getPos();
	bool LifeCheck();
	bool destroy;
	int type;
};

#endif
