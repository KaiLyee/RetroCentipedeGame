/*
Author: Junkai Li
Class: ECE6122 
Last Date Modified: 9/27/2024
Description:
Create an ECE_Centipede class derived from the SFML::Sprite.
a. The class is responsible for calculating the location of all the segments of the
centipede and which segments have broken apart into new centipedes with its own
head.
b. The class is also responsible for detecting collisions with other objects and taking
the appropriate action.
*/
#include "ECE_Centipede.h"
#include "ECE_LaserBlast.h"

// Constructor to initialize the centipede's texture, position, speed, and determine if it's a head or body
ECE_Centipede::ECE_Centipede(sf::Texture& headTexture, sf::Texture& bodyTexture, float startX, float startY, float speed, int segmentIndex)
    : speed(speed), direction(Direction::RIGHT), preDirection(Direction::RIGHT), x(startX), y(startY)
{
    setScale(0.8f, 0.8f);  // Set the size of the centipede

    // Set the texture and head status based on segment index
    if (segmentIndex == 0) {
        setTexture(headTexture);    // Apply the head texture
        isHeadSegment = true;       // Mark this segment as the head
    }
    else {
        setTexture(bodyTexture);    // Apply the body texture for other segments
        isHeadSegment = false;      // This is not the head
    }

    setPosition(startX, startY);  // Set the initial position
}

// Updates the centipede's position and direction based on movement logic
void ECE_Centipede::update()
{
    // Update position based on the current direction
    switch (direction) {
    case Direction::LEFT:
        x -= speed;  // Move left
        break;
    case Direction::RIGHT:
        x += speed;  // Move right
        break;
    case Direction::DOWN:
        y += getGlobalBounds().height;  // Move down by the height of the sprite
        break;
    default:
        break;
    }

    // Set the sprite's origin to its center
    setOrigin(getGlobalBounds().width / 2, getGlobalBounds().height / 2);
    setPosition(x, y);  // Update the sprite's position

    // Rotate the sprite according to the direction of movement
    switch (direction) {
    case Direction::LEFT:
        setRotation(180);  // Face left
        break;
    case Direction::RIGHT:
        setRotation(0);    // Face right
        break;
    case Direction::DOWN:
        setRotation(90);   // Face downward
        break;
    default:
        break;
    }

    // After moving down, switch to the opposite horizontal direction
    if (direction == Direction::DOWN) {
        direction = (preDirection == Direction::LEFT) ? Direction::RIGHT : Direction::LEFT;
        printf("Switching Horizontal Direction\n");
    }
}

// Check for collision between the centipede and another object
bool ECE_Centipede::checkCollision(const sf::Sprite& otherObject)
{
    return getGlobalBounds().intersects(otherObject.getGlobalBounds());
}

// Ensure the centipede stays within screen bounds
void ECE_Centipede::checkBounds(float screenWidth, float screenHeight)
{
    // Check if the centipede hits the left or right screen boundaries
    if (x - getGlobalBounds().width / 2 <= 0 || x + getGlobalBounds().width / 2 >= screenWidth) {
        if (direction != Direction::DOWN) {
            preDirection = direction;  // Store the current direction before changing

            // Adjust position to stay within screen boundaries
            if (direction == Direction::RIGHT) {
                x = screenWidth - getGlobalBounds().width;  // Stay within the right boundary
            }
            else {
                x = getGlobalBounds().width;  // Stay within the left boundary
            }
            direction = Direction::DOWN;  // Change direction to downwards
            printf("Moving Down\n");
        }
    }

    // Clamp the centipede's position if it goes below the screen height
    if (y + getGlobalBounds().height >= screenHeight) {
        y = screenHeight - getGlobalBounds().height;  // Clamp to screen height
        setPosition(x, y);  // Update position
    }
}
#include "ECE_Centipede.h"
#include "ECE_LaserBlast.h"

// Set a new movement direction for the centipede
void ECE_Centipede::setDirection(Direction newDirection)
{
    direction = newDirection;
}

