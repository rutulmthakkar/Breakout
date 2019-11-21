/*
Author: Rutul Thakkar
Course: Game 230 Game Engineering
Date: 17 Nov 2019

Done Main Parts: A,B,C,D,E,F,G,H,I,J,K
Extra Done : L,M,N

-> More than one time hit required to destroy brick (determined by noOfHitsRequired attribute) (objective M)
-> bricks that speed up ball (objective M) 
-> Multiple textures
-> multipe levels (0,1) (objective L)
-> Sounds integrated for all types of events
-> has powerup to make paddle width bigger (objective N)
-> texture changes depending on number of hits required to destroy the brick is left
1 -> orange
2 -> Blue
3 -> red
4 -> green
-> any brick with special effect are rendered with grey texture

*/
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "paddle.hpp"
#include "Ball.hpp"
#include "Brick.hpp"
#include <iostream>

using namespace std;
float deltaTime = 1.16f;
const int TOTAL_LEVELS = 2;
int currentLevel = 0;
bool isArrayBeingFilled = false;
sf::SoundBuffer sbHitBrick;
sf::SoundBuffer sbLoseLife;
sf::SoundBuffer sbHitPaddle;
sf::SoundBuffer sbGameOver;
sf::SoundBuffer sbHitWall;
sf::SoundBuffer sblevelup;

sf::Sound soundHitBrick;
sf::Sound soundLoseLife;
sf::Sound soundHitPaddle;
sf::Sound soundGameOver;
sf::Sound soundHitWall;
sf::Sound soundlevelup;

void populateBricks(vector<Brick> *bricks, sf::Texture *t, sf::Texture *tOval, sf::Texture *tblue, sf::Texture *tRed);
void populateBricksLevel2(vector<Brick> *bricks, sf::Texture *t, sf::Texture *tblue, sf::Texture *tOval, sf::Texture *tGreen, sf::Texture *tRed);
void loadSound();

