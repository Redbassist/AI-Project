#include "stdafx.h"
#include "Player.h"

Player::Player() {
	direction = Pvector((float)cos(rotation), -sin(rotation));
	speed = 7;
	float randX = rand() % (int)(globalBounds.x - 50) + 50;
	float randY = rand() % (int)(globalBounds.y - 50) + 50; 
	position = Pvector(randX, randY);
	fireRate = 5;
	fireTimer = fireRate;
	loadResources();;
	lives = 3;
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

	if (!uitexture.loadFromFile("Sprites/Spaceship3.png"))
	{
		std::cout << "cant find image";
	}

	uitexture.setSmooth(true);
	uisprite.setTexture(uitexture);
	uisprite.setOrigin(200, 400);
	uisprite.setScale(0.02f, 0.02f);

	if (!livesTexture.loadFromFile("Sprites/Life.png"))
	{
		std::cout << "cant find image";
	}
	
	livesTexture.setSmooth(true);
	livesSprite.setTexture(livesTexture);
	livesSprite.setOrigin(200, 200);
	livesSprite.setScale(0.06f, 0.06f);

	radius = 300 * sprite.getScale().x;

	health = 1000;
}

void Player::Draw() {
	window->draw(sprite);
}

void Player::DrawUI()
{
	View tempView = window->getView();
	Vector2f viewPos = tempView.getCenter();

	uisprite.setRotation(radiansToDegrees(rotation) + 90);
	uisprite.setPosition(Vector2f(viewPos.x + 520, viewPos.y - 270));


	livesSprite.setRotation(0);

	for (int i = 0; i < lives; i++)
	{
		livesSprite.setPosition(Vector2f(viewPos.x + 310, (viewPos.y - 318) + 50 * i));
		window->draw(livesSprite);
	}
	
	window->draw(uisprite);
}

void Player::Update() {
	Movement();
	WrapAround();
	Shoot();
	Respawn();
	CenterCamera();
}

float Player::getRadius()
{
	return radius;
}

sf::Sprite Player::getSprite()
{
	return sprite;
}

Pvector Player::getDirection()
{
	return direction;
}

float Player::getHealth()
{
	return health;
}

void Player::dropHealth(float h)
{
	health -= h;
}

void Player::ActivateSpeed()
{
	speedTimer = 600;
}

void Player::ActivatePower()
{
	powerTimer = 600;
}

void Player::Movement()
{
	prevRotation = rotation;
	position = Pvector(sprite.getPosition());
	Vector2i mp = Mouse::getPosition(*window);
	//used to convert to view coordinates
	sf::Vector2f worldMousePos = window->mapPixelToCoords(mp);
	Pvector mousePos = Pvector(worldMousePos);
	Pvector wantedVector = mousePos.subTwoVector(mousePos, position);

	float angleBetweenTwo = atan2(mousePos.y - position.y, mousePos.x - position.x);

	rotation = CurveAngle(rotation, angleBetweenTwo, 0.12f);

	//error check, rotation was crashing every so often, this is a loose fix
	if (isnan(rotation))
		rotation = prevRotation;

	direction = Pvector(cos(rotation), sin(rotation));
	//normalize(direction);

	if (speedTimer > 0)
	{
		direction.mulScalar(speed * 2);
	}
	else
	{
		direction.mulScalar(speed);
	}

	speedTimer--;

	//if space is held down, will fly forwards
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		position.addVector(direction);

	sprite.setRotation(radiansToDegrees(rotation) + 90);
	
	if (!dead)
		sprite.setPosition(sf::Vector2f(position.x, position.y));
}

void Player::WrapAround()
{
	//checking if the player has moved off the side of the screen and moving it
	if (position.x > globalBounds.x + 32)
		position.x = -32;
	else if (position.x < -32)
		position.x = globalBounds.x + 32;
	if (position.y > globalBounds.y + 32)
		position.y = -32;
	else if (position.y < -32)
		position.y = globalBounds.y + 32;

	sprite.setPosition(sf::Vector2f(position.x, position.y));
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

void Player::Shoot()
//Creates a bullet that is fired from the front of the ship in the direction that the player is looking
{ 
	fireTimer++;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (fireTimer > fireRate) {
			fireTimer = 0;
			if (powerTimer > 0) {
				Bullet* bullet = new Bullet((position + direction * 3), direction, true, true);
				BulletManager::GetInstance()->AddBullet(bullet);
			}
			else {
				Bullet* bullet = new Bullet((position + direction * 3), direction, true, false);
				BulletManager::GetInstance()->AddBullet(bullet);
			}
		}
	}
	powerTimer--;
}

void Player::Respawn()
{
	if (health <= 0) {
		float randX = rand() % (int)(globalBounds.x - 50) + 50;
		float randY = rand() % (int)(globalBounds.y - 50) + 50;
		position = Pvector(randX, randY);
		sprite.setPosition(sf::Vector2f(position.x, position.y));
		health = 1000;
		speedTimer = 0;
		powerTimer = 0;
		lives--;

		if (lives == 0)
		{
			position = Pvector(-100000, -100000);
			dead = true;
		}
	}
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
