#pragma once  
#include "stdafx.h"
#include "Globals.h"
#include <math.h>
#include "Player.h"
#include "BoidManager.h"
#include "Predator.h"

class Factory{
private:
	float prevRotation;
	float rotation;
	float targetRotation;
	Player* player;
	int width, height;
	Pvector position;
	Pvector direction;
	float speed;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Texture texture2;
	sf::Sprite sprite2;
	float radius;
	float health;
	bool travelling;
	Pvector destination;
	bool scared;
	int spawnTimer;
public:

	Factory(Player& p);

	void loadResources();

	void Draw();

	void Update();

	void Movement();

	void WrapAround();

	void Shoot();

	//these are the code from the gods that fix the rotation loop around 2 pi
	float CurveAngle(float from, float to, float step);

	Pvector Slerp(Pvector from, Pvector to, float step);

	Pvector normalize(Pvector source);

	float degreeToRadian(float angle);

	float radiansToDegrees(float angle);

	float dotProduct(Pvector v1, Pvector v2);

	Pvector getPosition();

	float getRadius();

	sf::Sprite getSprite();

	Pvector getDirection();

	float getHealth();

	void setHealth(float h);

	float Distance(Pvector, Pvector);

	void State();

	void Spawn();
};
