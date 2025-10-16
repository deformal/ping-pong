#include <iostream>
#include <SDL.h>
using namespace std;

struct Paddle
{
    int x, y;
    int w = 100, h = 100;
    int speed = 10;
};

struct Ball
{
    float x, y;
    int size = 10;
    float velocityX = 3, velocityY = 3;
};

int main(int argc, char **args)
{
    SDL_Window *window = NULL;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        SDL_Delay(2000);
        return 1;
    }

    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);

    if (window == NULL || !window)
    {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        SDL_Delay(2000);
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL)
    {
        cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        SDL_Delay(2000);
        return 1;
    }

    // windowSurface = SDL_GetWindowSurface(window);

    // if (windowSurface == NULL || !windowSurface)
    // {
    //     cout << "Window surface could not be created! SDL_Error: " << SDL_GetError() << endl;
    //     SDL_Delay(2000);
    //     return 1;
    // }

    // SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 123, 255, 255));

    // SDL_UpdateWindowSurface(window);

    bool running = true;

    SDL_Event event;

    Paddle leftPaddlePosition = {0, 310, 10, 100};
    Paddle rightPaddlePosition = {1270, 310, 10, 100};
    Ball ball = {640, 360};

    leftPaddlePosition.speed += 10;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);

        if (keystate[SDL_SCANCODE_Q] || keystate[SDL_SCANCODE_ESCAPE])
        {
            running = false;
        }

        if (keystate[SDL_SCANCODE_W] && leftPaddlePosition.y > 0)
        {
            leftPaddlePosition.y -= leftPaddlePosition.speed;
            // cout << "[DEBUG]:  " << "left pad speed: " << leftPaddlePosition.speed << endl;
            // cout << "[DEBUG]:  " << "left pad y position: " << leftPaddlePosition.y << endl;
        }

        if (keystate[SDL_SCANCODE_S] && leftPaddlePosition.y + leftPaddlePosition.h < 720)
        {
            leftPaddlePosition.y += leftPaddlePosition.speed;
            // cout << "[DEBUG]:  " << "left pad speed: " << leftPaddlePosition.speed << endl;
            // cout << "[DEBUG]:  " << "left pad y position: " << leftPaddlePosition.y << endl;
        }

        if (keystate[SDL_SCANCODE_UP] && rightPaddlePosition.y > 0)
        {
            rightPaddlePosition.y -= rightPaddlePosition.speed;
        }

        if (keystate[SDL_SCANCODE_DOWN] && rightPaddlePosition.y + rightPaddlePosition.h < 720)
        {
            rightPaddlePosition.y += rightPaddlePosition.speed;
        }

        if (ball.velocityX >= 100 || ball.velocityX <= -100)
        {
            ball.velocityX = 3;
        }

        cout << "[DEBUG]:  " << "ball x velociry " << ball.velocityX << endl;

        ball.x += ball.velocityX;
        ball.y += ball.velocityY;

        if (ball.y <= 0 || ball.y + ball.size >= 720)
        {
            ball.velocityY *= -1;
        }

        if (ball.x <= leftPaddlePosition.x + leftPaddlePosition.w &&
            ball.y + ball.size >= leftPaddlePosition.y &&
            ball.y <= leftPaddlePosition.y + leftPaddlePosition.h)
        {
            ball.velocityX -= 1;
            ball.velocityX *= -1;
            ball.x = leftPaddlePosition.x + leftPaddlePosition.w;
        }

        if (ball.x + ball.size >= rightPaddlePosition.x &&
            ball.y + ball.size >= rightPaddlePosition.y &&
            ball.y <= rightPaddlePosition.y + rightPaddlePosition.h)
        {
            ball.velocityX += 1;
            ball.velocityX *= -1;
            ball.x = rightPaddlePosition.x - ball.size;
        }

        if (ball.x < 0 || ball.x > 1280)
        {
            ball.x = 640;
            ball.y = 360;
            ball.velocityX = -ball.velocityX;
        }

        SDL_SetRenderDrawColor(ren, 30, 30, 30, 255);
        SDL_RenderClear(ren);

        SDL_Rect leftRect = {leftPaddlePosition.x, leftPaddlePosition.y, leftPaddlePosition.w, leftPaddlePosition.h};
        SDL_Rect rightRect = {rightPaddlePosition.x, rightPaddlePosition.y, rightPaddlePosition.w, rightPaddlePosition.h};
        SDL_Rect ballRect = {(int)ball.x, (int)ball.y, ball.size, ball.size};

        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderFillRect(ren, &leftRect);
        SDL_RenderFillRect(ren, &rightRect);
        SDL_RenderFillRect(ren, &ballRect);

        SDL_SetRenderDrawColor(ren, 100, 100, 100, 255);
        for (int i = 0; i < 720; i += 20)
            SDL_RenderDrawLine(ren, 640, i, 640, i + 10);

        // SDL_SetRenderDrawColor(ren, 80, 200, 220, 255);
        // SDL_RenderDrawLine(ren, 10, 200, 10, 400);

        // int farRightMarging, farRightMarginTop;
        // SDL_GetWindowSize(window, &farRightMarging, &farRightMarginTop);
        // farRightMarging = farRightMarging - 10;

        // SDL_SetRenderDrawColor(ren, 100, 200, 100, 255);
        // SDL_RenderDrawLine(ren, farRightMarging, 200, farRightMarging, 400);

        // if (event.type == SDL_KEYDOWN)
        // {
        //     switch (event.key.keysym.scancode)
        //     {
        //     case SDL_SCANCODE_UP:
        //         cout << "UP arrow key pressed" << endl;
        //         break;
        //     case SDL_SCANCODE_DOWN:
        //         cout << "DOWN arrow key pressed" << endl;
        //         break;
        //     case SDL_SCANCODE_LEFT:
        //         cout << "LEFT arrow key pressed" << endl;
        //         break;
        //     case SDL_SCANCODE_RIGHT:
        //         cout << "RIGHT arrow key pressed" << endl;
        //         break;
        //     case SDL_SCANCODE_W:
        //         cout << "W key pressed" << endl;
        //         break;
        //     case SDL_SCANCODE_S:
        //         cout << "S key pressed" << endl;
        //         break;
        //     case SDL_SCANCODE_A:
        //         cout << "A key pressed" << endl;
        //         break;
        //     case SDL_SCANCODE_D:
        //         cout << "D key pressed" << endl;
        //         break;
        //     default:
        //         break;
        //     }
        // }

        SDL_RenderPresent(ren);

        // SDL_Delay(16);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
