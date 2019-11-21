#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include "Brick.hpp"
#include <iostream>
using namespace std;

void Brick::setTexture(sf::Texture *t) {
	this->texture = t;
	this->sprite.setTexture(*t);
}
void Brick::draw(sf::RenderWindow *window) {
	//window->draw(this->text);
	window->draw(this->sprite);
}
bool Brick::getHasSpeedUpEffect() {
	return this->hasSpeedUpEffect;
}
bool Brick::gethasMakePaddleLargerEffect() {
	return this->hasMakePaddleLargerEffect;
}
bool Brick::checkIfHit(sf::Vector2f hitPosition) {
	float startXPosition = this->position.x - width / 2.0f;
	float endXPosition = this->position.x + width / 2.0f;
	float startYPosition = this->position.y - height / 2.0f;
	float endYPosition = this->position.y + height / 2.0f;

	if (hitPosition.x >= startXPosition && hitPosition.x < endXPosition) {
		if (hitPosition.y >= startYPosition && hitPosition.y < endYPosition) {
			cout << "BEFORE : " << this->noOfHitsRequired << endl;
			this->noOfHitsRequired--;
			this->text.setString(to_string(this->noOfHitsRequired));
			cout << "AFTER : " << this->noOfHitsRequired << endl;
			if (this->noOfHitsRequired < 0) {
				this->noOfHitsRequired = 0;
			}
			if (this->noOfHitsRequired == 0) {
				this->isDestroyed = true;
			}

			return true;
		}
	}
	return false;
}

int Brick::getNoOfHitsRequired() {
	return this->noOfHitsRequired;
}

sf::RectangleShape Brick::getBrickShape() {
	return this->shape;
}
float Brick::getWidth() {
	return this->width;
}
float Brick::getHeight() {
	return this->height;
}
int Brick::getId() {
	return this->id;
}
bool Brick::getIsDestroyed() {
	return this->isDestroyed;
}
void Brick::setIsDestroyed(bool b) {
	this->isDestroyed = b;
}