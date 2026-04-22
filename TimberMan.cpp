#include <SFML/Graphics.hpp>
#include <sstream>
#include <ctime>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

const int NUM_BRANCHES = 6;
enum class Side
{
	LEFT,
	RIGHT,
	NONE
};

Sprite branches[NUM_BRANCHES];
Side branchPositions[NUM_BRANCHES];

void updateBranches(int seed)
{
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
		branchPositions[j] = branchPositions[j - 1];

	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r)
	{
	case 0:
		branchPositions[0] = Side::LEFT;
		break;
	case 1:
		branchPositions[0] = Side::RIGHT;
		break;
	default:
		branchPositions[0] = Side::NONE;
		break;
	}
}

int main()
{
	VideoMode vm(1920, 1080);
	RenderWindow window(vm, "Timber Game!!!");

	View view(FloatRect(0, 0, 1920, 1080));
	window.setView(view);

	Texture textureBackground;
	textureBackground.loadFromFile("Assets/graphics/background.png");
	Sprite spriteBackground(textureBackground);

	Texture textureTree;
	textureTree.loadFromFile("Assets/graphics/tree.png");
	Sprite spriteTree(textureTree);
	spriteTree.setPosition(810, 0);

	Texture texturePlayer;
	texturePlayer.loadFromFile("Assets/graphics/player.png");
	Sprite spritePlayer(texturePlayer);
	spritePlayer.setPosition(580, 720);

	Texture textureAxe;
	textureAxe.loadFromFile("Assets/graphics/axe.png");
	Sprite spriteAxe(textureAxe);
	spriteAxe.setPosition(700, 830);

	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	Side playerSide = Side::LEFT;
	bool acceptInput = false;

	Texture textureBranch;
	textureBranch.loadFromFile("Assets/graphics/branch.png");

	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
		branchPositions[i] = Side::NONE;
	}

	updateBranches(1);
	updateBranches(2);
	updateBranches(3);
	updateBranches(4);
	updateBranches(5);

	Texture textureBee;
	textureBee.loadFromFile("Assets/graphics/bee.png");
	Sprite spriteBee(textureBee);
	spriteBee.setPosition(0, 800);

	bool beeActive = false;
	float beeSpeed = 0.0f;

	Texture textureCloud;
	textureCloud.loadFromFile("Assets/graphics/cloud.png");

	Sprite spriteCloud1(textureCloud);
	Sprite spriteCloud2(textureCloud);
	Sprite spriteCloud3(textureCloud);

	bool cloudActive1 = false;
	bool cloudActive2 = false;
	bool cloudActive3 = false;

	float cloudSpeed1 = 0.0f;
	float cloudSpeed2 = 0.0f;
	float cloudSpeed3 = 0.0f;

	Texture textureLog;
	textureLog.loadFromFile("Assets/graphics/log.png");
	Sprite spriteLog(textureLog);
	spriteLog.setPosition(810, 720);

	bool logActive = false;
	float logSpeedX = 0;
	float logSpeedY = -1500;

	Texture textureRIP;
	textureRIP.loadFromFile("Assets/graphics/rip.png");
	Sprite spriteRIP(textureRIP);
	spriteRIP.setPosition(2000, 800);

	Font font;
	font.loadFromFile("Assets/font/KOMIKAP_.ttf");

	//-------audio-------
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("Assets/sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("Assets/sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("Assets/sound/out_of_time.wav");
	Sound outOfTime;
	chop.setBuffer(ootBuffer);

	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(100);
	scoreText.setFillColor(Color::Red);
	scoreText.setPosition(20, 20);

	int score = 0;
	
	Text Highest_scoreText;
	Highest_scoreText.setFont(font);
	Highest_scoreText.setCharacterSize(35);
	Highest_scoreText.setFillColor(Color::Green);
	Highest_scoreText.setPosition(20, 120);
	int highestScore = 0;

	Text messageText;
	messageText.setFont(font);
	messageText.setCharacterSize(75);
	messageText.setFillColor(Color::Green);
	messageText.setString("Press Enter to Start");

	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(
		textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	bool paused = true;

	Clock clock;
	Time dt;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)
			{
				paused = false;
				timeRemaining = 6.0f;
				clock.restart();
				messageText.setString("");
				spritePlayer.setPosition(580, 720);
				score = 0;
				// reset branches
				for (int i = 0; i < NUM_BRANCHES; i++)
					branchPositions[i] = Side::NONE;

				updateBranches(1);
				updateBranches(2);
				updateBranches(3);
				updateBranches(4);
				updateBranches(5);

				// reset axe and rip
				spriteAxe.setPosition(2000, 830);
				spriteRIP.setPosition(2000, 720);
			}

			if (event.type == Event::KeyReleased && !paused)
			{
				acceptInput = true;
				spriteAxe.setPosition(2830, 830);
			}
		}

		if (!paused)
		{
			dt = clock.restart();
			timeRemaining -= dt.asSeconds();

			timeBar.setSize(
				Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f)
			{
				paused = true;
				messageText.setString("Out of Time!");
				outOfTime.play();
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(
					textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
				if (score > highestScore){
					highestScore = score;
				}
			}

			if (branchPositions[5] == playerSide)
			{
				paused = true;
				acceptInput = false;
				// highest score update
				if (score > highestScore){
					highestScore = score;
				}

				spritePlayer.setPosition(2000, 720);
				spriteAxe.setPosition(2000, 830);
				if (playerSide == Side::LEFT)
				{
					spriteRIP.setPosition(580, 720);
					death.play();
				}
				else if (playerSide == Side::RIGHT)
				{
					spriteRIP.setPosition(1200, 720);
					death.play();
				}
				messageText.setString("SQUISHED!!!");

				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(
					textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
			}

			if (!beeActive)
			{
				srand((int)time(0));
				beeSpeed = (rand() % 200) + 200;

				float height = (rand() % 500) + 500;

				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else
			{
				spriteBee.move(-beeSpeed * dt.asSeconds(), 0);

				if (spriteBee.getPosition().x < 100)
					beeActive = false;
			}

			if (!cloudActive1)
			{
				cloudSpeed1 = rand() % 200;
				float height = rand() % 150;
				spriteCloud1.setPosition(-200, height);
				cloudActive1 = true;
			}
			else
			{
				spriteCloud1.move(cloudSpeed1 * dt.asSeconds(), 0);
				if (spriteCloud1.getPosition().x > 1920)
					cloudActive1 = false;
			}

			if (!cloudActive2)
			{
				cloudSpeed2 = rand() % 200;
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloudActive2 = true;
			}
			else
			{
				spriteCloud2.move(cloudSpeed2 * dt.asSeconds(), 0);
				if (spriteCloud2.getPosition().x > 1920)
					cloudActive2 = false;
			}

			if (!cloudActive3)
			{
				cloudSpeed3 = rand() % 200;
				float height = rand() % 450 - 150;
				spriteCloud3.setPosition(-200, height);
				spriteCloud3.setScale(0.5f, 0.5f);
				cloudActive3 = true;
			}
			else
			{
				spriteCloud3.move(cloudSpeed3 * dt.asSeconds(), 0);
				if (spriteCloud3.getPosition().x > 1920)
					cloudActive3 = false;
			}

			if (acceptInput)
			{
				if (Keyboard::isKeyPressed(Keyboard::Left))
				{
					playerSide = Side::LEFT;
					spritePlayer.setPosition(580, 720);
					spriteAxe.setPosition(AXE_POSITION_LEFT, 830);

					score += 1;
					timeRemaining += (2 / score) + .15;

					spriteLog.setPosition(810, 720);
					logSpeedX = 5000;
					logActive = true;
					chop.play();
					updateBranches(score);
					acceptInput = false;
				}

				if (Keyboard::isKeyPressed(Keyboard::Right))
				{
					playerSide = Side::RIGHT;
					spritePlayer.setPosition(1200, 720);
					spriteAxe.setPosition(AXE_POSITION_RIGHT, 830);

					score++;
					timeRemaining += (2 / score) + .15;

					spriteLog.setPosition(810, 720);
					logSpeedX = -5000;
					logActive = true;
					chop.play();
					updateBranches(score);

					acceptInput = false;
				}
			}
			if (logActive)
			{
				spriteLog.move(logSpeedX * dt.asSeconds(),
							   logSpeedY * dt.asSeconds());

				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
				{
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}
			stringstream ss;
			ss << "Score=" << score;
			scoreText.setString(ss.str());

			stringstream hs;
			hs << "Highest Score=" << highestScore;
			Highest_scoreText.setString(hs.str());

			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;

				if (branchPositions[i] == Side::LEFT)
				{
					branches[i].setPosition(610, height);
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == Side::RIGHT)
				{
					branches[i].setPosition(1330, height);
					branches[i].setRotation(0);
				}
				else
				{
					branches[i].setPosition(3000, height);
				}
			}
		}

		window.clear();

		window.draw(spriteBackground);

		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		window.draw(spriteTree);
		window.draw(spriteLog);

		window.draw(spritePlayer);
		window.draw(spriteAxe);
		window.draw(spriteRIP);

		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}

		window.draw(spriteBee);
		window.draw(scoreText);
		window.draw(Highest_scoreText);
		window.draw(timeBar);

		if (paused)
			window.draw(messageText);

		window.display();
	}
}