// Handle collisions between the centipede and mushrooms
void ECE_Centipede::handleMushroomCollision(std::list<sf::Sprite>& mushrooms)
{
    for (auto& mushroom : mushrooms) {
        if (checkCollision(mushroom)) {
            if (direction != Direction::DOWN) {
                preDirection = direction;  // Store the current direction before changing

                // Adjust position to avoid overlap with the mushroom
                if (direction == Direction::RIGHT) {
                    x -= getGlobalBounds().width / 2;  // Move left to avoid collision
                }
                else {
                    x += getGlobalBounds().width / 2;  // Move right to avoid collision
                }

                direction = Direction::DOWN;  // Change direction to downwards
                printf("Moving Down\n");
            }
        }
    }
}

// Handle collisions between centipedes and laser blasts
int ECE_Centipede::handleLaserCollision(
    std::list<ECE_Centipede*>& centipedes,            // List of centipede segments
    std::list<sf::Sprite>& mushrooms,                 // List of mushrooms
    std::list<ECE_LaserBlast>& laserBlasts,           // List of laser blasts
    const sf::Texture& mushroomTexture,               // Texture for mushrooms
    const sf::Texture& centipedeTextureHead,          // Texture for centipede head
    const sf::Texture& centipedeTextureBody,           // Texture for centipede body
    int score                                          // Updata score 
) {
    // Iterate through centipede segments
    for (auto centipedeIt = centipedes.begin(); centipedeIt != centipedes.end(); ) {
        bool centipedeHit = false;  // Flag to check if a centipede was hit

        // Check for collision with laser blasts
        for (auto laserIt = laserBlasts.begin(); laserIt != laserBlasts.end(); ) {
            if ((*centipedeIt)->checkCollision(*laserIt)) {
                // Check if the current segment is the head
                if ((*centipedeIt)->isHeadSegment) {
                    score += 100; // Increase score by 100 for hitting the head
                }
                else {
                    score += 10;  // Increase score by 10 for hitting the body
                }

                // Create a new mushroom at the position of the hit centipede segment
                sf::Sprite mushroom(mushroomTexture);
                mushroom.setScale(0.8f, 0.8f);
                mushroom.setPosition((*centipedeIt)->getPosition());
                mushrooms.push_back(mushroom);  // Add mushroom to the list

                // Save iterator to the next segment
                auto nextSegment = std::next(centipedeIt);
                // Save iterator to the previous segment
                auto prevSegment = (centipedeIt == centipedes.begin()) ? centipedes.end() : std::prev(centipedeIt);

                // Remove the current centipede segment
                centipedeIt = centipedes.erase(centipedeIt);
                centipedeHit = true;

                // Handle the next segment becoming the new head
                if (nextSegment != centipedes.end()) {
                    (*nextSegment)->setAsHead(centipedeTextureHead); // Set the next segment as the new head
                }

                // Check if the segment is isolated (both previous and next segments were erased)
                if (nextSegment == centipedes.end() || (prevSegment != centipedes.end() && (*prevSegment)->checkCollision(*laserIt))) {
                    // Both adjacent segments are gone, remove this segment
                    if (centipedeIt != centipedes.end()) {
                        centipedeIt = centipedes.erase(centipedeIt);
                    }
                }

                // Remove the laser that caused the collision
                laserIt = laserBlasts.erase(laserIt);
                break;  // Exit the inner loop to process the next centipede segment
            }
            else {
                ++laserIt;  // Move to the next laser if no collision
            }
        }

        if (!centipedeHit) {
            ++centipedeIt;  // Move to the next centipede segment if not hit
        }
    }
    return score;
}

// Set this centipede segment as the head
void ECE_Centipede::setAsHead(const sf::Texture& headTexture)
{
    setTexture(headTexture);  // Set the new texture for the head
    isHeadSegment = true;     // Mark this segment as the head
}

// Check for collisions between centipedes and the spaceship
bool ECE_Centipede::checkSpaceshipCollision(std::list<ECE_Centipede*>& centipedes, sf::Sprite& spaceship)
{
    for (auto& centipede : centipedes) {
        if (centipede->getGlobalBounds().intersects(spaceship.getGlobalBounds())) {
            return true;  // Collision detected
        }
    }
    return false;  // No collision
}

