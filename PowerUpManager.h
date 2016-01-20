#pragma once
#ifndef POWERUPMANAGER_H
#define POWERUPMANAGER_H

#include <SFML/Graphics.hpp>
#include "stdafx.h"
#include "Player.h"
#include "PowerUp.h"
#include "Globals.h"

class PowerUpManager {
private:
public:
	PowerUpManager();
	static PowerUpManager* GetInstance();
	void Update();
	void Draw();
	void SpawnPowerUp(PowerUp * powerup);
	vector<PowerUp*> powerups;
};

#endif
