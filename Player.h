#pragma once  
#include "stdafx.h"
#include "Globals.h"
#include "BulletManager.h"
#include <math.h>

/*! The player class */
class Player {
private:
	int width, height; 
	Pvector position; /**< Position of the player (is updated to move) */
	Pvector direction; /**< The direction that the player is facing */
	float prevRotation; /**< Used for small bug fix, stores rotation of last update tick */
	float rotation; /**< Current rotation angle of the player (radians) */
	float targetRotation; /**< The next rotation step for the player */
	float speed; /**< the speed at which the player moves around the game */
	sf::Texture texture; 
	sf::Sprite sprite;
	sf::Texture uitexture; /**< For drawing a small UI version of the player */
	sf::Sprite uisprite; /**< For drawing a small UI version of the player */
	sf::Texture livesTexture; /**< Represents the amount of player lives are left */
	sf::Sprite livesSprite;
	float fireRate;  /**< How fast bullets are fired by the player */
	float fireTimer;  /**< works in tandom with the fireRate variable */
	float radius;  /**< circle collision variable */
	float health; 
	int speedTimer;  /**< How long the speed powerup will last */
	bool dead;

public:
	int lives;
	/**
	* Constructor for the player
	* Sets the location of the player as well as the starting attributes
	*/
	Player();

	/**
	* Loads the sprites and textures for the player and the UI pieces
	*/
	void loadResources();

	/**
	* Draws the player sprite at it's current location using the render window
	*/
	void Draw();

	/**
	* Draws the UI pieces for the player (lives / radar player) 
	*/
	void DrawUI();

	/**
	* Calls the various methods that control the player
	* Movement/ Wraparound
	*/
	void Update();

	/**
	* Controls the movement of the player
	* Uses the world mouse coordinates to aim the player at the mouse.
	* Space key is used to thrust the player in the direction currently facing
	*/
	void Movement();

	/**
	* If the player moves outside the bounds of the level, will be shifted to the otherside of the world.
	*/
	void WrapAround();

	/**
	* Centres the camera on the player, unless near the edge of the world.
	* Camera edge will not go off the side of the level, will start moving in that case.
	*/
	void CenterCamera();

	/**
	* Checks for the space key being pressed and if the attributes are right for a bullet to be fired.
	* Uses the timer and fire rate
	*/
	void Shoot();

	/**
	* IF the health of the player is below 0, will respawn in a random location
	* Also drops the lives of the player by one
	*/
	void Respawn();

	Pvector getPosition();
	
	/**
	* Used to rotate the player to the desired direction. 
	* Step: The speed at which it turns
	*/
	float CurveAngle(float from, float to, float step);

	Pvector Slerp(Pvector from, Pvector to, float step);

	/**
	* Normalises the Pvector 
	*/
	Pvector normalize(Pvector source);

	float degreeToRadian(float angle);

	float radiansToDegrees(float angle);

	float dotProduct(Pvector v1, Pvector v2);

	float getRadius();

	sf::Sprite getSprite();

	Pvector getDirection();

	float getHealth();

	/**
	* Lowers the player health by the sent amount (h)
	*/
	void dropHealth(float h);

	/**
	* Sets the speed timer to 10 seconds 
	* and increases the movement speed of the player for this given amount of time.
	*/
	void ActivateSpeed();

	/**
	* Allows the player to shoot strong bullets 
	* while the power timer is over 0.
	*/
	void ActivatePower();

	/**
	* For checking if the player can fire strong bullets.
	* Global for access.
	*/
	int powerTimer;
};