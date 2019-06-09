// Simion 
// Cristian
// Grupa 135

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

#include "texture.hpp"

#pragma once

// Forward declaration
class gameWindow;

class Button {
    // Shape for dimensions and position
    SDL_Rect fillRect;

    // Texture of the button, here it is used for displaying text
    Texture bTexture;

    // Transparency value
    Uint8 alpha;

    // Type of button: 0 - default (nothing happens on click), 1 - quit, 2 - restart game, current player loses, 3 - skip turn, 4 - accept swap, 5 - accept swap2
    int type, active = 1;

public:
    Button(int t = 0, int w = 0, int h = 0);
    void setPosition(int x, int y);
    void setDimensions(int w, int h);
    void setType(int t);
    void setText(std::string text, TTF_Font *gFont, SDL_Renderer *rend);
    void setState(int i);
    int getState() { return active; };

    // Handles button events
    void handleEvent(SDL_Event *e, gameWindow &G);
    
    // Renders the button at its x and y coordinates
    void render(gameWindow &G);
};