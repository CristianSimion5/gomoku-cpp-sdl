// Simion 
// Cristian
// Grupa 135

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

#include "texture.hpp"
#include "button.hpp"

#pragma once

class gameWindow {
public:
    // Screen dimensions
    const int SCREEN_WIDTH = 1000;
    const int SCREEN_HEIGHT = 600;

    // Number of buttons
    const int TOTAL_BUTTONS = 7;

    // SDL Specific variables
    SDL_Window *gWindow;
    SDL_Renderer *gRenderer;
    
    // Textures and fonts
    Texture gBackgroundTexture, gSidebarTexture, gBlack, gWhite, gTextTexture;
    Texture p1Score, p2Score;
    TTF_Font *gFont;
    
    Button gButton[7];
    
    // Data associated with game flow
    bool quit = false, won = false, inDraw = false, inSwap = true, inSwap2 = false, swapBlock = true;
    bool playerTurn;
    int dontPress = 0;
    int whoBlack = 0;
    int swapType = 0;
    int player1Score = 0, player2Score = 0;
    int swapCount = 0;

    // Data associated with the game table
    int m[20][23];
    int offsetW = 11;
    int offsetH = 11;
    int distW = 34;
    int distH = 29;
    int mLasti, mLastj;

    gameWindow(): gWindow(NULL), gRenderer(NULL), gFont(NULL), playerTurn(false) {};

    // Initiates window
    bool init();

    // Loads textures
    bool loadMedia();

    // Loads individual image
    SDL_Surface *loadSurface(std::string path);

    // Loads individual image as texture
    SDL_Texture* loadTexture(std::string path);
    
    // Correlates the matrix coordinates with the mouse coordinates
    int posW(int i) { return offsetW + i * distW; };
    int posH(int i) { return offsetH + i * distH; };

    // Renders black and white stones on the table
    void renderTable();

    // Renders sidebar buttons
    void renderButtons();

    // Renders stats
    void renderStats();

    // Renders sidebar elements, such as buttons and stats
    void renderUI();

    // Clears the table
    void clearTable() {
        for (int i = 0; i < 20; i++)
            for (int j = 0; j < 23; j++)
                m[i][j] = 0;
    }
    
    // Handles mouse events such as clicking and hovering
    void handleEvent(SDL_Event *e);
    void handleButtonsEvent(SDL_Event *e);

    // Checks if the game ended, or if it is still running, returns:
    // 0: if the game didn't end
    // 1: if current player won
    // 2: if there is a draw 
    int checkGameState(int I, int J);

    // Modifies UI for preparing a new round
    void setWin();

    // Modifies UI after starting a new round
    void setGame();

    // Exits the special rules of the Swap/Swap2 openings
    void exitSwap();

    // Closes window
    void close();
};

