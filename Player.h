#pragma once  
#include <SFML/Graphics.hpp>
#include <math.h>

class Player {
private:
	int width, height;
	sf::Vector2f position;
	sf::Vector2f direction;
	float prevRotation;
	float rotation;
	float targetRotation;
	float speed;
	sf::Texture texture;
	sf::Sprite sprite;
public:
	Player();

	void loadResources();

	void Draw(sf::RenderWindow &window);

	void Update(sf::RenderWindow &window);
	sf::Vector2f getPosition();

	//these are the code from the gods that fix the rotation loop around 2 pi
	float CurveAngle(float from, float to, float step);

	sf::Vector2f Slerp(sf::Vector2f from, sf::Vector2f to, float step);

	sf::Vector2f normalize(sf::Vector2f source);

	float degreeToRadian(float angle);

	float radiansToDegrees(float angle);

	float dotProduct(sf::Vector2f v1, sf::Vector2f v2);
};