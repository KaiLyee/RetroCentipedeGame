/*
Author: Junkai Li
Class: ECE6122
Last Date Modified: 9/27/2024
Description:
The objective of this game is to replicate the gameplay of the classic arcade game "Centipede,"
featuring a centipede that moves across the screen while players attempt to shoot it down using a laser. 
The game is designed for a single level and ends when either all segments of the centipede are destroyed or the player runs out of lives.
*/
// Include important C++ libraries
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "ECE_Centipede.h"
#include "ECE_LaserBlast.h"
#include <list> 
#include <random>

using namespace sf;

// Constants
const int NUM_MUSHROOMS = 30;      // Number of mushrooms in the game
const int FIRE_RATE = 240;         // Fire rate: frames between laser shots

// Global variables
std::list<sf::Sprite> mushrooms;                 // List of mushrooms
std::list<ECE_LaserBlast> laserBlasts;           // List of laser blasts
sf::Sprite spaceship;                            // Player's spaceship sprite
sf::Sprite spider;                               // Spider enemy sprite
std::list<ECE_Centipede*> centipedeSegments;     // List of centipede segments (head and body)
sf::Sprite background;                           // Background sprite
sf::Sprite StartupScreen;                        // Startup screen sprite
std::vector<sf::Sprite> spaceshipIcons;          // Spaceship icons for remaining lives display
int spaceshipLives = 3;                          // Initial number of spaceship lives
sf::Vector2f initialSpaceshipPosition;           // Initial position of the spaceship
sf::Font font;                                   // Font for text display
sf::Text scoreText;                              // Text to display score
sf::Text gameOverText;                           // Text for Game Over screen
sf::Text finalScoreText;                         // Text to display final score
sf::Text gameRestartText;                        // Text for restarting the game
std::random_device rd;                           // Random device for random number generation
std::mt19937 gen(rd());                          // Mersenne Twister random number generator

// Textures
sf::Texture textureStarship;               // Texture for the spaceship
sf::Texture textureSpider;                 // Texture for the spider
sf::Texture textureBackground;             // Texture for the background
sf::Texture textureMashroom0, textureMashroom1;  // Textures for mushrooms (different states)
sf::Texture textureCentipedeBody;          // Texture for centipede body segments
sf::Texture textureCentipedeHead;          // Texture for the centipede head
sf::Texture textureLaser;                  // Texture for laser blasts
sf::Texture textureStartupScreen;          // Texture for the startup screen

// Game state variables
bool paused = true;                        // Game paused state
int score = 0;                             // Player's score
int frameCounter = 0;                      // Frame counter (for timing events)
int laserFireCounter = 0;                  // Counter for controlling laser fire intervals
bool ended = false;                        // Game end state
bool playerWon = false;                    // Track if the player won the game

// Function declarations
void initGame(RenderWindow& window);       // Initialize the game
void updateGame();                         // Update game state (called every frame)
void drawGame(sf::RenderWindow& window, bool paused);  // Render the game scene
void updateSpiderMovement();               // Update spider movement
void handleSpaceshipHit();
bool isOverlapping(const sf::Sprite& sprite1, const sf::Sprite& sprite2); // Check sprite collision
bool checkSpaceshipMushroomCollision();    // Check for collisions between spaceship and mushrooms


