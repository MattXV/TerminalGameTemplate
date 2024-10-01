#include <curses.h>
#include <string>
#include <cstdio>
#include <functional>
#include "Utility.h"
#include <thread>


int main() {
    // 
    // Setup Phase
    // 


    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    // Rendering options
    int frameRate = 60;
    int delay = 1000 / frameRate;


    int screenHeight, screenWidth;
        getmaxyx(stdscr, screenHeight, screenWidth);
    auto lastFrameTime = high_resolution_clock::now();

    // Add colours here :)
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_RED);
    init_pair(3, COLOR_BLACK, COLOR_GREEN);


    // Game 'Assets' and game data lol
    int ballX = 0;
    int ballY = 20;

    int ballDirection = 1;


    double eventTimer = 0;
    double jumpTimer = 0;
    bool jumping = false;

    // Main game loop
    Time::Init();
    while (true) {
        // 
        // Update Phase
        // 


        // Calculate deltaTime
        Time::Update();
       


        // Handle user input and Update game state
        int userInput = getch();
        if (userInput == 'q') {
            break; // Exit the loop and terminate the game if 'q' is pressed
        }
        else if (userInput == KEY_RIGHT) {
            ballDirection = 1; // Move right if the right arrow key is pressed
        }
        else if (userInput == KEY_LEFT) {
            ballDirection = -1; // Move left if the left arrow key is pressed
        }
        else if (userInput == ' ') {
            if (!jumping && jumpTimer < 0.01) {
                jumping = true;
            }
        }

        if (ballX > screenWidth || ballX < 0) {
            ballDirection *= -1; // Reverse direction on collision with edges
        }

        eventTimer += Time::getDeltaTime();
        if (eventTimer > .02) {
            ballX += ballDirection;
            eventTimer = 0;

            if (jumping) {
                ballY -= 1;
            }
            else {
                ballY = 20;
            }
        }

        //ballX += ballDirection;
        if (jumping) {
            jumpTimer += Time::getDeltaTime();
            if (jumpTimer > 0.4) {
                jumpTimer = 0.;
                jumping = false;
            }
        }


        // 
        // Rendering Phase
        // 
        
        // Clear the screen first
        erase();
        // Render game objects using mvprintw
        //mvprintw(y, x, "Character or object symbol");


        // Standard colour
        attron(COLOR_PAIR(1));
        // Draw the ball on the screen
        mvprintw(ballY, ballX, "O");

        // UI? Can you call it that?
        mvprintw(0, 0, formatString("%s | DeltaTime: %f", "Henlo frens", Time::getDeltaTime()).c_str());
        // Change colour - see top of the function for colour setup
        attron(COLOR_PAIR(2));
        mvprintw(5, 5, "This should be printed in black with a red background!\n");

        attron(COLOR_PAIR(3));
        mvprintw(7, 5, "And this in a green background!\n");
        

        // Update the terminal
        refresh();


        Time::SetLastFrameTime();
        std::this_thread::sleep_for(milliseconds(delay));

    }

    // Cleanup ncurses
    endwin();
    return 0;
}