int main()
{
	bool movePaddle = false;
	int directionMovePaddle = 0;
	bool gameOver = false;
	bool pressedSpace = false;
	bool hasPlayedGameOverSound = false;
	Paddle paddle(200.0f, 350.0f, 100.0f, 20.0f);
	Ball ball(sf::Vector2f(200.0f, 330.0f), 10.0f);
	sf::Texture t;
	if (!t.loadFromFile("texture.png")) {
		cout << "ERROR LOADING TEXTURE in MAIN !" << endl;
	}
	else {
		cout << "SUCCESS LOADED TEXTURE MAIN" << endl;
	}
	sf::Texture tBlue;
	if (!tBlue.loadFromFile("blue.jpg")) {
		cout << "ERROR LOADING TEXTURE BLUE in MAIN !" << endl;
	}
	else {
		cout << "SUCCESS LOADED TEXTURE BLUE MAIN" << endl;
	}
	sf::Texture tRed;
	if (!tRed.loadFromFile("red.jpg")) {
		cout << "ERROR LOADING TEXTURE RED in MAIN !" << endl;
	}
	else {
		cout << "SUCCESS LOADED TEXTURE RED MAIN" << endl;
	}

	sf::Texture tGreen;
	if (!tGreen.loadFromFile("green.jpg")) {
		cout << "ERROR LOADING TEXTURE GREEN in MAIN !" << endl;
	}
	else {
		cout << "SUCCESS LOADED TEXTURE GREEN MAIN" << endl;
	}
	sf::Texture tOval;
	if (!tOval.loadFromFile("grey.jpg")) {
		cout << "ERROR LOADING TEXTURE GREY in MAIN !" << endl;
	}
	else {
		cout << "SUCCESS LOADED TEXTURE GREY MAIN" << endl;
	}
	loadSound();
	vector<Brick> bricks;
	populateBricks(&bricks, &t, &tOval, &tGreen, &tRed);

	sf::RenderWindow window(sf::VideoMode(400, 400), "Breakout");
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	sf::Font font;
	if (!(font.loadFromFile("arial.ttf"))) {
		cout << "ERROR LOADING FONT!" << endl;
	}
	else {
		cout << "FONT LOADED!" << endl;
	}
	sf::Text lifeLeftText;
	sf::Text scoreText;
	sf::Text gameStatusText;
	sf::Text levelText;

	levelText.setString("Level : " + to_string(currentLevel + 1));
	levelText.setFont(font);
	levelText.setCharacterSize(20);
	levelText.setOrigin(0, 0);
	levelText.setFillColor(sf::Color::White);
	levelText.setStyle(sf::Text::Bold);
	levelText.setPosition(sf::Vector2f(120, 360));

	lifeLeftText.setString("Life : " + to_string(paddle.getLife()));
	lifeLeftText.setFont(font);
	lifeLeftText.setCharacterSize(20);
	lifeLeftText.setOrigin(0, 0);
	lifeLeftText.setFillColor(sf::Color::White);
	lifeLeftText.setStyle(sf::Text::Bold);
	lifeLeftText.setPosition(sf::Vector2f(25, 360));

	scoreText.setString("Score : " + to_string(paddle.getScore()));
	scoreText.setFont(font);
	scoreText.setCharacterSize(20);
	scoreText.setOrigin(0, 0);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setStyle(sf::Text::Bold);
	scoreText.setPosition(sf::Vector2f(250, 360));


	gameStatusText.setString("GAME OVER\nPress Space to restart!");
	gameStatusText.setFont(font);
	gameStatusText.setCharacterSize(25);
	gameStatusText.setOrigin(0, 0);
	gameStatusText.setFillColor(sf::Color::White);
	gameStatusText.setStyle(sf::Text::Bold);
	gameStatusText.setPosition(sf::Vector2f(50, 150));

	sf::Clock clock;
	clock.restart();
	sf::Mouse mouse;
	sf::Vector2i mousePosition = mouse.getPosition(window);
	sf::Vector2f oldMousePositionFloat(mousePosition.x * 1.0f, mousePosition.y * 1.0f);

	while (window.isOpen())
	{
		deltaTime = clock.getElapsedTime().asMilliseconds();
		clock.restart();
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::MouseMoved && !gameOver)
			{
				sf::Vector2f newMousePositionFloat(event.mouseMove.x * 1.0f, event.mouseMove.y * 1.0f);
				if (newMousePositionFloat.x < oldMousePositionFloat.x && paddle.getXPosition() > newMousePositionFloat.x) {
					movePaddle = true;
					directionMovePaddle = -1;
				}
				else if(newMousePositionFloat.x > oldMousePositionFloat.x && paddle.getXPosition() < newMousePositionFloat.x) {
					movePaddle = true;
					directionMovePaddle = 1;
				}
				else {
					movePaddle = false;
					directionMovePaddle = 0;
				}
				oldMousePositionFloat = newMousePositionFloat;
			}
			else {
				movePaddle = false;
				directionMovePaddle = 0;
			}

			/*if (event.type == sf::Event::MouseEntered)
			{
				mousePosition = mouse.getPosition(window);
				sf::Vector2f newMousePositionFloat(mousePosition.x * 1.0f, mousePosition.y * 1.0f);
				if (newMousePositionFloat.x < oldMousePositionFloat.x) {
					movePaddle = true;
					directionMovePaddle = -1;
				}
				else if (newMousePositionFloat.x > oldMousePositionFloat.x) {
					movePaddle = true;
					directionMovePaddle = 1;
				}
				else {
					movePaddle = false;
					directionMovePaddle = 0;
				}
				oldMousePositionFloat = newMousePositionFloat;
			}*/

			if (event.type == sf::Event::MouseLeft) {
				movePaddle = false;
				directionMovePaddle = 0;
			}
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (!gameOver) {
						pressedSpace = true;
						ball.move(&paddle, deltaTime, &pressedSpace, &lifeLeftText, &gameStatusText, &gameOver, &bricks, &scoreText, &soundLoseLife, &soundHitPaddle, &soundHitBrick, &soundHitWall);
						if (paddle.getLife() == 0) {
							gameOver = true;
							hasPlayedGameOverSound = false;
							gameStatusText.setString("GAME OVER\nPress Space to restart!");
							pressedSpace = false;
							currentLevel = 0;
						}
					}
					else {
						//restart game
						currentLevel = 0;
						populateBricks(&bricks, &t, &tOval, &tGreen, &tRed);
						paddle.resetGame();
						ball.resetGame();
						pressedSpace = false;
						gameOver = false;
						hasPlayedGameOverSound = false;
						gameStatusText.setString("GAME OVER\nPress Space to restart!");
						lifeLeftText.setString("Life : " + to_string(paddle.getLife()));
						scoreText.setString("Score : " + to_string(paddle.getScore()));
					}

				}
			}

			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right) {
					movePaddle = false;
					directionMovePaddle = 0;
				}
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Left && !gameOver) {
					movePaddle = true;
					directionMovePaddle = -1;
					/*paddle.move(paddle.getSpeed(), -1);
					ball.repositionBall(&paddle);*/
				}
				if (event.key.code == sf::Keyboard::Right && !gameOver) {
					movePaddle = true;
					directionMovePaddle = 1;
					/*paddle.move(paddle.getSpeed(), 1);
					ball.repositionBall(&paddle);*/
				}

				if (event.key.code == sf::Keyboard::Space) {
					if (!gameOver) {
						pressedSpace = true;
						ball.move(&paddle, deltaTime, &pressedSpace, &lifeLeftText, &gameStatusText, &gameOver, &bricks, &scoreText, &soundLoseLife, &soundHitPaddle, &soundHitBrick, &soundHitWall);
						if (paddle.getLife() == 0) {
							gameOver = true;
							hasPlayedGameOverSound = false;
							gameStatusText.setString("GAME OVER\nPress Space to restart!");
							pressedSpace = false;
							currentLevel = 0;
						}
					}
					else {
						//restart game
						currentLevel = 0;
						populateBricks(&bricks, &t, &tOval, &tGreen, &tRed);
						paddle.resetGame();
						ball.resetGame();
						pressedSpace = false;
						gameOver = false;
						hasPlayedGameOverSound = false;
						gameStatusText.setString("GAME OVER\nPress Space to restart!");
						lifeLeftText.setString("Life : " + to_string(paddle.getLife()));
						scoreText.setString("Score : " + to_string(paddle.getScore()));
					}
					
				}
			}
		}

		if (movePaddle && directionMovePaddle != 0) {
			paddle.move(paddle.getSpeed(), directionMovePaddle);
			ball.repositionBall(&paddle);
		}

		window.clear();
		window.draw(lifeLeftText);
		window.draw(scoreText);
		window.draw(levelText);
		if (gameOver) {
			if (soundGameOver.getStatus() != sf::Sound::Playing && !hasPlayedGameOverSound) {
				hasPlayedGameOverSound = true;
				soundGameOver.play();
			}
			window.draw(gameStatusText);
		}
		else {
			//cout << "TOTAL BRICKS LEFT : " << bricks.size() << endl;
			if (bricks.size() == 0 && !isArrayBeingFilled) {
				isArrayBeingFilled = true;
				//cout << "OLD LEVEL " << currentLevel << endl;
				currentLevel++;
				//cout << "NEW LEVEL " << currentLevel << endl;
				if (currentLevel >= TOTAL_LEVELS) {
					//cout << "INSIDE IF CHANGING TO 0 " << endl;
					currentLevel = 0;
				}
				levelText.setString("Level : " + to_string(currentLevel + 1));
				if (soundlevelup.getStatus() != sf::Sound::Playing) {
					soundlevelup.play();
				}
			}
			if (bricks.size() == 0 && ball.getY() >= 220.0f) {
				switch (currentLevel) {
					case 0:
						populateBricks(&bricks, &t, &tOval, &tBlue, &tRed);
						break;
					case 1:
						populateBricksLevel2(&bricks, &t, &tBlue, &tOval, &tGreen, &tRed);
						break;
					default:
						populateBricks(&bricks, &t, &tOval, &tBlue, &tRed);
						break;
				}
				
				ball.increaseSpeed();
			}
		}
		paddle.draw(&window);
		for (int i = 0; i < bricks.size(); i++) {
			if (!bricks[i].getHasSpeedUpEffect() && !bricks[i].gethasMakePaddleLargerEffect()) {
				switch (bricks[i].getNoOfHitsRequired()) {
					case 1:
						bricks[i].setTexture(&t);
						break;
					case 2:
						bricks[i].setTexture(&tBlue);
						break;
					case 3:
						bricks[i].setTexture(&tRed);
						break;
					case 4:
						bricks[i].setTexture(&tGreen);
						break;
				}
			}
			
			bricks[i].draw(&window);
		}
		ball.draw(&window);

		ball.move(&paddle, deltaTime, &pressedSpace, &lifeLeftText, &gameStatusText, &gameOver, &bricks, &scoreText, &soundLoseLife, &soundHitPaddle, &soundHitBrick, &soundHitWall);
		window.display();
	}

	return 0;
}