int main()
{
	// Create a video mode object with lower resolution
	VideoMode vm(960, 540);
	RenderWindow window(vm, "Centipede!!!");

	// Set the view to match the window size
	View view(sf::FloatRect(0, 0, 960, 540));
	window.setView(view);

	// Control variables for player input and game state
	bool acceptInput = false;

	// Initialize game elements
	initGame(window);

	// Main game loop
	while (window.isOpen())
	{
		// Draw the game (renders all the elements)
		drawGame(window, paused);

		// Handle player input
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				acceptInput = true;  // Accept player input when a key is released
			}
		}

		// Check for Escape key to close the window
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Start the game when Return/Enter is pressed
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;  // Unpause the game
			background.setTexture(textureBackground);  // Set gameplay background
			background.setPosition(0, 0);
			window.clear();  // Clear the window
			score = 0;  // Reset score
			acceptInput = true;  // Allow player input
		}

		// Player movement and actions when input is accepted
		if (acceptInput)
		{
			// Spaceship movement controls (left, right, up, down)
			if (Keyboard::isKeyPressed(Keyboard::Left) && spaceship.getPosition().x > 0)
			{
				spaceship.move(-0.3f, 0);
				if (checkSpaceshipMushroomCollision()) spaceship.move(0.2f, 0);  // Undo movement if collision occurs
			}

			if (Keyboard::isKeyPressed(Keyboard::Right) && spaceship.getPosition().x < 940)
			{
				spaceship.move(0.3f, 0);
				if (checkSpaceshipMushroomCollision()) spaceship.move(-0.2f, 0);  // Undo movement if collision occurs
			}

			if (Keyboard::isKeyPressed(Keyboard::Up) && spaceship.getPosition().y > 0)
			{
				spaceship.move(0, -0.3f);
				if (checkSpaceshipMushroomCollision()) spaceship.move(0, 0.2f);  // Undo movement if collision occurs
			}

			if (Keyboard::isKeyPressed(Keyboard::Down) && spaceship.getPosition().y < 510)
			{
				spaceship.move(0, 0.3f);
				if (checkSpaceshipMushroomCollision()) spaceship.move(0, -0.2f);  // Undo movement if collision occurs
			}

			// Spacebar to fire laser
			if (Keyboard::isKeyPressed(Keyboard::Space) && laserFireCounter >= FIRE_RATE)
			{
				// Find an off-screen laser and reposition it for firing
				for (auto& laser : laserBlasts)
				{
					if (laser.getPosition().y < 0)  // Laser is off-screen
					{
						laser.setPosition(spaceship.getPosition().x, spaceship.getPosition().y);  // Set laser position
						laserFireCounter = 0;  // Reset fire counter
						break;  // Only fire one laser at a time
					}
				}
			}
		}

		// Update the game scene if not paused
		if (!paused)
		{
			laserFireCounter++;  // Increment fire counter
			frameCounter++;  // Increment frame counter
			updateGame();  // Update game logic (centipede, spider, mushrooms, etc.)
		}

		// If the game has ended, show the Game Over screen
		if (ended)
		{
			// Display game over text
			gameOverText.setFont(font);
			gameOverText.setCharacterSize(80);
			gameOverText.setStyle(sf::Text::Bold);
			gameOverText.setPosition(960 / 2 - 200, 540 / 2 - 100);  // Center the text

			// Check if the player won or lost
			if (playerWon)
			{
				gameOverText.setString("YOU WIN!");
				gameOverText.setFillColor(sf::Color::White);  // White color for winning
			}
			else
			{
				gameOverText.setString("YOU LOSE!");
				gameOverText.setFillColor(sf::Color::Red);  // Red color for losing
			}

			// Clear the screen to black and display final score
			window.clear(sf::Color::Black);
			finalScoreText.setFont(font);
			finalScoreText.setCharacterSize(30);
			finalScoreText.setString("Final Score: " + std::to_string(score));
			finalScoreText.setFillColor(sf::Color::White);
			finalScoreText.setPosition(960/ 2.0f - finalScoreText.getGlobalBounds().width / 2.0f,
				540 / 2.0f + 100);
			// Clear the screen to black and display the way to restart the game
			gameRestartText.setFont(font);
			gameRestartText.setCharacterSize(30);
			gameRestartText.setString("Press Shift to restart the game.");
			gameRestartText.setFillColor(sf::Color::White);
			gameRestartText.setPosition(960 / 2.0f - gameRestartText.getGlobalBounds().width / 2.0f,
				540 / 2.0f + 150);
			// Draw the game over text and final score
			window.draw(gameOverText);
			window.draw(finalScoreText);
			window.draw(gameRestartText);
			window.display();  // Display the updated window

			// Wait for the player to press Shift to restart the game
			while (window.isOpen())
			{
				sf::Event event;
				while (window.pollEvent(event))
				{
					if (event.type == Event::KeyReleased && !paused)
					{
						acceptInput = true;  // Accept input during the end screen
					}
				}

				if (Keyboard::isKeyPressed(Keyboard::Escape))
				{
					window.close();  // Close the window when the user closes the game
				}

				// Check for Shift key press to restart the game
				if (Keyboard::isKeyPressed(Keyboard::RShift) || Keyboard::isKeyPressed(Keyboard::LShift))
				{
					paused = true;  // Pause the game to reset
					ended = false;  // Reset end state
					initGame(window);  // Reinitialize the game
					break;  // Exit the end screen loop and restart the game
				}
			}
		}
	}

	return 0;  // Exit the game
}

