#pragma once
#include <iostream>
#include "Ball.hpp"
#include "Brick.hpp"
#include <SFML/Audio.hpp>

float Ball::getRadius() { return this->radius; }
float Ball::getX() { return this->position.x; }
float Ball::getY() { return this->position.y; }
float Ball::getOriginalX() { return this->originalPosition.x; }
float Ball::getOriginalY() { return this->originalPosition.y; }
float Ball::getSpeed() { return this->speed; }
float Ball::getOriginalSpeed() { return this->originalSpeed; }

void Ball::repositionBall(Paddle *paddle) {
	if (!this->hasStarted) {
		sf::Vector2f paddlePosition = paddle->getPaddleObj().getPosition();
		if (this->position.x != paddlePosition.x) {
			this->position.x = paddlePosition.x;
			this->ballObj.setPosition(this->position);
		}
	}
}

void Ball::reset() {
	this->hasStarted = false;
	this->position.x = this->originalPosition.x;
	this->position.y = this->originalPosition.y;
	this->dir = TOPRIGHT;
	this->directionOfBall = direction[this->dir];
	this->ballObj.setPosition(this->position);
}

void Ball::resetGame() {
	this->hasStarted = false;
	this->position.x = this->originalPosition.x;
	this->position.y = this->originalPosition.y;
	this->dir = TOPRIGHT;
	//(BallDirection)((rand() + 4) % 6);
	this->directionOfBall = direction[this->dir];
	this->speed = this->originalSpeed;
	this->ballObj.setPosition(this->position);
}
void Ball::increaseSpeed() {
	if (this->speed <= this->maxSpeed) {
		//cout << "INCREASE SPEED : OLD : " << speed << endl;
		this->speed = this->speed * (120.0f / 100.0f); //increase by 10 percent
		//cout << "INCREASE SPEED : NEW : " << speed << endl;
	}
}

void Ball::draw(sf::RenderWindow *window) {
	window->draw(this->ballObj);
}

