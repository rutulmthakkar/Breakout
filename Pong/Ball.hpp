#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include "paddle.hpp"
#include "Brick.hpp"
#include <SFML/Audio.hpp>

using namespace std;
const enum BallDirection { STOP = -1, LEFT, TOPLEFT, BOTTOMLEFT, RIGHT, TOPRIGHT, BOTTOMRIGHT };
//vectors representing directions (0 -> left, 1 -> top left, 2 -> bottom left, 3 -> right, 4 -> top right, 5 -> bottom right)
const sf::Vector2f direction[6] = { sf::Vector2f(-1.0f,0.0f),sf::Vector2f(-0.707f,-0.707f),sf::Vector2f(-0.707f,0.707f),sf::Vector2f(1.0f,0.0f),sf::Vector2f(0.707f,-0.707f),sf::Vector2f(0.707f,0.707f) };

class Ball {
private:
	sf::Vector2f position;
	sf::Vector2f originalPosition;
	float radius;
	sf::Vector2f directionOfBall;
	BallDirection dir;
	float speed;
	float originalSpeed;
	float maxSpeed;
	sf::CircleShape ballObj;
	bool hasStarted;

public:
	Ball(sf::Vector2f position, float radius) {
		this->position = position;
		this->originalPosition = position;
		this->radius = radius;
		this->dir = TOPRIGHT;
		//(BallDirection)((rand() + 4) % 6); 
		cout << "Direction : " << dir << endl;
		this->directionOfBall = direction[dir];
		this->speed = 0.4f;
		this->originalSpeed = 0.4f;
		this->maxSpeed = 2.0f;
		this->hasStarted = false;

		sf::CircleShape shape(radius);
		shape.setFillColor(sf::Color::White);
		shape.setPosition(position);
		shape.setOrigin(radius, radius); // centre
		ballObj = shape;
	}

	float getRadius();
	float getX();
	float getY();
	float getOriginalX();
	float getOriginalY();
	float getSpeed();
	float getOriginalSpeed();
	void repositionBall(Paddle *paddle);
	void reset();
	void resetGame();
	void increaseSpeed();
	void draw(sf::RenderWindow *window);
	void move(Paddle *paddle, float deltaTime, bool *pressedSpace, sf::Text *lifeLeftText, sf::Text *gameStatusText, bool *gameOver, vector<Brick> *bricks, sf::Text *scoreText, sf::Sound *soundLoseLife, sf::Sound *soundHitPaddle, sf::Sound *soundHitBrick, sf::Sound *soundHitWall);

};

