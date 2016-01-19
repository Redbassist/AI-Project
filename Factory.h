#pragma once  
#include "stdafx.h"
#include "Globals.h"
#include <math.h>
#include "Player.h"

class Factory{
private:
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
public:

	Factory(Player& p);

	void loadResources();

	void Draw();

	void Update();

	void Movement();

	void WrapAround();

	void Shoot();

	Pvector getPosition();

	float getRadius();

	sf::Sprite getSprite();

	Pvector getDirection();

	float getHealth();

	void setHealth(float h);

	float Distance(Pvector, Pvector);
};