void Ball::move(Paddle *paddle, float deltaTime, bool *pressedSpace, sf::Text *lifeLeftText, sf::Text *gameStatusText, bool *gameOver, vector<Brick> *bricks, sf::Text *scoreText, sf::Sound *soundLoseLife, sf::Sound *soundHitPaddle, sf::Sound *soundHitBrick, sf::Sound *soundHitWall) {
	if (*pressedSpace) {
		if (this->hasStarted == false) {
			cout << "entered the first if !" << endl;
			this->hasStarted = true;
			//launching ball
			sf::Vector2f paddlePosition = paddle->getPaddleObj().getPosition();
			if (paddlePosition.x <= 200.0f) {
				//paddle is on left half of screen
				//move towards right
				this->dir = TOPRIGHT;
				this->directionOfBall = direction[dir];
			}
			else {
				//paddle is on right half of screen
				//move towards left
				this->dir = TOPLEFT;
				this->directionOfBall = direction[dir];
			}
			//updating ball position
			this->position.x = this->position.x + this->directionOfBall.x * this->speed * deltaTime;
			this->position.y = this->position.y + this->directionOfBall.y * this->speed * deltaTime;

			this->ballObj.setPosition(this->position);
		}
		else {
			
			//checking if brick hit
			for (int i = 0; i < bricks->size(); i++) {
				Brick *b = &(bricks->at(i));
				if (b->checkIfHit(this->position)) {
					//bouncing the ball back
					this->position.y += 10.0f;
					this->ballObj.setPosition(this->position);
					switch (this->dir) {
					case TOPLEFT:
						dir = BOTTOMLEFT;
						break;
					case TOPRIGHT:
						dir = BOTTOMRIGHT;
						break;
					}
					this->directionOfBall = direction[dir];
					this->position.y = this->position.y + this->radius + this->directionOfBall.y * this->speed * deltaTime;
					//playing sound
					if (soundHitBrick->getStatus() != sf::Sound::Playing) {
						soundHitBrick->play();
					}
					
					if (b->getHasSpeedUpEffect()) {
						//if has speedup effect increase ball speed
						this->increaseSpeed();
					}
					if (b->gethasMakePaddleLargerEffect()) {
						paddle->IncreasePaddleWidth();
					}

					//removing the brick
					cout << "hit brick id " << b->getId() << " Required hit " << b->getNoOfHitsRequired() << endl;
					if (b->getNoOfHitsRequired() == 0) {
						bricks->erase(bricks->begin() + i);
					}
					paddle->increaseScore(10);
					scoreText->setString("Score : " + to_string(paddle->getScore()));
				}
			}

			//checking where ball has hit
			sf::Vector2f paddlePosition = paddle->getPaddleObj().getPosition();
			//checking for paddle
			float paddleStartX = paddlePosition.x - paddle->getWidth() / 2.0f;
			float paddleEndX = paddlePosition.x + paddle->getWidth() / 2.0f;

			if (this->position.y >= paddlePosition.y - paddle->getheight()/2.0f && 
				this->position.y <= paddlePosition.y + paddle->getheight() / 2.0f) {
				if (this->position.x >= paddleStartX && this->position.x <= paddleEndX) {
					//hits the paddle
					//repositioning ball
					this->position.y -= 20.0f;
					this->ballObj.setPosition(this->position);

					//playing sound
					if (soundHitPaddle->getStatus() != sf::Sound::Playing) {
						soundHitPaddle->play();
					}

					cout << "HIT PADDLE !" << this->position.x << " , " << this->position.y << endl;
					cout << "PADDLE START " << paddleStartX << " PADDLE END " << paddleEndX << endl;
					cout << "PADDLE Y " << paddlePosition.y << endl;
					//changing direction
					if (dir == BOTTOMLEFT) {
						dir = TOPLEFT;
					}
					else if (dir == BOTTOMRIGHT) {
						dir = TOPRIGHT;
					}
					this->directionOfBall = direction[dir];
				}
			}
			


			//checking for left and right edge
			if (this->position.x <= 2.0f * this->radius) {
				//left edge
				//playing sound
				if (soundHitWall->getStatus() != sf::Sound::Playing) {
					soundHitWall->play();
					soundHitBrick->stop();
					soundHitPaddle->stop();
					soundLoseLife->stop();
				}
				this->position.x = 2.0f * this->radius;
				this->ballObj.setPosition(this->position);

				switch (this->dir) {
					case TOPLEFT:
						dir = TOPRIGHT;
						break;
					case TOPRIGHT:
						dir = TOPLEFT;
						break;
					case BOTTOMLEFT:
						dir = BOTTOMRIGHT;
						break;
					case BOTTOMRIGHT:
						dir = BOTTOMLEFT;
						break;
				}
				this->directionOfBall = direction[dir];
			}

			if (this->position.x >= 400.0f - 2.0f * this->radius) {
				//right edge
				
				//playing sound
				if (soundHitWall->getStatus() != sf::Sound::Playing) {
					soundHitWall->play();
					soundHitBrick->stop();
					soundHitPaddle->stop();
					soundLoseLife->stop();
				}

				this->position.x = 400.0f - 2.0f * this->radius;
				this->ballObj.setPosition(this->position);

				switch (this->dir) {
				case TOPLEFT:
					dir = TOPRIGHT;
					break;
				case TOPRIGHT:
					dir = TOPLEFT;
					break;
				case BOTTOMLEFT:
					dir = BOTTOMRIGHT;
					break;
				case BOTTOMRIGHT:
					dir = BOTTOMLEFT;
					break;
				}
				this->directionOfBall = direction[dir];
			}

			if (this->position.y <= 2.0f * this->radius + 30.0f) {
				//top window edge
				//playing sound
				if (soundHitWall->getStatus() != sf::Sound::Playing) {
					soundHitWall->play();
					soundHitBrick->stop();
					soundHitPaddle->stop();
					soundLoseLife->stop();
				}

				this->position.y = 2.0f * this->radius + 30.0f;
				this->ballObj.setPosition(this->position);
				switch (this->dir) {
					case TOPLEFT:
						dir = BOTTOMLEFT;
						break;
					case TOPRIGHT:
						dir = BOTTOMRIGHT;
						break;
				}
				this->directionOfBall = direction[dir];
				this->position.y = this->position.y + this->radius + this->directionOfBall.y * this->speed * deltaTime;
			}
			else if (this->position.y >= 400.0f - this->radius) {
				//bottom window edge
				//playing lose life sound
				if (soundLoseLife->getStatus() != sf::Sound::Playing) {
					soundLoseLife->play();
				}

				paddle->decreaseLife();
				lifeLeftText->setString("Life : " + to_string(paddle->getLife()));
				
				cout << "LIFE DECREASED TO " << paddle->getLife();
				if (paddle->getLife() == 0) {
					*gameOver = true;
				}
				this->hasStarted = false;
				*pressedSpace = false;
				this->reset();
				this->repositionBall(paddle);
				return;
				/*this->position.y = 400.0f - 2.0f * this->radius - 10.0f;
				this->ballObj.setPosition(this->position);

				switch (this->dir) {
				case BOTTOMLEFT:
					dir = TOPLEFT;
					break;
				case BOTTOMRIGHT:
					dir = TOPRIGHT;
					break;
				}
				this->directionOfBall = direction[dir];
				this->position.y = this->position.y - this->radius + this->directionOfBall.y * this->speed * deltaTime;*/
			}

			//updating ball position
			this->position.x = this->position.x + this->directionOfBall.x * this->speed * deltaTime;
			this->position.y = this->position.y + this->directionOfBall.y * this->speed * deltaTime;

			this->ballObj.setPosition(this->position);
		}
	}
}