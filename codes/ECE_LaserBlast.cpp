/*
Author: Junkai Li
Class: ECE6122
Last Date Modified: 9/27/2024
Description:
3. Create an ECE_LaserBlast class derived from the SFML::Sprite that calculates the
current location of the laser blast and detecting collisions with objects and taking the
appropriate action. Make sure to allow for the movement of multiple laser blasts by
using a std::list.
*/
#include "ECE_LaserBlast.h"

// Constructor to initialize the laser blast with a texture and set its position off-screen
ECE_LaserBlast::ECE_LaserBlast(const sf::Texture& texture) {
    setTexture(texture);
    setPosition(-100, -100); // Initial position off-screen
}

// Updates the laser's position by moving it upward
void ECE_LaserBlast::update() {
    move(0, -1); // Move upward by 1 pixel
}

// Checks if the laser is off the screen
bool ECE_LaserBlast::isOffScreen() const {
    return getPosition().y < 0; // Off the top of the screen
}

// Resets the laser's position to the specified coordinates
void ECE_LaserBlast::resetPosition(const sf::Vector2f& position) {
    setPosition(position);
}

// Handles collisions between lasers and mushrooms
int ECE_LaserBlast::checkLaserMushroomCollision(
    std::list<ECE_LaserBlast>& laserBlasts,
    std::list<sf::Sprite>& mushrooms,
    sf::Texture& textureMushroom0,
    sf::Texture& textureMushroom1,
    const sf::Texture& textureLaser,
    int score
) {
    for (auto laserIt = laserBlasts.begin(); laserIt != laserBlasts.end(); ) {
        bool laserHit = false;

        for (auto mushroomIt = mushrooms.begin(); mushroomIt != mushrooms.end(); ) {
            if (laserIt->getGlobalBounds().intersects(mushroomIt->getGlobalBounds())) {
                if (mushroomIt->getTexture() == &textureMushroom0) {
                    mushroomIt->setTexture(textureMushroom1);
                }
                else {
                    mushroomIt = mushrooms.erase(mushroomIt); // Remove mushroom
                    score += 4;
                    continue; // Next mushroom
                }
                laserHit = true; // Laser hit a mushroom
                break; // Exit inner loop
            }
            ++mushroomIt; // Next mushroom
        }

        if (laserHit) {
            laserIt = laserBlasts.erase(laserIt); // Remove laser

            // Replenish lasers if below limit
            if (laserBlasts.size() < 21) {
                ECE_LaserBlast laser(textureLaser);
                laser.setScale(0.5f, 0.5f);
                laserBlasts.push_back(laser);
            }
        }
        else {
            ++laserIt; // Next laser
        }
    }
    return score;
}

// Check for collisions between lasers and a spider
int ECE_LaserBlast::checkLaserSpiderCollision(
    std::list<ECE_LaserBlast>& laserBlasts,
    sf::Sprite& spider,
    const sf::Texture& textureSpider,
    const sf::Texture& textureLaser,
    int score
) {
    for (auto laserIt = laserBlasts.begin(); laserIt != laserBlasts.end(); ) {
        if (laserIt->getGlobalBounds().intersects(spider.getGlobalBounds())) {
            laserIt = laserBlasts.erase(laserIt); // Remove laser
            spider.setPosition(-300, -300); // Move spider off-screen
            score += 300; // Update score
            break; // Exit loop
        }
        else {
            ++laserIt; // Next laser
        }
    }

    return score; // Return updated score
}
