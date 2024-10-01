#include <curses.h>
#include <windows.h>
#include <chrono>
#include <string>
#include <cstdio>
#include <functional>
#include <thread>


using namespace std::chrono;



class Time {
public:
    static void Init() { lastFrameTime = steady_clock::now(); }
    static void Update() {
        const steady_clock::time_point now = steady_clock::now();
        const duration<double> dt_ms = now - lastFrameTime;
        deltaTime = dt_ms.count();
        
       
    }
    static void SetLastFrameTime() { lastFrameTime = steady_clock::now(); }

    static double getDeltaTime() { return deltaTime; }
private:
    static steady_clock::time_point lastFrameTime;
    static double deltaTime;
};
double Time::deltaTime = 0;
steady_clock::time_point Time::lastFrameTime = steady_clock::now();


template<typename... FormatParameters>
std::string formatString(const char* format, const FormatParameters&... pars) {
    int stringSize = snprintf(nullptr, 0, format, pars...) + 1;
    if (stringSize < 0) return std::string();
    
    const int stackThresh = 64;
    char stackAll[stackThresh];
    char* str = nullptr;
    if (stringSize < stackThresh) {
        str = &stackAll[0];
    }
    else {
        str = new char[stringSize];

    }

    snprintf(str, stringSize, format, pars...);
    std::string outString((const char*)str);
    if (!(stringSize < stackThresh))
        delete[] str;
    
    
    return outString;
}



int main() {


    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    // Set the refresh rate
    int frameRate = 60;
    int delay = 1000 / frameRate;

    int x = 10;
    int y = 10;

    int screenHeight, screenWidth;
        getmaxyx(stdscr, screenHeight, screenWidth);
    int ballX = 0;
    int ballY = 20;

    int ballDirection = 1;



    auto lastFrameTime = high_resolution_clock::now();


    double eventTimer = 0;
    double jumpTimer = 0;
    bool jumping = false;
    double timer = 0;

    // Main game loop
    Time::Init();
    while (true) {
        // start with a blank canvas

        // Calculate deltaTime
        Time::Update();
        double deltaTime = Time::getDeltaTime();
       

        // Handle user input and Update game state
        int userInput = getch();
        if (userInput == 'q') {
            break; // Exit the game if 'q' is pressed
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

        // More rendering...
        eventTimer += deltaTime;
        if (eventTimer > .02) {
            //ballDirection *= -1;
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


        if (ballX > screenWidth || ballX < 0) {
            ballDirection *= -1; // Reverse direction on collision with edges
        }

        erase();
        // Render game objects using mvprintw
        //mvprintw(y, x, "Character or object symbol");
        timer += deltaTime;
        //std::string timeStr = std::string("DeltaTime: ") + std::to_string(deltaTime) + std::string("Timer: ") + std::to_string(timer);
        
        mvprintw(10, 10, formatString("%s DeltaTime: %f", "Loooooooooooooooooooooooooooong", Time::getDeltaTime()).c_str());

        

        // Draw the ball on the screen
        mvprintw(ballY, ballX, "O");

        // Refresh the screen
        refresh();
        //Sleep(delay);
        //usleep(delay);

        Time::SetLastFrameTime();
        std::this_thread::sleep_for(milliseconds(12));
    }

    // Cleanup ncurses
    endwin();
    return 0;
}
