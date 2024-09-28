/*
Author: Junkai Li
Class: ECE6122 
Last Date Modified: 9/27/2024
Description:
3. Create an ECE_LaserBlast class derived from the SFML::Sprite that calculates the
current location of the laser blast and detecting collisions with objects and taking
the appropriate action. Make sure to allow for the movement of multiple laser blasts by
using a std::list.
*/
#ifndef ECE_LASERBLAST_H
#define ECE_LASERBLAST_H

#include <SFML/Graphics.hpp>
#include <list>

class ECE_LaserBlast : public sf::Sprite {
public:
    // Constructor to initialize the laser blast with a specified texture
    ECE_LaserBlast(const sf::Texture& texture);

    // Updates the position of the laser blast based on its velocity
    void update();

    // Checks if the laser blast has moved off the screen
    bool isOffScreen() const;

    // Resets the position of the laser blast to the specified location
    void resetPosition(const sf::Vector2f& position);

    // Checks for collisions between laser blasts and mushrooms, handling the outcome
    static int checkLaserMushroomCollision(
        std::list<ECE_LaserBlast>& laserBlasts,            // List of active laser blasts
        std::list<sf::Sprite>& mushrooms,                  // List of mushrooms in the game
        sf::Texture& textureMushroom0,                    // Texture for mushroom when damaged
        sf::Texture& textureMushroom1,                    // Texture for mushroom when further damaged
        const sf::Texture& textureLaser,                   // Original texture of the laser
        int score                                           // Current score to update
    );

    // Checks for collisions between laser blasts and a spider, updating the score if needed
    static int checkLaserSpiderCollision(
        std::list<ECE_LaserBlast>& laserBlasts,            // List of active laser blasts
        sf::Sprite& spider,                                 // Spider sprite to check collision with
        const sf::Texture& textureSpider,                  // Texture for the spider
        const sf::Texture& textureLaser,                   // Original texture of the laser
        int score                                           // Current score to update
    );
};

#endif // ECE_LASERBLAST_H
