#include "Missile.h"

Missile::Missile(Pvector p, Player& pl) :
	pos(p)
{
	speed = 2.5;
	destroyed = false;
	player = &pl;
	LoadAssets();
	ttl = 600;
}

void Missile::LoadAssets()
{
	if (!texture.loadFromFile("Sprites/missile.png"))
	{
		cout << "cant find image";
	}

	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setOrigin(200, 200);

	float scale = 0.1;
	sprite.setScale(scale, scale);

	radius = 200 * scale;
	sprite.setPosition(sf::Vector2f(pos.x, pos.y));
}

void Missile::Update()
{
	Movement();
	ttl--;
}

bool Missile::LifeCheck()
{
	bool destroy = false;

	if (pos.x > globalBounds.x + radius || pos.x < -radius || pos.y > globalBounds.y + radius || pos.y < -radius)
		destroy = true;

	if (destroyed == true)
	{
		destroy = true;
	}

	if (ttl <= 0)
	{
		destroy = true;
	}

	return destroy;
}

void Missile::Movement()
{
	prevRotation = rotation;
	position = Pvector(sprite.getPosition());
	//Vector2i mp = Mouse::getPosition(*window);
	//used to convert to view coordinates
	//sf::Vector2f worldMousePos = window->mapPixelToCoords(mp);
	//Pvector mousePos = Pvector(worldMousePos);
	Pvector mousePos = player->getSprite().getPosition();
	Pvector wantedVector = mousePos.subTwoVector(mousePos, position);

	float angleBetweenTwo = atan2(mousePos.y - position.y, mousePos.x - position.x);

	rotation = CurveAngle(rotation, angleBetweenTwo, 0.05f);

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

void Missile::Draw()
{
	window->draw(sprite);
}

Pvector Missile::getPos()
{
	return pos;
}

float Missile::getRadius()
{
	return radius;
}

void Missile::setDestroyed(bool d)
{
	destroyed = d;
}

float Missile::CurveAngle(float from, float to, float step)
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

Pvector Missile::Slerp(Pvector from, Pvector to, float step)
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

Pvector Missile::normalize(Pvector source)
{
	float length = sqrt((source.x * source.x) + (source.y * source.y));
	if (length != 0)
		return sf::Vector2f(source.x / length, source.y / length);
	else
		return source;
}

float Missile::degreeToRadian(float angle) {
	float pi = 3.14159265358979323846;
	return  pi * angle / 180.0;
}

float Missile::radiansToDegrees(float angle) {
	float pi = 3.14159265358979323846;
	return angle * (180.0 / pi);
}

float Missile::dotProduct(Pvector v1, Pvector v2) {
	v1 = normalize(v1);
	v2 = normalize(v2);
	return (v1.x * v2.x) + (v1.y * v2.y);
}
