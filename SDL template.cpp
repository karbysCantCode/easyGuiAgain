    // SDL template.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "SDL.h"
#include "EasyGui.h"
#include "TickRateMadeEasy.h"
#include "physicsOrSomething.h"

void what() {
    std::cout << "clicked!\n";
}



int main(int argc, char* argv[])
{
    std::cout << "Hello World!\n";
    SDL_Window* window;
    SDL_Renderer* renderer;
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return 1; // Exit with error code
        }

        window = SDL_CreateWindow(
            "My SDL Window",               // Window title
            SDL_WINDOWPOS_CENTERED,        // X position, centered
            SDL_WINDOWPOS_CENTERED,        // Y position, centered
            800,                           // Width
            600,                           // Height
            SDL_WINDOW_SHOWN                // Window flags
        );

        if (window == nullptr) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit(); // Clean up SDL
            return 1; // Exit with error code
        }

        renderer = SDL_CreateRenderer(
            window,              // Window to associate with
            -1,                  // Use the first rendering driver
            SDL_RENDERER_ACCELERATED // Renderer flags
        );

        if (renderer == nullptr) {
            std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit(); // Clean up SDL
            return 1; // Exit with error code
        }
    }

    bool running = true;

    SDL_Event event;

    int tick = 0;
    const vector2<int> scrSize = { 800, 600 };
    GME_ObjectManager manager(renderer, 800, 600);
    WorldPhysicsManager phyManager;
    phyManager.ScreenSize = scrSize;


    auto button = manager.Create<GME_Button>();
    auto box = phyManager.Create(button->ID);

    box->Size = vector2<double>(50, 50);
    box->Position = vector2<double>(200, 500);

    button->Position = box->Position;
    button->Size = box->Size;
    button->Color = { 255,255,255,255 };
    button->RefreshTexture(renderer);

    manager.UpdateClickableSquares(button->ID, button->Position, button->Size);
    const int tickRate = 40;

    const auto tickInterval = TRME_getTickInterval(tickRate);

    while (running) {
        auto startTime = TRME_getTimePoint();
        
        tick++;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                manager.ProcessClick(mouseX, mouseY);
            }
            // Add more event handling as needed (e.g., keyboard input, mouse clicks)
        }

        SDL_RenderClear(renderer);

        phyManager.StepPhysics(tickRate);
        vector2<int> size, pos;
        size = button->Size;
        pos = button->Position;

        button->Position = box->GetPixelPosition(scrSize);
        manager.UpdateClickableSquares(button->ID, pos, size);

        if (tick % 60 == 0) {
            
        }

        manager.RenderAll();
        SDL_RenderPresent(renderer);
        TRME_sleepUntilNextTick(startTime, tickInterval);
    }



    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
// 
// Drop SDL2-2.30.3 in the same directory as the SLN
// 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
