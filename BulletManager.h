#pragma once
#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include <SFML/Graphics.hpp>
#include "stdafx.h"
#include "Player.h"
#include "Bullet.h"
#include "Globals.h"

class BulletManager {
private:
	int maxAsteroids;
	float addTimer;
	float timerLength;
public:
	BulletManager();
	static BulletManager* GetInstance();
	void AddBullet(Bullet*);
	void Update();
	void Draw();
	vector<Bullet*> bullets;
};

#endif