#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>

using namespace std;

class Brick {
private:
	int id;
	int noOfHitsRequired;
	float width;
	float height;
	bool isDestroyed;
	bool hasSpeedUpEffect;
	bool hasMakePaddleLargerEffect;
	sf::Vector2f position;
	sf::Texture *texture;
	sf::RectangleShape shape;
	sf::Sprite sprite;
	sf::Text text;
	sf::Font font;
public:
	Brick(int id, sf::Vector2f position, float width, float height, sf::Texture *texture, int noOfHitsRequired, bool hasSpeedUpEffect, bool hasMakePaddleLargerEffect) {
		this->id = id;
		this->noOfHitsRequired = noOfHitsRequired;
		this->position = position;
		this->width = width;
		this->height = height;
		this->texture = texture;
		this->hasSpeedUpEffect = hasSpeedUpEffect;
		this->hasMakePaddleLargerEffect = hasMakePaddleLargerEffect;
		this->text.setString(to_string(noOfHitsRequired));

		sprite.setTexture(*texture);
		sprite.setOrigin(this->width / 2.0f, this->height / 2.0f);
		//sprite.setScale(sf::Vector2f(0.05f, 0.02f));
		sprite.setPosition(this->position);
		sf::IntRect textureRect;
		textureRect.width = this->width;
		textureRect.height = this->height;
		textureRect.left = this->position.x - this->width / 2.0f;
		textureRect.top = this->position.y - this->height / 2.0f;
		sprite.setTextureRect(textureRect);

		
		if (!(font.loadFromFile("arial.ttf"))) {
			cout << "ERROR LOADING FONT!" << endl;
		}
		else {
			cout << "FONT LOADED!" << endl;
		}

		this->text.setCharacterSize(20);
		this->text.setFillColor(sf::Color::White);
		this->text.setPosition(this->width / 2.0f, this->height / 2.0f);
		this->text.setFont(font);
		this->text.setOrigin(0, 0);
		
		//shape.setSize(sf::Vector2f(this->width, this->height));
		//shape.setOutlineThickness(1);
		//shape.setPosition(this->position);
		//shape.setOrigin(width / 2.0f, height / 2.0f); //centre
		//shape.setFillColor(sf::Color::Blue);
		//shape.setTexture(texture);
	}
	~Brick() {

	}
	
	void draw(sf::RenderWindow *window);
	bool checkIfHit(sf::Vector2f hitPosition);

	sf::RectangleShape getBrickShape();
	float getWidth();
	float getHeight();
	int getId();
	bool getIsDestroyed();
	int getNoOfHitsRequired();
	void setIsDestroyed(bool b);
	sf::Text getText();
	bool getHasSpeedUpEffect();
	bool gethasMakePaddleLargerEffect();
	void setTexture(sf::Texture *t);
};
