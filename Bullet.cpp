#include "Bullet.h"

Bullet::Bullet(Pvector p, Pvector dir, bool owner, bool super) :
	pos(p)
{
	superBullet = super;
	playerBullet = owner;
	speed = 10;
	direction = dir;
	direction.normalize();
	direction.mulScalar(speed);
	direction = direction + dir;
	destroyed = false;
	ttl = 180;
	LoadAssets();
}

void Bullet::LoadAssets()
{
	if (!texture.loadFromFile("Sprites/bullet2.png"))
	{
		cout << "cant find image";
	}

	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setOrigin(2.5, 2.5);

	float scale = 0.5;
	sprite.setScale(scale, scale);

	radius = 2.5 * scale;
	sprite.setPosition(sf::Vector2f(pos.x, pos.y));
}

void Bullet::Update()
{
	Movement(); 

	sprite.setPosition(sf::Vector2f(pos.x, pos.y));

	ttl--;
}

bool Bullet::LifeCheck()
{
	bool destroy = false;

	if (pos.x > globalBounds.x + radius || pos.x < -radius || pos.y > globalBounds.y + radius || pos.y < -radius)
		destroy = true;
	if (ttl <= 0)
		destroy = true;

	if (destroyed == true)
	{
		destroy = true;
	}

	return destroy;
}

void Bullet::Movement()
{
	pos.addVector(direction);
}

void Bullet::Draw()
{
	window->draw(sprite);
}

Pvector Bullet::getPos()
{
	return pos;
}

float Bullet::getRadius()
{
	return radius;
}

void Bullet::setDestroyed(bool d)
{
	destroyed = d;
}