// Function definition
// Initialize game elements
void initGame(RenderWindow& window)
{
	// Load image resources
	if (!textureStarship.loadFromFile("graphic/StarShip.png") ||
		!textureSpider.loadFromFile("graphic/spider.png") ||
		!textureStartupScreen.loadFromFile("graphic/Startup Screen BackGround.png") ||
		!textureMashroom0.loadFromFile("graphic/Mushroom0.png") ||
		!textureMashroom1.loadFromFile("graphic/Mushroom1.png") ||
		!textureCentipedeBody.loadFromFile("graphic/CentipedeBody.png") ||
		!textureCentipedeHead.loadFromFile("graphic/CentipedeHead.png") ||
		!textureBackground.loadFromFile("graphic/background.png") ||
		!textureLaser.loadFromFile("graphic/laser.png"))
	{
		std::cerr << "Failed to load some textures!" << std::endl;
		return;  // Exit if textures fail to load
	}

	// Load font
	if (!font.loadFromFile("fonts/KOMIKAP_.ttf")) {
		std::cerr << "Error loading font!" << std::endl;
	}

	// Initialize background
	background.setTexture(textureStartupScreen);
	background.setPosition(0, 0);
	Vector2u windowSize = background.getTexture()->getSize();
	background.setScale(
		static_cast<float>(window.getSize().x) / windowSize.x,
		static_cast<float>(window.getSize().y) / windowSize.y
	);

	// Initialize spaceship
	spaceship.setTexture(textureStarship);
	spaceship.setPosition(window.getSize().x / 2, 500);
	initialSpaceshipPosition = spaceship.getPosition();  // Store the initial position for respawning

	// Initialize spaceship icons for remaining lives
	spaceshipIcons.clear();  // Clear any previous icons
	for (int i = 0; i < spaceshipLives; ++i) {
		sf::Sprite lifeIcon(textureStarship);
		lifeIcon.setPosition(900 - i * 25, 10);  // Position icons in the top-right corner
		spaceshipIcons.push_back(lifeIcon);  // Add each icon to the vector
	}

	// Initialize spider at the bottom-right corner
	spider.setTexture(textureSpider);
	spider.setScale(0.8f, 0.8f);
	spider.setPosition(960 - spider.getGlobalBounds().width, 540 - spider.getGlobalBounds().height); // Bottom-right position

	// Initialize centipede
	for (int i = 0; i < 12; ++i) {
		ECE_Centipede* centipede = new ECE_Centipede(textureCentipedeHead, textureCentipedeBody, 218.4f - i * 18.4f, 50.0f, 5.0f, i);
		centipedeSegments.push_back(centipede);
	}

	// Initialize mushrooms without overlapping
	const float mushroomScale = 0.8f;
	std::uniform_real_distribution<> xDist(0, 960 - textureMashroom0.getSize().x * mushroomScale);
	std::uniform_real_distribution<> yDist(70, 480 - textureMashroom0.getSize().y * mushroomScale);

	// Generate mushrooms
	for (int i = 0; i < NUM_MUSHROOMS; ++i) {
		sf::Sprite mushroom(textureMashroom0);
		mushroom.setScale(mushroomScale, mushroomScale);

		bool positionFound = false;

		// Repeat until a valid position is found
		while (!positionFound) {
			// Generate random position
			float x = xDist(gen);
			float y = yDist(gen);
			mushroom.setPosition(x, y);
			positionFound = true;  // Assume valid unless a collision is found

			// Check for overlap with existing mushrooms
			for (const auto& existingMushroom : mushrooms) {
				if (mushroom.getGlobalBounds().intersects(existingMushroom.getGlobalBounds())) {
					positionFound = false;  // Overlap detected
					break;  // Exit the checking loop
				}
			}
		}

		// Add mushroom to the list
		mushrooms.push_back(mushroom);
	}

	// Initialize lasers
	for (int i = 0; i < 21; ++i) {
		ECE_LaserBlast laser(textureLaser);
		laser.setScale(0.5f, 0.5f);
		laserBlasts.push_back(laser);
	}

	// Initialize score text
	scoreText.setFont(font);
	scoreText.setString("Score: 0");
	scoreText.setCharacterSize(30);
	scoreText.setFillColor(sf::Color::White);

	// Initialize score and game status
	score = 0;
	playerWon = false;

	// Position score text at the top center of the screen
	scoreText.setPosition(960 / 2.0f - scoreText.getGlobalBounds().width / 2.0f, 10);
}