void populateBricks(vector<Brick> *bricks, sf::Texture *t, sf::Texture *tOval, sf::Texture *tblue, sf::Texture *tRed) {
	float brickWidth = 80.0f;
	float brickHeight = 25.0f;
	float posXRow1 = 70.0f;
	float posYRow1 = 50.0f;
	float posXRow2 = 50.0f;
	float posYRow2 = 80.0f;
	float posXRow3 = 70.0f;
	float posYRow3 = 110.0f;
	bricks->clear();
	for (int i = 0; i < 13; i++) {
		if (i < 4) {
			//row 1 4 bricks
			Brick brick1(i + 1, sf::Vector2f(posXRow1, posYRow1), brickWidth, brickHeight, t, 1, false, false);
			posXRow1 += 85.0f;
			bricks->push_back(brick1);
		}
		else if (i >= 4 && i <= 8) {
			//row 2 5 bricks
			if (i == 6) {
				//adding speed up 
				Brick brick2(i + 1, sf::Vector2f(posXRow2, posYRow2), brickWidth - 10.0f, brickHeight, tOval, 1, true, false);
				bricks->push_back(brick2);
			}
			else {
				Brick brick22(i + 1, sf::Vector2f(posXRow2, posYRow2), brickWidth - 10.0f, brickHeight, tblue, 2, false, false);
				bricks->push_back(brick22);
			}

			posXRow2 += 75.0f;
			
		}
		else {
			//row 3 4 bricks
			Brick brick3(i + 1, sf::Vector2f(posXRow3, posYRow3), brickWidth, brickHeight, t, 1, false, false);
			posXRow3 += 85.0f;
			bricks->push_back(brick3);
		}
	}
	//cout << "TOTAL BRICKS " << bricks->size() << endl;
	isArrayBeingFilled = false;
}

