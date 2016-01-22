#ifdef _DEBUG
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-network-d.lib")
#else
#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-audio.lib")
#pragma comment(lib,"sfml-system.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-network.lib")
#endif

#include "stdafx.h"
#include "Globals.h"
#include <iostream>
#include "Factory.h"

using namespace std;

#include <SFML/Graphics.hpp>
#include "Player.h" 
#include "Boid.h"
#include "BoidManager.h"
#include "Predator.h"
#include "Swarmer.h"
#include "AsteroidManager.h"
#include "CollisionManager.h"
#include "FactoryManager.h"
#include "MissileManager.h"
#include "PowerUpManager.h"

int main()
{
	bool gameOn = false;
	bool checkEnter = true;
	bool loser = false;
	bool winner = false;

	srand(time(NULL));
	sf::Font MyFont;
	if (!MyFont.loadFromFile("arial.ttf"))
	{
		// Error...
	}

	srand(time(NULL)); //setting seed for random

	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 1000;
	settings.majorVersion = 3;
	settings.minorVersion = 0;

	globalBounds = Vector2f(3840, 2160);

	window = new RenderWindow(sf::VideoMode(1280, 720), "AI Lab 1", sf::Style::Default, settings);
	View view = View(Vector2f(0, 0), Vector2f(1280, 720));
	view.zoom(1);
	window->setView(view);
	window->setFramerateLimit(60);

	sf::Texture start;
	start.loadFromFile("Sprites/start.png");
	start.setSmooth(true);
	sf::Sprite startSprite;
	startSprite.setTexture(start);
	startSprite.setScale(1, 1);

	sf::Texture win;
	win.loadFromFile("Sprites/win.png");
	win.setSmooth(true);
	sf::Sprite winSprite;
	winSprite.setTexture(win);
	winSprite.setScale(1, 1);

	sf::Texture lose;
	lose.loadFromFile("Sprites/lose.png");
	lose.setSmooth(true);
	sf::Sprite loseSprite;
	loseSprite.setTexture(lose);
	loseSprite.setScale(1, 1);

	sf::Texture overlay;
	overlay.loadFromFile("Sprites/overlay.png");
	overlay.setSmooth(true);
	sf::Sprite overlaySprite;
	overlaySprite.setTexture(overlay);
	overlaySprite.setScale(0.3, 0.3);

	sf::Texture underlay;
	underlay.loadFromFile("Sprites/underlay.png");
	underlay.setSmooth(true);
	sf::Sprite underlaySprite;
	underlaySprite.setTexture(underlay);
	underlaySprite.setScale(0.3, 0.3);

	sf::Texture healthUI;
	healthUI.loadFromFile("Sprites/health.png");
	healthUI.setSmooth(true);
	sf::Sprite healthUISprite;
	healthUISprite.setTexture(healthUI);
	healthUISprite.setScale(0.3, 0.3);

	sf::Texture healthbar;
	healthbar.loadFromFile("Sprites/healthbar.png");
	healthbar.setSmooth(true);
	sf::Sprite healthbarSprite;
	healthbarSprite.setTexture(healthbar);
	healthbarSprite.setScale(0.3, 0.3);

	sf::Texture background;
	if (!background.loadFromFile("Sprites/background.jpg"))
	{
		// error...
	}

	background.setSmooth(true);
	sf::Sprite backgroundSprite;
	backgroundSprite.setTexture(background);
	backgroundSprite.setScale(sf::Vector2f(1.5f, 1.5f));
	backgroundSprite.setPosition(sf::Vector2f(1500, 0));
	backgroundSprite.setOrigin(sf::Vector2f(1715, 1733));

	Player* player = new Player();

	AsteroidManager::GetInstance()->player = player;

	int numFactories = 4;
	for (int i = 0; i < numFactories; i++)
	{
		FactoryManager::GetInstance()->AddFactory(new Factory(*player, i));
	}

	CollisionManager::GetInstance()->setPlayer(*player);

	//update loop
	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
		}

		if (gameOn == true) {
			
			if (!winner && !loser) {
				player->Update();
				if (FactoryManager::GetInstance()->factories.size() == 0)
					winner = true;
				else if (player->lives == 0)
					loser = true;
			}

			AsteroidManager::GetInstance()->Update();
			BoidManager::GetInstance()->Update();
			BulletManager::GetInstance()->Update();
			MissileManager::GetInstance()->Update();
			FactoryManager::GetInstance()->Update();
			CollisionManager::GetInstance()->CheckCollisions();
			PowerUpManager::GetInstance()->Update();

			window->clear();
			//draw stuff here
			window->draw(backgroundSprite);

			if (!winner && !loser)
				player->Draw();

			AsteroidManager::GetInstance()->Draw();
			PowerUpManager::GetInstance()->Draw();
			BoidManager::GetInstance()->Draw();
			FactoryManager::GetInstance()->Draw();
			BulletManager::GetInstance()->Draw();
			MissileManager::GetInstance()->Draw();

			//UI stuff
			View tempView = window->getView();
			Vector2f viewPos = tempView.getCenter();

			overlaySprite.setPosition(Vector2f(viewPos.x + 400, viewPos.y - 360));
			underlaySprite.setPosition(Vector2f(viewPos.x + 400, viewPos.y - 360));
			healthUISprite.setPosition(Vector2f(viewPos.x + 330, viewPos.y - 360));

			float scaleY = 0.3 * (player->getHealth() / 1000);
			healthbarSprite.setScale(0.3, scaleY);
			healthbarSprite.setPosition(Vector2f(viewPos.x + 352, viewPos.y - 330));

			if (!winner && !loser) {
				window->draw(underlaySprite);
				window->draw(overlaySprite);
				window->draw(healthUISprite);
				window->draw(healthbarSprite);
				BoidManager::GetInstance()->DrawUI();
				FactoryManager::GetInstance()->DrawUI();
				player->DrawUI();
			}
			else if (winner) {
				winSprite.setPosition(Vector2f(viewPos.x - 640, viewPos.y - 360));
				window->draw(winSprite);
			}
			else if (loser) {
				loseSprite.setPosition(Vector2f(viewPos.x - 640, viewPos.y - 360));
				window->draw(loseSprite);
			}
			window->display();
		}
		else {
			if (checkEnter == true) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
					checkEnter = false;
					gameOn = true;
				}
			}

			window->clear();
			if (checkEnter == true) {
				startSprite.setPosition(-640, -360);
				window->draw(startSprite);
			}

			window->display();
		}

	}

	return 0;
}