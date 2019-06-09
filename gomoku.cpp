// Simion 
// Cristian
// Grupa 135

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "gameWindow.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    // Main game variable
    gameWindow G;
    
    if (!G.init()) {
        cout << "Failed to initialize!\n";
    }
    else {
        if (!G.loadMedia()) {
            cout << "Failed to load media!\n";
        }
        else {
            SDL_Event e;
            SDL_SetRenderDrawBlendMode(G.gRenderer, SDL_BLENDMODE_BLEND);

            // Main game loop
            while (!G.quit) {

                // Checks user input
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        G.quit = true;
                    }
                    G.handleEvent(&e);
                    G.handleButtonsEvent(&e);
                }

                // Clear screen
                SDL_RenderClear(G.gRenderer);

                // Game table viewport
                SDL_Rect gTableViewport = {0, 0, (8 * G.SCREEN_WIDTH) / 10, G.SCREEN_HEIGHT};
                SDL_RenderSetViewport(G.gRenderer, &gTableViewport);

                // Renders the background and the table
                G.gBackgroundTexture.render(0, 0, G.gRenderer);
                G.renderTable();

                // Sidebar viewport
                SDL_Rect gSidebarViewport = {(8 * G.SCREEN_WIDTH) / 10, 0, (2 * G.SCREEN_WIDTH) / 10, G.SCREEN_HEIGHT};
                SDL_RenderSetViewport(G.gRenderer, &gSidebarViewport);
                G.gSidebarTexture.render(0, 0, G.gRenderer);

                // Renders the interface
                G.renderUI();

                // Update the surface
                SDL_RenderPresent(G.gRenderer);

                // Limits the game to ~60 fps
                SDL_Delay(16);
            }
        }
    }

    // Close SDL
    G.close();

    return 0;
}