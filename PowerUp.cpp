#include "PowerUp.h"

PowerUp::PowerUp(Pvector p)
{
	pos = p;
	type = (rand() % 2 + 1);
	destroy = false;
	LoadAssets();
	ttl = 300;
}

void PowerUp::LoadAssets()
{
	if (type == 1)
	{
		if (!texture.loadFromFile("Sprites/Speed.png"))
		{
			cout << "cant find image";
		}
	}
	else
	{
		if (!texture.loadFromFile("Sprites/Power.png"))
		{
			cout << "cant find image";
		}
	}

	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setOrigin(200, 200);

	sprite.setScale(0.1, 0.1);

	radius = 200 * sprite.getScale().x;

	sprite.setPosition(sf::Vector2f(pos.x, pos.y));
}

void PowerUp::Update()
{
	sprite.setPosition(sf::Vector2f(pos.x, pos.y));
}

bool PowerUp::LifeCheck()
{
	return destroy;
}

void PowerUp::Draw()
{
	window->draw(sprite);
	ttl--;
	if (ttl <= 0)
	{
		destroy = true;
	}
}

float PowerUp::getRadius()
{
	return radius;
}

Pvector PowerUp::getPos()
{
	return pos;
}
