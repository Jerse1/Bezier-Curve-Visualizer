#define SDL_MAIN_HANDLED

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <iomanip>
#include <math.h>
#include <vector>

bool _isRunning;
SDL_Window *window;
SDL_Renderer *renderer;
SDL_KeyCode Key = SDLK_RIGHT;
int Speed = 20;

int xscreen = SDL_WINDOWPOS_CENTERED;
int yscreen = SDL_WINDOWPOS_CENTERED;

int x, y;
short currentHoldObject = -1;

bool mouseDown;

short int windowWidth = 800;
short int windowHeight = 600;

SDL_Rect P0;
SDL_Rect P1;
SDL_Rect P2;
SDL_Rect P3;

void generateApple();

void Initialize(const char *title, bool fullscreen);

void handleEvents();
void Update();
void Render();
void Clean();

bool isRunning();

int getPt(int n1, int n2, float perc)
{
    int diff = n2 - n1;

    return n1 + (diff * perc);
}

int main(int argc, char *argv[])
{

    Initialize("Bezier Curves Visualizer", false);

    clock_t start, end;

    while (isRunning())
    {

        handleEvents();

        double time_since = double(clock() - start);

        int lastTailLength;

        if (time_since / 1000.0 > 0.003)
        {
            Update();
            Render();

            // std::cout << std::setprecision(3) << 1000 / time_since << std::endl;

            start += 3.0;
        }
    }

    Clean();

    return 0;
}

// TODO
void Initialize(const char *title, bool fullscreen)
{
    int flags = 0;

    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "Game has been initialized!" << std::endl;

        window = SDL_CreateWindow(title, xscreen, yscreen, windowWidth, windowHeight, flags);
        if (window)
        {
            std::cout << "Window has been successfully created!" << std::endl;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
            SDL_RenderSetLogicalSize(renderer, windowWidth, windowHeight);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer has been successfully created!" << std::endl;
        }

        _isRunning = true;
    }

    P0.w = 10;
    P0.h = 10;
    P0.x = windowWidth / 2 - 200;
    P0.y = windowHeight / 2 + 100;

    P1.w = 10;
    P1.h = 10;
    P1.x = windowWidth / 2 - 200;
    P1.y = windowHeight / 2 - 100;

    P2.w = 10;
    P2.h = 10;
    P2.x = windowWidth / 2 + 200;
    P2.y = windowHeight / 2 - 100;

    P3.w = 10;
    P3.h = 10;
    P3.x = windowWidth / 2 + 200;
    P3.y = windowHeight / 2 + 100;
};

bool isRunning()
{
    return _isRunning;
}

void handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        _isRunning = false;
        break;
    case SDL_MOUSEBUTTONUP:
    
        if (mouseDown == true)
        {
            mouseDown = false;
            std::cout << "mouse up\n";
            currentHoldObject = -1;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (mouseDown == false)
        {
            mouseDown = true;
            std::cout << "mouse down\n";

            if (((x >= P0.x) && (x <= P0.x + P0.w)) && ((y >= P0.y) && y <= P0.y + P0.h))
            {
                currentHoldObject = 0;
            }
            else if (((x >= P1.x) && (x <= P1.x + P1.w)) && ((y >= P1.y) && y <= P1.y + P1.h))
            {
                currentHoldObject = 1;
            }
            else if (((x >= P2.x) && (x <= P2.x + P2.w)) && ((y >= P2.y) && y <= P2.y + P2.h))
            {
                currentHoldObject = 2;
            }
            else if (((x >= P3.x) && (x <= P3.x + P3.w)) && ((y >= P3.y) && y <= P3.y + P3.h))
            {

                currentHoldObject = 3;
            }
            else
            {
                currentHoldObject = -1;
            }
            //std::cout << currentHoldObject << " HOLD\n";
        }
        break;
    default:
        break;
    }
}

void Update()
{
    SDL_GetMouseState(&x, &y);
    
    if (mouseDown == true)
    {
        if (currentHoldObject == 0)
        {
            P0.x = x - (P0.w / 2);
            P0.y = y - (P0.h / 2);
        }
        else if (currentHoldObject == 1)
        {
            P1.x = x - (P1.w / 2);
            P1.y = y - (P1.h / 2);
        }
        else if (currentHoldObject == 2)
        {
            P2.x = x - (P2.w / 2);
            P2.y = y - (P2.h / 2);
        }
        else if (currentHoldObject == 3)
        {
            P3.x = x - (P3.w / 2);
            P3.y = y - (P3.h / 2);
        }
    }
}

void Render()
{
    SDL_RenderClear(renderer);
    // Rendering n shit;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (float i = 0; i < 1; i += 0.001)
    {
        int xa = getPt(P0.x, P1.x, i);
        int ya = getPt(P0.y, P1.y, i);
        int xb = getPt(P1.x, P2.x, i);
        int yb = getPt(P1.y, P2.y, i);
        int xc = getPt(P2.x, P3.x, i);
        int yc = getPt(P2.y, P3.y, i);

        int xm = getPt(xa, xb, i);
        int ym = getPt(ya, yb, i);
        int xn = getPt(xb, xc, i);
        int yn = getPt(yb, yc, i);

        int finalx = getPt(xm, xn, i) + 5;
        int finaly = getPt(ym, yn, i) + 5;

        SDL_RenderDrawPoint(renderer, finalx, finaly);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);

    SDL_RenderDrawLine(renderer, P0.x + P0.w / 2, P0.y + P0.h / 2, P1.x + P1.w / 2, P1.y + P1.h / 2);
    SDL_RenderDrawLine(renderer, P1.x + P1.w / 2, P1.y + P1.h / 2, P2.x + P2.w / 2, P2.y + P2.h / 2);
    SDL_RenderDrawLine(renderer, P2.x + P2.w / 2, P2.y + P2.h / 2, P3.x + P3.w / 2, P3.y + P3.h / 2);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_RenderFillRect(renderer, &P0);
    SDL_RenderFillRect(renderer, &P1);
    SDL_RenderFillRect(renderer, &P2);
    SDL_RenderFillRect(renderer, &P3);

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);

    SDL_RenderPresent(renderer);
}

void Clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    std::cout << "Game has been successfully cleaned!";
};