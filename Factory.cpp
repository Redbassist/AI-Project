#include "stdafx.h"
#include "Factory.h"

Factory::Factory(Player& p){
	speed = 1;
	//float randX = rand() % (int)(globalBounds.x - 50) + 50;
	//float randY = rand() % (int)(globalBounds.y - 50) + 50;
	float randX = 200;
	float randY = 200;
	position = Pvector(randX, randY);
	loadResources();
	scared = false;
	travelling = false;
	player = &p;
}

void Factory::loadResources() {
	if (!texture.loadFromFile("Sprites/FactoryPart.png"))
	{
		std::cout << "cant find image";
	}

	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setOrigin(400, 400);
	sprite.setScale(0.35f, 0.35f);
	sprite.setPosition(position.x, position.y);

	if (!texture2.loadFromFile("Sprites/FactoryPart.png"))
	{
		std::cout << "cant find image";
	}

	texture2.setSmooth(true);
	sprite2.setTexture(texture);
	sprite2.setOrigin(400, 400);
	sprite2.setScale(0.35f, 0.35f);
	sprite2.setPosition(position.x, position.y);

	radius = 400 * sprite.getScale().x;

	health = 1000;
}

void Factory::Draw() {
	window->draw(sprite);
	window->draw(sprite2);
}

void Factory::Update() {
	Movement();
	WrapAround();
	Shoot();
}

float Factory::getRadius()
{
	return radius;
}

sf::Sprite Factory::getSprite()
{
	return sprite;
}

Pvector Factory::getDirection()
{
	return direction;
}

float Factory::getHealth()
{
	return health;
}

void Factory::Movement()
{
	sprite.setRotation(sprite.getRotation() + 0.1);
	sprite2.setRotation(sprite2.getRotation() - 0.1);

	if (scared == false)
	{
		if (travelling == true)
		{
			Pvector vel;
			vel.x = position.x - destination.x;
			vel.y = position.y - destination.y;
			vel.normalize();
			vel.mulScalar(speed);
			position.x = position.x - vel.x;
			position.y = position.y - vel.y;
		}
		else
		{
			destination.x = rand() % (int)(globalBounds.x - 100) + 100;
			destination.y = rand() % (int)(globalBounds.y - 100) + 100;
			travelling = true;
		}

		if (Distance(position, destination) < 50)
		{
			travelling = false;
		}

		if (Distance(position, player->getPosition()) < 400)
		{
			scared = true;
			travelling = false;
		}
	}
	else
	{
		Pvector vel;
		vel.x = position.x - player->getPosition().x;
		vel.y = position.y - player->getPosition().y;
		vel.normalize();
		vel.mulScalar(speed);
		position.x = position.x + vel.x;
		position.y = position.y + vel.y;

		if (Distance(position, player->getPosition()) > 399)
		{
			scared = false;
		}
	}
}

void Factory::WrapAround()
{
	//checking if the player has moved off the side of the screen and moving it
	if (position.x > globalBounds.x + 120)
		position.x = -120;
	else if (position.x < -120)
		position.x = globalBounds.x + 120;
	if (position.y > globalBounds.y + 120)
		position.y = -100;
	else if (position.y < -120)
		position.y = globalBounds.y + 120;

	sprite.setPosition(sf::Vector2f(position.x, position.y));
	sprite2.setPosition(sprite.getPosition());
}

void Factory::Shoot()
{

}

Pvector Factory::getPosition() {
	return position;
}

float Factory::Distance(Pvector pos1, Pvector pos2)
{
	return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
}