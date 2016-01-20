#pragma once
#ifndef MISSILEMANAGER_H
#define MISSILEMANAGER_H

#include <SFML/Graphics.hpp>
#include "stdafx.h"
#include "Player.h"
#include "Missile.h"
#include "Globals.h"

class MissileManager {
private:
	int maxAsteroids;
	float addTimer;
	float timerLength;
public:
	MissileManager();
	static MissileManager* GetInstance();
	void AddMissile(Missile*);
	void Update();
	void Draw();
	vector<Missile*> missiles;
};

#endif
