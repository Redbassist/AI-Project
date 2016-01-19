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
	if (!texture.loadFromFile("Sprites/FactoryPart2.png"))
	{
		std::cout << "cant find image";
	}

	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setOrigin(400, 400);
	sprite.setScale(0.35f, 0.35f);
	sprite.setPosition(position.x, position.y);

	if (!texture2.loadFromFile("Sprites/FactoryPart2.png"))
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
	State();
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

void Factory::State()
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
		//vel.mulScalar(speed);
		destination.x = vel.x * 400;
		destination.y = vel.y * 400;
		//position.x = position.x + vel.x;
		//position.y = position.y + vel.y;

		if (Distance(position, player->getPosition()) > 399)
		{
			scared = false;
			travelling = false;
		}
	}
}

void Factory::Movement()
{
	prevRotation = rotation;
	position = Pvector(sprite.getPosition());
	//Vector2i mp = Mouse::getPosition(*window);
	//used to convert to view coordinates
	//sf::Vector2f worldMousePos = window->mapPixelToCoords(mp);
	//Pvector mousePos = Pvector(worldMousePos);
	Pvector mousePos = destination;
	Pvector wantedVector = mousePos.subTwoVector(mousePos, position);

	float angleBetweenTwo = atan2(mousePos.y - position.y, mousePos.x - position.x);

	rotation = CurveAngle(rotation, angleBetweenTwo, 0.01f);

	//error check, rotation was crashing every so often, this is a loose fix
	if (isnan(rotation))
		rotation = prevRotation;

	direction = Pvector(cos(rotation), sin(rotation));
	//normalize(direction);
	direction.mulScalar(speed);

	//if space is held down, will fly forwards
	position.addVector(direction);

	//sprite.setRotation(radiansToDegrees(rotation) + 90);
	sprite.setPosition(sf::Vector2f(position.x, position.y));
}

void Factory::WrapAround()
{
	//checking if the player has moved off the side of the screen and moving it
	if (position.x > globalBounds.x + 150)
		position.x = -120;
	else if (position.x < -150)
		position.x = globalBounds.x + 120;
	if (position.y > globalBounds.y + 150)
		position.y = -100;
	else if (position.y < -150)
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

float Factory::CurveAngle(float from, float to, float step)
{
	if (step == 0)
		return from;
	if (from == to || step == 1)
		return to;

	Pvector fromVector = Pvector((float)cos(from), (float)sin(from));
	Pvector toVector = Pvector((float)cos(to), (float)sin(to));

	Pvector currentVector = Slerp(fromVector, toVector, step);

	return (float)atan2(currentVector.y, currentVector.x);
}

Pvector Factory::Slerp(Pvector from, Pvector to, float step)
{
	if (step == 0) return from;
	if (from == to || step == 1) return to;

	double theta = acos(dotProduct(from, to));
	if (theta == 0) return to;

	double sinTheta = sin(theta);
	from.mulScalar((float)(sin((1 - step) * theta) / sinTheta));
	to.mulScalar((float)(sin(step * theta) / sinTheta));
	from.addVector(to);
	return from;
}

Pvector Factory::normalize(Pvector source)
{
	float length = sqrt((source.x * source.x) + (source.y * source.y));
	if (length != 0)
		return sf::Vector2f(source.x / length, source.y / length);
	else
		return source;
}

float  Factory::degreeToRadian(float angle) {
	float pi = 3.14159265358979323846;
	return  pi * angle / 180.0;
}

float Factory::radiansToDegrees(float angle) {
	float pi = 3.14159265358979323846;
	return angle * (180.0 / pi);
}

float Factory::dotProduct(Pvector v1, Pvector v2) {
	v1 = normalize(v1);
	v2 = normalize(v2);
	return (v1.x * v2.x) + (v1.y * v2.y);
}