void populateBricksLevel2(vector<Brick> *bricks, sf::Texture *t, sf::Texture *tblue, sf::Texture *tOval, sf::Texture *tGreen, sf::Texture *tRed) {
	float brickWidth = 80.0f;
	float brickHeight = 25.0f;
	float posXRow1 = 70.0f;
	float posYRow1 = 50.0f;

	float posXRow2 = 50.0f;
	float posYRow2 = 80.0f;
	
	float posXRow3 = 50.0f;
	float posYRow3 = 110.0f;

	float posXRow4 = 70.0f;
	float posYRow4 = 140.0f;
	bricks->clear();
	for (int i = 0; i < 18; i++) {
		if (i < 4) {
			//row 1 4 bricks
			Brick brick1(i + 1, sf::Vector2f(posXRow1, posYRow1), brickWidth, brickHeight, tRed, 3, false, false);
			posXRow1 += 85.0f;
			bricks->push_back(brick1);
		}
		else if (i >= 4 && i <= 8) {
			//row 2 5 bricks
			if (i == 6) {
				//adding speed up 
				Brick brick2(i + 1, sf::Vector2f(posXRow2, posYRow2), brickWidth - 10.0f, brickHeight, tOval, 1, true, false);
				bricks->push_back(brick2);
			}
			else {
				Brick brick22(i + 1, sf::Vector2f(posXRow2, posYRow2), brickWidth - 10.0f, brickHeight, tblue, 2, false, false);
				bricks->push_back(brick22);
			}
			posXRow2 += 75.0f;
		}
		else if (i >= 9 && i <= 13) {
			//row 3 5 bricks
			if (i == 11) {
				//adding increase width
				Brick brick3(i + 1, sf::Vector2f(posXRow3, posYRow3), brickWidth - 10.0f, brickHeight, tOval, 1, false, true);
				bricks->push_back(brick3);
			}
			else {
				Brick brick32(i + 1, sf::Vector2f(posXRow3, posYRow3), brickWidth - 10.0f, brickHeight, tblue, 2, false, false);
				bricks->push_back(brick32);
			}
			posXRow3 += 75.0f;
		}
		else {
			//row 4 4 bricks
			Brick brick4(i + 1, sf::Vector2f(posXRow4, posYRow4), brickWidth, brickHeight, t, 1, false, false);
			posXRow4 += 85.0f;
			bricks->push_back(brick4);
		}
	}

	//cout << "TOTAL BRICKS " << bricks->size() << endl;
	isArrayBeingFilled = false;
}