void updateGame()
{
	// Update centipede segments every 30 frames
	if (frameCounter % 30 == 0) {
		frameCounter = 0;

		// Update each segment of the centipede
		for (auto& body : centipedeSegments) {
			body->checkBounds(960, 540); // Check for boundary collisions
			body->handleMushroomCollision(mushrooms); // Handle collisions with mushrooms
			body->update(); // Update the segment's position
		}
	}

	// Update laser movements
	for (auto& laser : laserBlasts) {
		laser.update(); // Update laser position
		// Check if the laser is off-screen and reset its position
		if (laser.isOffScreen()) {
			laser.setPosition(-100, -100); // Move off-screen
		}
	}

	// Update spider movement
	updateSpiderMovement();

	// Update every 60 frames for collision checks
	if (frameCounter % 60 == 0) {
		// Check laser collisions with mushrooms
		score = ECE_LaserBlast::checkLaserMushroomCollision(laserBlasts, mushrooms, textureMashroom0, textureMashroom1, textureLaser,score);
		// Check laser collisions with spider
		score = ECE_LaserBlast::checkLaserSpiderCollision(laserBlasts, spider, textureSpider, textureLaser, score);
		// Check centipede collisions with lasers
		score = ECE_Centipede::handleLaserCollision(centipedeSegments, mushrooms, laserBlasts, textureMashroom0, textureCentipedeHead, textureCentipedeBody,score);

		// Check collision between the spider and the spaceship
		if (isOverlapping(spider, spaceship)) {
			handleSpaceshipHit(); // Handle spaceship hit
		}

		// Check collision between the centipede and the spaceship
		if (ECE_Centipede::checkSpaceshipCollision(centipedeSegments, spaceship)) {
			handleSpaceshipHit(); // Handle spaceship hit
		}

		// Check if the player has won by clearing all centipedes
		if (centipedeSegments.empty()) {
			playerWon = true;
			std::cout << "Player won! All centipedes cleared." << std::endl;
			ended = true; // End the game
		}
		scoreText.setString("Score: " + std::to_string(score));
		scoreText.setPosition(960 / 2.0f - scoreText.getGlobalBounds().width / 2.0f, 10);
	}
}

// Draw the game scene
void drawGame(sf::RenderWindow& window, bool paused)
{
	// Clear the screen to black
	window.clear(sf::Color::Black);

	if (!paused) {
		// Draw the spaceship
		window.draw(spaceship);

		// Draw the spider
		window.draw(spider);

		// Draw the score at the top center of the screen
		window.draw(scoreText);

		// Draw spaceship life icons
		for (const auto& spaceshipIcon : spaceshipIcons) {
			window.draw(spaceshipIcon);
		}

		// Draw lasers
		for (const auto& laser : laserBlasts) {
			window.draw(laser);
		}

		// Draw mushrooms
		for (const auto& mushroom : mushrooms) {
			window.draw(mushroom);
		}

		// Draw the centipede segments
		for (const auto& body : centipedeSegments) {
			window.draw(*body); // Draw each segment of the centipede
		}
	}
	else {
		// Draw the background if the game is paused
		window.draw(background);
	}

	// Display the updated window
	window.display();
}


