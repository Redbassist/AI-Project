#include "stdafx.h"
#include "Player.h"

Player::Player() {
	direction = sf::Vector2f((float)cos(rotation), -sin(rotation));
	speed = 0.04f;
	float randX = rand() % 700 + 50;
	float randY = rand() % 500 + 50;

	position = sf::Vector2f(randX, randY);

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
	position = sprite.getPosition();
	sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
	sf::Vector2f wantedVector = mousePos - position;

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

	direction = sf::Vector2f(cos(rotation), sin(rotation));
	//normalize(direction);
	direction *= speed;

	//if space is held down, will fly forwards
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		position += direction;

	sprite.setRotation(radiansToDegrees(rotation) + 90);
	sprite.setPosition(position);
}

sf::Vector2f Player::getPosition() {
	return position;
}
float Player::CurveAngle(float from, float to, float step)
{
	if (step == 0)
		return from;
	if (from == to || step == 1)
		return to;

	sf::Vector2f fromVector = sf::Vector2f((float)cos(from), (float)sin(from));
	sf::Vector2f toVector = sf::Vector2f((float)cos(to), (float)sin(to));

	sf::Vector2f currentVector = Slerp(fromVector, toVector, step);

	return (float)atan2(currentVector.y, currentVector.x);
}

sf::Vector2f Player::Slerp(sf::Vector2f from, sf::Vector2f to, float step)
{
	if (step == 0) return from;
	if (from == to || step == 1) return to;

	double theta = acos(dotProduct(from, to));
	if (theta == 0) return to;

	double sinTheta = sin(theta);
	return (float)(sin((1 - step) * theta) / sinTheta) * from + (float)(sin(step * theta) / sinTheta) * to;
}

sf::Vector2f Player::normalize(sf::Vector2f source)
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

float Player::dotProduct(sf::Vector2f v1, sf::Vector2f v2) {
	v1 = normalize(v1);
	v2 = normalize(v2);
	return (v1.x * v2.x) + (v1.y * v2.y);
}
