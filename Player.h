#pragma once  
#include "stdafx.h"
#include "Globals.h"
#include <math.h>

class Player {
private:
	int width, height;
	Pvector position;
	Pvector direction;
	float prevRotation;
	float rotation;
	float targetRotation;
	float speed;
	sf::Texture texture;
	sf::Sprite sprite;
	float radius;
	float health;

public:
	Player();

	void loadResources();

	void Draw();

	void Update();

	void CenterCamera();

	Pvector getPosition();

	//these are the code from the gods that fix the rotation loop around 2 pi
	float CurveAngle(float from, float to, float step);

	Pvector Slerp(Pvector from, Pvector to, float step);

	Pvector normalize(Pvector source);

	float degreeToRadian(float angle);

	float radiansToDegrees(float angle);

	float dotProduct(Pvector v1, Pvector v2);

	float getRadius();
	
	sf::Sprite getSprite();

	Pvector getDirection();

	float getHealth();

	void setHealth(float h);
};