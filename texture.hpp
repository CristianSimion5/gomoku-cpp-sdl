// Simion 
// Cristian
// Grupa 135

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

#pragma once

class Texture {
    // The plain texture
    SDL_Texture *mTexture;

    // Texture dimension
    int mWidth;
    int mHeight;

public:
    Texture();
    ~Texture();

    // Loads texture from file
    bool loadFromFile(std::string path, SDL_Renderer *rend);
    
    // Loads texture from a string and a TrueType font
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font *gFont, SDL_Renderer *rend);
    
    // Frees the texture from the memory
    void free();

    // Sets the blend mode of the texture
    void setBlendMode(SDL_BlendMode blending);
    
    // Sets the transparency of the texture
    void setAlpha(unsigned char alpha);

    // Renders the texture at given coordinates
    void render(int x, int y, SDL_Renderer *rend);

    // Returns the texture dimensions
    int getWidth() { return mWidth; };
    int getHeight() { return mHeight; };
};