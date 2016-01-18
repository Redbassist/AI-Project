#include "Asteroid.h"

Asteroid::Asteroid(Pvector p)
{
	pos = p;
	float randomX = (rand() % 15 + 5);
	float randomY = (rand() % 15 + 5);
	direction.x = (randomX - 10) / 10;
	direction.y = (randomY - 10) / 10;
	direction.normalize();
	speed = (rand() % 30 + 10) / 10;
	direction.mulScalar(speed);
	LoadAssets();
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

	float asteroidSize = (rand() % 20 + 1) / 10;
	sprite.setScale(asteroidSize, asteroidSize);

	sprite.setPosition(sf::Vector2f(pos.x, pos.y));
}

void Asteroid::Update()
{
	Movement();
	WrapAround();

	sprite.setPosition(sf::Vector2f(pos.x, pos.y));
}

void Asteroid::WrapAround()
{
	if (pos.x > globalBounds.x)
		pos.x = -50;
	else if (pos.x < -50)
		pos.x = globalBounds.x;

	if (pos.y > globalBounds.y)
		pos.y = -50;
	else if (pos.y < -50)
		pos.y = globalBounds.y;
}

void Asteroid::Movement()
{
	pos.addVector(direction);
}

void Asteroid::Draw()
{
	window->draw(sprite);
}
