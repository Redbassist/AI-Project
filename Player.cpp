#include "stdafx.h"
#include "Player.h"

Player::Player() {
	direction = Pvector((float)cos(rotation), -sin(rotation));
	speed = 0.04f;
	float randX = rand() % 700 + 50;
	float randY = rand() % 500 + 50;

	position = Pvector(randX, randY);

	loadResources();
}

void Player::loadResources() {
	if (!texture.loadFromFile("ship.png"))
	{
		std::cout << "cant find image";
	}
	sprite.setTexture(texture);
	sprite.setOrigin(32, 32);
	sprite.setPosition(position.x, position.y);
}

void Player::Draw(sf::RenderWindow &window) {
	window.draw(sprite);
}

void Player::Update(sf::RenderWindow &window) {
	prevRotation = rotation;
	position = Pvector(sprite.getPosition());
	Pvector mousePos = Pvector(sf::Mouse::getPosition(window));
	Pvector wantedVector = mousePos.subTwoVector(mousePos, position);

	float angleBetweenTwo = atan2(mousePos.y - position.y, mousePos.x - position.x);

	rotation = CurveAngle(rotation, angleBetweenTwo, 0.0006f);

	//checking if the player has moved off the side of the screen and moving it ----------------------------------
	if (position.x > 800)
		position.x = -32;
	else if (position.x < -32)
		position.x = 800;

	if (position.y > 600)
		position.y = -32;
	else if (position.y < -32)
		position.y = 600;
	//------------------------------------------------------------------------------------------------------------
	//error check, rotation was crashing every so often, this is a loose fix
	if (isnan(rotation))
		rotation = prevRotation;

	direction = Pvector(cos(rotation), sin(rotation));
	//normalize(direction);
	direction.mulScalar(speed);

	//if space is held down, will fly forwards
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		position.addVector(direction);

	sprite.setRotation(radiansToDegrees(rotation) + 90);
	sprite.setPosition(sf::Vector2f(position.x, position.y));
}

Pvector Player::getPosition() {
	return position;
}
float Player::CurveAngle(float from, float to, float step)
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

Pvector Player::Slerp(Pvector from, Pvector to, float step)
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

Pvector Player::normalize(Pvector source)
{
	float length = sqrt((source.x * source.x) + (source.y * source.y));
	if (length != 0)
		return sf::Vector2f(source.x / length, source.y / length);
	else
		return source;
}

float Player::degreeToRadian(float angle) {
	float pi = 3.14159265358979323846;
	return  pi * angle / 180.0;
}

float Player::radiansToDegrees(float angle) {
	float pi = 3.14159265358979323846;
	return angle * (180.0 / pi);
}

float Player::dotProduct(Pvector v1, Pvector v2) {
	v1 = normalize(v1);
	v2 = normalize(v2);
	return (v1.x * v2.x) + (v1.y * v2.y);
}
