/* Librairies */
    #include "bn_core.h"
    #include "bn_log.h"
    #include "bn_math.h"
    #include <bn_sprite_ptr.h>
    #include <bn_sprite_item.h>
    #include <bn_keypad.h>
    #include <bn_regular_bg_ptr.h>
    #include <bn_random.h>
    #include <bn_string.h>
    #include <bn_vector.h>
    #include <bn_sprite_text_generator.h>
    #include "common_info.h"
    #include "common_variable_8x8_sprite_font.h"
    #include <bn_vector.h>

/* Graphics */
    #include "bn_sprite_items_snakehead.h"
    #include "bn_sprite_items_snakebody.h"
    #include "bn_sprite_items_apple.h"
    #include "bn_regular_bg_items_bggame.h"

/* Variables */
    // Player
        const int playerMovement = 15; //The amount of pixel they move
        int playerDirection = 1; //0 = Up, 1 = Right, 2 = Down, 3 = Left

    // Apple
        int appleX = 0; // Apple X position
        int appleY = -8; // Apple Y position

    // Waiting
        const int movementTime = 15; // The amount of frames before moving again
        int wait = 0;

    bn::random random; // Initalise the randomness

    int score = 0; // Score

void randomApple() { // Randomize the apple placement
    appleX = ((random.get_int() % 15) * 15) - 120 + playerMovement;
    appleY = ((random.get_int() % 10) * 15) - 83 + playerMovement;
}

void setApple(bn::sprite_ptr apple) { // Set the apple position
    apple.set_x(appleX); //Set the apple's x value
    apple.set_y(appleY); //Set the apple's y value
}

int main() { /* Main Program */

    bn::core::init(); // Initalise the game

    // Create the graphics
        //Create the background
        bn::regular_bg_ptr bggame = bn::regular_bg_items::bggame.create_bg(10, 47);

        //Create the snake
        bn::sprite_ptr snakehead = bn::sprite_items::snakehead.create_sprite(0, -8);
        bn::vector<bn::sprite_ptr, 149> snakebody;

        //Create the apple
        bn::sprite_ptr apple = bn::sprite_items::apple.create_sprite(0, -8);
        randomApple();
        setApple(apple);

    // Create the text generator and font
        bn::sprite_text_generator text_generator(common::variable_8x8_sprite_font);
        bn::vector<bn::sprite_ptr, 16> text_sprites;

    while(true) { // Gameplay

        bn::core::update();

        text_sprites.clear(); // Clear all text
        bn::string<32> test_txt = "Score: " + bn::to_string<32>(score); // Choose the text
        text_generator.generate(-6 * 16, -68, test_txt, text_sprites); // Draw the text

        wait = wait + 1; // Add waiting period

        if (wait >= movementTime) { // Move the snake after the end of waiting period
            wait = 0;
                
            switch (playerDirection) { // What player direction?
                default:
                    break;
                case 0: // Up
                    snakehead.set_rotation_angle(90); // Rotate the image
                    if (snakehead.y() - playerMovement <= -70) { // If it will leave the screen...
                        break;
                    }
                    else { // If it's ok to move
                        snakehead.set_y(snakehead.y() - playerMovement); // Move up
                        break;
                    }
                case 1: // Right
                    snakehead.set_rotation_angle(0); // Rotate the image
                    if (snakehead.x() + playerMovement >= 110) { // If it will leave the screen...
                        break;
                    }
                    else { // If it's ok to move
                        snakehead.set_x(snakehead.x() + playerMovement); // Move right
                        break;
                    }
                case 2: // Down
                    snakehead.set_rotation_angle(270); // Rotate the image
                    if (snakehead.y() + playerMovement >= 70) { // If it will leave the screen...
                        break;
                    }
                    else { // If it's ok to move
                        snakehead.set_y(snakehead.y() + playerMovement); // Move down
                        break;
                    }
                case 3: // Left
                    snakehead.set_rotation_angle(180); // Rotate the image
                    if (snakehead.x() - playerMovement <= -110) { // If it will leave the screen...
                        break;
                    }
                    else { // If it's ok to move
                        snakehead.set_x(snakehead.x() - playerMovement); // Move left
                        break;
                    }
            }
            /* This doesn't work because i'm stupid
            for (int i = snakebody.size() - 1; i > 0; i--) { // For all the tail parts
                snakebody[i].x() = snakehead.x(); //Move the x position
                snakebody[i].y() = snakehead.y(); //Move the y position
            }
            */
        }

        if (apple.x() == snakehead.x() and apple.y() == snakehead.y()) { // Detect if the apple is touched
            score = score + 1; // Add a point to the score

            // Add a tail
            snakebody.push_back(bn::sprite_items::snakebody.create_sprite(snakehead.x(), snakehead.y()));

            randomApple(); // Generate a random placement for the apple
            while (appleX == snakehead.x() && appleY == snakehead.y()) { // If the apple has been placed exactly where the snake is...
                randomApple(); // Generate another random placement for the apple
            }
            setApple(apple); // Set the apple's new position
        }

        // Controls
            if (bn::keypad::left_pressed()) { // If left is pressed
                if (playerDirection != 1) { // If the player isn't looking right
                    playerDirection = 3; // Look Left
                }

            }

            if (bn::keypad::right_pressed()) { // If right is pressed
                if (playerDirection != 3) { // If the player isn't looking left
                    playerDirection = 1; // Look Right
                }
            }

            if (bn::keypad::up_pressed()) { // If up is pressed
                if (playerDirection != 2) { // If the player isn't looking down
                    playerDirection = 0; // Look Up
                }
            }

            if (bn::keypad::down_pressed()) { // If down is pressed
                if (playerDirection != 0) { // If the player isn't looking up
                    playerDirection = 2; // Look Down
                }
            }
    }
}
