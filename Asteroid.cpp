#include "Asteroid.h"

Asteroid::Asteroid(Pvector p)
{
	pos = p;
	float randomX = (rand() % 15 + 5);
	float randomY = (rand() % 15 + 5);
	direction.x = (randomX - 10) / 10;
	direction.y = (randomY - 10) / 10;
	direction.normalize();
	speed = (rand() % 20 + 10);
	speed = speed / 10;
	direction.mulScalar(speed);
	LoadAssets();
	health = 100;
	rot = (rand() % 10 + 1);
	rot = rot / 10;
	int temp = (rand() % 2) + 1;
	if (temp == 1)
	{
		rot = -rot;
	}
}

void Asteroid::LoadAssets()
{
	int asteroidNum = rand() % 3 + 1;
	if (!texture.loadFromFile("Sprites/asteroid" +  to_string(asteroidNum) + ".png"))
	{
		cout << "cant find image";
	}

	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setOrigin(50, 50);

	float asteroidSize = (rand() % 15 + 5);
	sprite.setScale(asteroidSize / 10, asteroidSize / 10);

	radius = 50 * (asteroidSize / 10);

	sprite.setPosition(sf::Vector2f(pos.x, pos.y));
}

void Asteroid::Update()
{
	Movement();
	WrapAround();

	sprite.setPosition(sf::Vector2f(pos.x, pos.y));
}

bool Asteroid::LifeCheck()
{
	bool destroy = false;

	if (health <= 0)
	{
		destroy = true;
	}

	return destroy;
}

void Asteroid::WrapAround()
{
	if (pos.x > globalBounds.x + radius)
		pos.x = -radius;
	else if (pos.x < -radius)
		pos.x = globalBounds.x + radius;

	if (pos.y > globalBounds.y + radius)
		pos.y = -radius;
	else if (pos.y < -radius)
		pos.y = globalBounds.y + radius;
}

void Asteroid::Movement()
{
	sprite.setRotation(sprite.getRotation() + rot);
	pos.addVector(direction);
}

void Asteroid::Draw()
{
	window->draw(sprite);
}

float Asteroid::getRadius()
{
	return radius;
}

Pvector Asteroid::getPos()
{
	return pos;
}

Pvector Asteroid::getDirection()
{
	return direction;
}

void Asteroid::setDirection(Pvector d)
{
	direction = d;
}

float Asteroid::getSpeed()
{
	return speed;
}

float Asteroid::getHealth()
{
	return health;
}

void Asteroid::setHealth(float h)
{
	health = h;
}
