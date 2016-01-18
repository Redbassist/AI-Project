#include "stdafx.h"
#include "Player.h"

Player::Player() {
	direction = Pvector((float)cos(rotation), -sin(rotation));
	speed = 3;
	float randX = rand() % 700 + 50;
	float randY = rand() % 500 + 50;

	position = Pvector(randX, randY);

	loadResources();
}

void Player::loadResources() {
	if (!texture.loadFromFile("Sprites/Spaceship3.png"))
	{
		std::cout << "cant find image";
	}

	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setOrigin(200, 400);
	sprite.setScale(0.06f, 0.06f);
	sprite.setPosition(position.x, position.y);
}

void Player::Draw() {
	window->draw(sprite);
}

void Player::Update() {
	prevRotation = rotation;
	position = Pvector(sprite.getPosition());
	Vector2i mp = Mouse::getPosition(*window);
	//used to convert to view coordinates
	sf::Vector2f worldMousePos = window->mapPixelToCoords(mp);
	Pvector mousePos = Pvector(worldMousePos);
	Pvector wantedVector = mousePos.subTwoVector(mousePos, position);

	float angleBetweenTwo = atan2(mousePos.y - position.y, mousePos.x - position.x);

	rotation = CurveAngle(rotation, angleBetweenTwo, 0.06f);

	//checking if the player has moved off the side of the screen and moving it ----------------------------------
	if (position.x > globalBounds.x)
		position.x = -32;
	else if (position.x < -32)
		position.x = globalBounds.x;

	if (position.y > globalBounds.y)
		position.y = -32;
	else if (position.y < -32)
		position.y = globalBounds.y;
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

	CenterCamera();
}

void Player::CenterCamera()
//will not move the camera if it is on the bounds of the screen
{
	View view = window->getView();
	Vector2f cameraPos = Vector2f(position.x, position.y);
	Vector2f size = view.getSize();

	if (cameraPos.x < size.x / 2) {
		cameraPos.x = size.x / 2;
	}
	else if (cameraPos.x > globalBounds.x - size.x / 2) {
		cameraPos.x = globalBounds.x - size.x / 2;
	}
	if (cameraPos.y < size.y / 2) {
		cameraPos.y = size.y / 2;
	}
	else if (cameraPos.y > globalBounds.y - size.y / 2) {
		cameraPos.y = globalBounds.y - size.y / 2;
	}
		
	view.setCenter(sf::Vector2f(cameraPos));
	window->setView(view);
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
