#pragma once
#include <iostream>
#include "paddle.hpp"

void Paddle::setPosition(float x) {
	this->x = x;
	this->paddleObj.setPosition(this->x, this->y);
}

int Paddle::getScore() {
	return this->score;
}

void Paddle::increaseScore(int amt) {
	this->score += amt;
}
void Paddle::IncreasePaddleWidth() {
	this->width = this->width * 120.0f / 100.0f;
	this->paddleObj.setSize(sf::Vector2f(this->width, this->height));
	this->paddleObj.setOrigin(paddleObj.getSize().x / 2.0f, paddleObj.getSize().y / 2.0f);
}
int Paddle::getSpeed() { return this->speed; }
float Paddle::getWidth() { return this->width; }
float Paddle::getheight() { return this->height; }
float Paddle::getXPosition() { return this->x; }
float Paddle::getYPosition() { return this->y; }
float Paddle::getOriginalXPosition() { return this->originalX; }
float Paddle::getOriginalYPosition() { return this->originalY; }
void Paddle::reset() { this->x = this->originalX; this->y = this->originalY; this->paddleObj.setPosition(this->x, this->y); }
int Paddle::getLife() { return this->life; }
void Paddle::decreaseLife() { this->life--; }
sf::Vector2f Paddle::getOriginOfPaddle() { return this->paddleObj.getOrigin(); }
void Paddle::resetGame() { reset(); this->life = 3; this->score = 0; this->width = this->originalWidth; }
sf::RectangleShape Paddle::getPaddleObj() { return this->paddleObj; }
void Paddle::draw(sf::RenderWindow *window) {
	window->draw(this->paddleObj);
}

void Paddle::move(int amt,int direction) {
	if (direction == -1) {
		//move left
		this->x = this->x - amt;
		if (this->x < (0 + this->width/2.0f + 10.0f)) {
			this->x = 0 + this->width/2.0f + 10.0f;
		}
		this->paddleObj.setPosition(this->x, this->y);
	}
	else if(direction == 1) {
		//move right
		this->x = this->x + amt;
		if (this->x > (400.0f - this->width/2.0f - 10.0f)) {
			this->x = 400.0f - this->width/2.0f - 10.0f;
		}
		this->paddleObj.setPosition(this->x, this->y);
	}
}