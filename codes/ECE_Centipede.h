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

#ifndef ECE_CENTIPEDE_H
#define ECE_CENTIPEDE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include "ECE_LaserBlast.h"

// ECE_Centipede class representing a segment of the centipede in the game
class ECE_Centipede : public sf::Sprite
{
public:
    // Enum to define movement directions for the centipede
    enum class Direction {
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    // Constructor to initialize the centipede with textures, starting position, and speed
    ECE_Centipede(sf::Texture& headTexture, sf::Texture& bodyTexture, float startX, float startY, float speed, int segmentIndex);

    // Updates the position of the centipede based on its speed and direction
    void update();

    // Checks for collision with another object
    bool checkCollision(const sf::Sprite& otherObject);

    // Checks for screen boundaries and adjusts direction if necessary
    void checkBounds(float screenWidth, float screenHeight);

    // Getter for the centipede's speed
    float getSpeed() const { return speed; }

    // Setter to change the direction of movement
    void setDirection(Direction newDirection);

    // Handles collision with mushrooms
    void handleMushroomCollision(std::list<sf::Sprite>& mushrooms);

    // Static method to check collision with the spaceship
    static bool checkSpaceshipCollision(std::list<ECE_Centipede*>& centipedes, sf::Sprite& spaceship);

    // Handles laser collisions, splitting the centipede if hit
    static int handleLaserCollision(
        std::list<ECE_Centipede*>& centipedes,
        std::list<sf::Sprite>& mushrooms,
        std::list<ECE_LaserBlast>& laserBlasts,
        const sf::Texture& mushroomTexture,
        const sf::Texture& centipedeTextureHead,
        const sf::Texture& centipedeTextureBody,
        int score
    );

    // Checks if the current segment is the head of the centipede
    bool isHead() const { return isHeadSegment; }

    // Sets the current segment as the head of the centipede
    void setAsHead(const sf::Texture& headTexture);

private:
    // Speed of the centipede's movement
    float speed;

    // Current and previous movement directions
    Direction direction;
    Direction preDirection;

    // Position of the centipede
    float x;
    float y;

    // Flag indicating if this segment is the head of the centipede
    bool isHeadSegment;
};

#endif // ECE_CENTIPEDE_H
