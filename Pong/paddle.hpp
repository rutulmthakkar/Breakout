#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#ifndef PADDLE_H
#define PADDLE_H

class Paddle {
private:
	int life;
	int score;
	float x;
	float y;
	float originalX;
	float originalY;
	float width;
	float originalWidth;
	float height;
	int speed;
	sf::RectangleShape paddleObj;
public:
	Paddle(float x, float y, float width, float height) {
		this->life = 3;
		this->score = 0;
		this->x = x;
		this->y = y;
		this->originalX = x;
		this->originalY = y;
		this->width = width;
		this->originalWidth = width;
		this->height = height;
		this->speed = 20;
		paddleObj.setSize(sf::Vector2f(this->width, this->height));
		paddleObj.setOutlineThickness(1);
		paddleObj.setPosition(x, y);
		paddleObj.setOrigin(paddleObj.getSize().x / 2.0f, paddleObj.getSize().y / 2.0f); //origin is centre
	}
	int getSpeed();
	float getWidth();
	float getheight();
	float getXPosition();
	float getYPosition();
	float getOriginalXPosition();
	float getOriginalYPosition();
	void reset();
	int getLife();
	void decreaseLife();
	sf::Vector2f getOriginOfPaddle();
	void resetGame();
	sf::RectangleShape getPaddleObj();
	void draw(sf::RenderWindow *window);
	void move(int amt, int direction);
	int getScore();
	void increaseScore(int amt);
	void IncreasePaddleWidth();
};
#endif