void loadSound() {
	if (!sbHitBrick.loadFromFile("brick.wav")) {
		cout << "FAILED TO LOAD BRICK.WAV !" << endl;
	}
	else {
		cout << "SUCCESSFULLY LOADED BRICK.WAV !" << endl;
	}
	soundHitBrick.setBuffer(sbHitBrick);
	soundHitBrick.setVolume(100.0f);

	if (!sbHitPaddle.loadFromFile("paddle.wav")) {
		cout << "FAILED TO LOAD PADDLE.WAV !" << endl;
	}
	else {
		cout << "SUCCESSFULLY LOADED PADDLE.WAV !" << endl;
	}
	soundHitPaddle.setBuffer(sbHitPaddle);
	soundHitPaddle.setVolume(100.0f);

	if (!sbGameOver.loadFromFile("gameover.wav")) {
		cout << "FAILED TO LOAD GAMEOVER.WAV !" << endl;
	}
	else {
		cout << "SUCCESSFULLY LOADED GAMEOVER.WAV !" << endl;
	}
	soundGameOver.setBuffer(sbGameOver);
	soundGameOver.setVolume(100.0f);

	if (!sbLoseLife.loadFromFile("loselife.wav")) {
		cout << "FAILED TO LOAD LOSELIFE.WAV !" << endl;
	}
	else {
		cout << "SUCCESSFULLY LOADED LOSELIFE.WAV !" << endl;
	}
	soundLoseLife.setBuffer(sbLoseLife);
	soundLoseLife.setVolume(100.0f);

	if (!sbHitWall.loadFromFile("wall.wav")) {
		cout << "FAILED TO LOAD WALL.WAV !" << endl;
	}
	else {
		cout << "SUCCESSFULLY LOADED WALL.WAV !" << endl;
	}
	soundHitWall.setBuffer(sbHitWall);
	soundHitWall.setVolume(100.0f);
	//soundHitWall.setPitch(2.0f);

	if (!sblevelup.loadFromFile("levelup.wav")) {
		cout << "FAILED TO LOAD LEVELUP.WAV !" << endl;
	}
	else {
		cout << "SUCCESSFULLY LOADED LEVELUP.WAV !" << endl;
	}
	soundlevelup.setBuffer(sblevelup);
	soundlevelup.setVolume(100.0f);
}