// Handle spaceship collision and respawn logic
void handleSpaceshipHit()
{
	spaceshipLives--; // Decrease lives
	std::cout << "Spaceship hit! Lives remaining: " << spaceshipLives << std::endl;

	// Remove the last life icon and respawn the spaceship
	if (!spaceshipIcons.empty()) {
		spaceshipIcons.pop_back();
	}
	spaceship.setPosition(initialSpaceshipPosition); // Respawn the spaceship

	// Check for game over condition
	if (spaceshipIcons.empty()) {
		std::cout << "Game Over! No lives left." << std::endl;
		ended = true; // End the game
		spaceship.setPosition(-100, -100); // Move spaceship off-screen
	}
}

// Update spider movement
void updateSpiderMovement()
{
	static bool movingLeft = true;  // Control horizontal movement direction
	static bool movingUp = false;    // Control vertical movement direction

	const float spiderSpeedX = 0.02f; // Spider's horizontal speed
	const float spiderSpeedY = 0.03f; // Spider's vertical speed

	// Get the current position of the spider
	float spiderX = spider.getPosition().x;
	float spiderY = spider.getPosition().y;

	// Horizontal movement control
	if (movingLeft) {
		spiderX -= spiderSpeedX; // Move left
		if (spiderX <= 0) {      // Reached the left boundary
			movingLeft = false;   // Change to move right
			movingUp = !movingUp; // Change vertical direction
		}
	}
	else {
		spiderX += spiderSpeedX; // Move right
		if (spiderX + spider.getGlobalBounds().width >= 960) { // Reached the right boundary
			movingLeft = true;    // Change to move left
			movingUp = !movingUp; // Change vertical direction
		}
	}

	// Vertical movement control (oscillate between 300 and 540 in y direction)
	if (movingUp) {
		spiderY -= spiderSpeedY; // Move up
		if (spiderY <= 300) {
			movingUp = false; // Change to move down
		}
	}
	else {
		spiderY += spiderSpeedY; // Move down
		if (spiderY + spider.getGlobalBounds().height >= 540) {
			movingUp = true; // Change to move up
		}
	}

	// Check for collision with mushrooms every 60 frames
	if (frameCounter % 60 == 0) {
		// Use a non-const iterator to allow element deletion
		for (auto mushroomIt = mushrooms.begin(); mushroomIt != mushrooms.end(); ) {
			if (isOverlapping(spider, *mushroomIt)) {
				mushroomIt = mushrooms.erase(mushroomIt); // Remove the current mushroom and get the next iterator
				// Reverse horizontal and vertical direction upon collision
				movingLeft = !movingLeft;
				break; // Exit after collision direction change
			}
			else {
				++mushroomIt; // If no collision, check the next mushroom
			}
		}
	}

	// Set the spider's new position
	spider.setPosition(spiderX, spiderY);
}

// Check if two sprites are overlapping
bool isOverlapping(const sf::Sprite& sprite1, const sf::Sprite& sprite2)
{
	// Get the global bounds of both sprites
	sf::FloatRect bounds1 = sprite1.getGlobalBounds();
	sf::FloatRect bounds2 = sprite2.getGlobalBounds();

	// Check if the bounds intersect
	return bounds1.intersects(bounds2);
}

// Check for collision between the spaceship and mushrooms
bool checkSpaceshipMushroomCollision()
{
	// Iterate through all mushrooms to check for collisions with the spaceship
	for (const auto& mushroom : mushrooms) {
		if (isOverlapping(spaceship, mushroom)) {
			return true; // Return true if a collision is detected
		}
	}
	return false; // Return false if no collisions are found
}
