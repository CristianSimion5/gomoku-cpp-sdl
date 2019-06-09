// Simion 
// Cristian
// Grupa 135
// Texture definitions

#include "texture.hpp"

#include <iostream>

Texture::Texture() {
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

Texture::~Texture() {
    free();
}

bool Texture::loadFromFile(std::string path, SDL_Renderer *rend) {
    free();
    SDL_Texture *newTexture = NULL;

    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        std::cout << "Unable to load image " << path.c_str() << "! SDL_image Error: " << IMG_GetError() << '\n';
    }
    else {
        newTexture = SDL_CreateTextureFromSurface(rend, loadedSurface);
        if (newTexture == NULL) {
            std::cout << "Unable to create texture from " << path.c_str() << "! SDL Error: " << SDL_GetError() << '\n';
        }
        else {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        SDL_FreeSurface(loadedSurface);
    }

    mTexture = newTexture;
    return (mTexture != NULL);
}

bool Texture::loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font *gFont, SDL_Renderer *rend) {
    free();

    SDL_Surface *textSurface = TTF_RenderText_Blended(gFont, textureText.c_str(), textColor);
    if (textSurface == NULL) {
        std::cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << '\n';
    }
    else {
        mTexture = SDL_CreateTextureFromSurface(rend, textSurface);
        if (mTexture == NULL) {
            std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << '\n';
        }
        else {
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        SDL_FreeSurface(textSurface);
    }

    return (mTexture != NULL);
}

void Texture::free() {
    if (mTexture != NULL) {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void Texture::setBlendMode(SDL_BlendMode blending) {
    // Set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}

void Texture::setAlpha(unsigned char alpha) {
    // Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void Texture::render(int x, int y, SDL_Renderer *rend) {
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    SDL_RenderCopy(rend, mTexture, NULL, &renderQuad);
}