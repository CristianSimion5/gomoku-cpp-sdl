// Simion 
// Cristian
// Grupa 135

#include "gameWindow.hpp"
#include "texture.hpp"

#include <iostream>

bool gameWindow::init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << '\n';
        success = false;
    }
    else {
        gWindow = SDL_CreateWindow("Gomoku", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << '\n';
            success = false;
        }
        else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL) {
                std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
                success = false;
            }
            else {
                // Initiate SDL_image for png loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << '\n';
                    success = false;
                }

                // Initiate SDL_ttf for true type fonts
                if (TTF_Init() == -1) {
                    std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << '\n';
                    success = false;
                }

                // Initiate the buttons

                gButton[0].setType(1);
                gButton[0].setDimensions(140, 40);
                gButton[0].setPosition(30, 500);

                gButton[1].setState(0);
                gButton[1].setType(2);
                gButton[1].setDimensions(140, 40);
                gButton[1].setPosition(30, 450);

                gButton[2].setState(0);
                gButton[2].setType(3);
                gButton[2].setDimensions(140, 40);
                gButton[2].setPosition(30, 400);

                gButton[3].setState(0);
                gButton[3].setType(0);
                gButton[3].setDimensions(140, 40);
                gButton[3].setPosition(30, 350);

                gButton[4].setState(0);
                gButton[4].setType(-1);
                gButton[4].setDimensions(65, 40);
                gButton[4].setPosition(105, 350);

                gButton[5].setState(1);
                gButton[5].setType(4);
                gButton[5].setDimensions(140, 40);
                gButton[5].setPosition(30, 240);

                gButton[6].setState(1);
                gButton[6].setType(5);
                gButton[6].setDimensions(140, 40);
                gButton[6].setPosition(30, 290);

                clearTable();
            }
        }
    }

    return success;
}

bool gameWindow::loadMedia() {
    bool success = true;

    // Load game table image
    if (!gBackgroundTexture.loadFromFile("textures/game_table.png", gRenderer)) {
        std::cout << "Unable to load texture image!\n";
        success = false; 
    }

    // Load sidebar image
    if (!gSidebarTexture.loadFromFile("textures/sidebar.png", gRenderer)) {
        std::cout << "Unable to load texture image!\n";
        success = false; 
    }

    // Load black stone image
    if (!gBlack.loadFromFile("textures/black.png", gRenderer)) {
        std::cout << "Failed to load black stone image!\n";
        success = false;
    }
    else {
        gBlack.setBlendMode(SDL_BLENDMODE_BLEND);
    }

    // Load white stone image
    if (!gWhite.loadFromFile("textures/white.png", gRenderer)) {
        std::cout << "Failed to load white stone image!\n";
        success = false;
    }
    else {
        gWhite.setBlendMode(SDL_BLENDMODE_BLEND);
    }

    // Load truetype font
    gFont = TTF_OpenFont("ttf/Roboto-Medium.ttf", 28);
    if (gFont == NULL) {
        std::cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << '\n';
        success = false;
    }
    else {
        SDL_Color textColor = {0, 0, 0};
        if (!gTextTexture.loadFromRenderedText("Choose swap", textColor, gFont, gRenderer)) {
            std::cout << "Failed to render text texture!\n";
            success = false;
        }
    }

    // Sets the text for buttons
    gButton[0].setText("Quit", gFont, gRenderer);
    gButton[1].setText("Forfeit", gFont, gRenderer);
    gButton[2].setText("Skip turn", gFont, gRenderer);
    gButton[3].setText("Draw", gFont, gRenderer);
    gButton[4].setText("No", gFont, gRenderer);
    gButton[5].setText("Swap", gFont, gRenderer);                        
    gButton[6].setText("Swap2", gFont, gRenderer);                        

    // Sets the text for the score textures
    p1Score.loadFromRenderedText("Player 1: 0", {0, 0, 0}, gFont, gRenderer);
    p2Score.loadFromRenderedText("Player 2: 0", {0, 0, 0}, gFont, gRenderer);

    return success;
}

SDL_Texture* gameWindow::loadTexture(std::string path) {
    SDL_Texture *newTexture = NULL;

    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        std::cout << "Unable to load image " << path.c_str() << "! SDL_image Error: " << IMG_GetError() << '\n';
    }
    else {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            std::cout << "Unable to create texture from " << path.c_str() << "! SDL Error: " << SDL_GetError() << '\n';
        }
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

void gameWindow::renderTable() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 23; j++) {
            if (m[i][j] == 1)
                gBlack.render(posW(j), posH(i), gRenderer);
            else if (m[i][j] == 2)
                gWhite.render(posW(j), posH(i), gRenderer);
            else if (m[i][j] == 3) {
                gBlack.setAlpha(128);
                gBlack.render(posW(j), posH(i), gRenderer);
                gBlack.setAlpha(255);
            }
            else if (m[i][j] == 4) {
                gWhite.setAlpha(128);
                gWhite.render(posW(j), posH(i), gRenderer);
                gWhite.setAlpha(255);
            }
        }
    }
}

void gameWindow::renderButtons() {
    for (int i = 0; i < TOTAL_BUTTONS; i++) {
        if (gButton[i].getState() == 1)
            gButton[i].render(*this);
    }
}

void gameWindow::renderStats() {
    p1Score.render(25, 40, gRenderer);
    p2Score.render(25, 80, gRenderer);
    gTextTexture.render(25, 140, gRenderer);
}

void gameWindow::renderUI() {
    renderButtons();
    renderStats();
}

void gameWindow::handleEvent(SDL_Event *e) {
    // The player should choose a swap type to open the game before placing stones
    if (swapBlock)
        return;
    
    if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        // If the round finished, clicking on the table starts a new round
        if (won == true) {
            if (e->type == SDL_MOUSEBUTTONUP && x <= 0.8 * SCREEN_WIDTH)
                setGame();
            return;
        }

        // Correlates the mouse coordinates with the matrix coordinates
        int indW, indH;
        indW = (x - offsetW) / distW;
        indH = (y - offsetH) / distH;

        // Checks the current color of the stone
        int currentColor;
        if (playerTurn == whoBlack)
            currentColor = 1;
        else
            currentColor = 2;
        
        // While the game is in the swap opening, the stones are not colored alternatively
        if (inSwap) {
            if (swapCount < 2)
                currentColor = 1;
            else if (swapCount == 2)
                currentColor = 2;
            else if (swapCount == 3 && inSwap2)
                currentColor = 1;
            else
                currentColor = 2;
        }

        // Checks if a stone should be rendered
        bool ok = false;
        if (indH >= 0 && indH <= 19 && indW >= 0 && indW <= 22 && !inDraw)
            ok = true;
        
        // Checks if it can delete a previous hover stone texture
        if (mLasti >= 0 && mLasti <= 19 && mLastj >= 0 && mLastj <= 22)
            if (m[mLasti][mLastj] == 3 || m[mLasti][mLastj] == 4)
                m[mLasti][mLastj] = 0;

        if (ok) {
            if (e->type == SDL_MOUSEBUTTONUP) {
                if (m[indH][indW] != 1 && m[indH][indW] != 2) {
                    m[indH][indW] = currentColor;

                    // Implements specific swap rules
                    if (inSwap) {
                        swapCount++;
                        
                        // When the first player puts the third stone, the second player must choose different options
                        if (swapCount == 3) {
                            playerTurn = 1 - playerTurn;
                            gTextTexture.loadFromRenderedText("Player 2 turn", {0xFF, 0xFF, 0xFF}, gFont, gRenderer);
                            gButton[5].setState(1);

                            // If the opening is Swap2, the player can also choose a third option
                            if (swapType == 1) {
                                gButton[6].setText("Place 2", gFont, gRenderer);
                                gButton[6].setState(1);
                            }
                        }
                        // If the second player simply placed a fourth stone, he accepted his given color,
                        // the game proceeds as normal from now on
                        else if (swapCount == 4 && !inSwap2) {
                            playerTurn = 1 - playerTurn;
                            gTextTexture.loadFromRenderedText("Player 1 turn", {0, 0, 0}, gFont, gRenderer);
                            exitSwap();
                        }
                        // When the second player places the fifth stone, the first player must choose his color
                        else if (swapCount == 5) {
                            playerTurn = 1 - playerTurn;
                            gTextTexture.loadFromRenderedText("Player 1 turn", {0xFF, 0xFF, 0xFF}, gFont, gRenderer);
                            gButton[5].setState(1);
                        }
                        // If the first player simply placed a sixth stone, he accepted his given color,
                        // the game proceeds as normal from now on
                        else if (swapCount == 6) {
                            whoBlack = 1;
                            playerTurn = 1 - playerTurn;
                            gTextTexture.loadFromRenderedText("Player 2 turn", {0, 0, 0}, gFont, gRenderer);
                            exitSwap();
                        }
                        // During the first three moves, it is the first player's turn
                        else if (!inSwap2)
                            gTextTexture.loadFromRenderedText("Player 1 turn", {0, 0, 0}, gFont, gRenderer);
                    }
                    else {
                        // Checks if the game is the game ended on a win, draw, or if it didn't end
                        int state = checkGameState(indH, indW);

                        // The game didn't end, it's the other player's turn
                        if (!state) {
                            playerTurn = 1 - playerTurn;
                            gTextTexture.loadFromRenderedText("Player " + std::to_string(playerTurn + 1) + " turn", {(2 - currentColor) * 0xFF, (2 - currentColor) * 0xFF, (2 - currentColor) * 0xFF}, gFont, gRenderer);
                        }
                        // The game was won with the last move, the current player wins
                        else if (state == 1) {
                            if (playerTurn == 0) {
                                player1Score++;
                                p1Score.loadFromRenderedText("Player 1: " + std::to_string(player1Score), {0, 0, 0}, gFont, gRenderer);
                            }
                            else {
                                player2Score++;
                                p2Score.loadFromRenderedText("Player 2: " + std::to_string(player2Score), {0, 0, 0}, gFont, gRenderer);
                            }
                            gTextTexture.loadFromRenderedText("Player " + std::to_string(playerTurn + 1) + " wins!", {(currentColor - 1) * 0xFF, (currentColor - 1) * 0xFF, (currentColor - 1) * 0xFF}, gFont, gRenderer);
                            setWin();
                        }
                        // The table is full and no one won, the game ends in a draw
                        else if (state == 2) {
                            gTextTexture.loadFromRenderedText("Draw!", {0, 0, 0}, gFont, gRenderer);
                            inDraw = true;
                            setWin();
                        }
                    }
                }
            }
            // If the player didn't release the click button, the game displays a
            // transparent version of the stone on where it will be placed.
            else if (m[indH][indW] == 0 && won == false)
                m[indH][indW] = currentColor + 2;   
        }
        
        // The current coordinates become the previous as the function ends.
        // During the next call, the game will use the previous coordinates to delete the transparent stones.
        mLasti = indH;
        mLastj = indW;
    }
}

void gameWindow::handleButtonsEvent(SDL_Event *e) {
    for (int i = 0; i < TOTAL_BUTTONS; i++) {
        // The special case is when the "Draw" button overlaps with the "No" button
        // which appears after "Draw" is clicked on.
        if (gButton[i].getState() == 1 && !(i == 4 && dontPress == 1)) {
            gButton[i].handleEvent(e, *this);
        }
    }
    // Same as above
    if (dontPress == 1)
        dontPress--;
}

int gameWindow::checkGameState(int I, int J) {
    int cColor = m[I][J];
    int dirI[8] = {-1, -1, 0, 1, 1, 1, 0, -1}, dirJ[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    
    // Distances on all 4 directions: 0: N to S, 1: N-E to S-W, 2: E to W, 3: S-E to N-W
    int dist[4] = {1, 1, 1, 1};
    int newi, newj;

    // Checks all directions for 5-in-a-row lines
    for (int i = 0; i < 8; i++) {
        newi = I + dirI[i];
        newj = J + dirJ[i];
        while (m[newi][newj] == m[I][J]) {
            dist[i % 4]++;
            newi += dirI[i];
            newj += dirJ[i];
        }
        if (i > 3 && dist[i % 4] == 5)
            return 1;
    }

    // If no one won, check if the table is full, if there exists one slot without a stone, the game is not over
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 23; j++) {
            if (m[i][j] != 1 && m[i][j] != 2)
                return 0;
        }
    }

    // If the table is full, return draw flag
    return 2;
}

void gameWindow::setWin() {
    // Resets the flags to default values
    won = true;
    inDraw = false;
    playerTurn = false;
    whoBlack = 0;

    // Hides buttons and renames some of them.
    gButton[1].setState(0);
    gButton[2].setState(0);
    gButton[3].setState(0);
    gButton[3].setText("Draw", gFont, gRenderer);
    gButton[3].setDimensions(140, 40);
    gButton[4].setState(0);
    gButton[6].setText("Swap2", gFont, gRenderer);
}

void gameWindow::setGame() {
    // Resets the flags to start game values
    won = false;
    inSwap = true;
    swapBlock = true;
    swapCount = 0;

    // Shows the Swap options
    gButton[5].setState(1);
    gButton[6].setState(1);

    gTextTexture.loadFromRenderedText("Choose swap", {0, 0, 0}, gFont, gRenderer);

    clearTable();
}

void gameWindow::exitSwap() {
    // Resets the swap flags
    inSwap = false;
    inSwap2 = false;

    // Shows standard game buttons and hides the swap buttons
    gButton[1].setState(1);
    gButton[2].setState(1);

    gButton[3].setState(1);
    gButton[3].setText("Draw", gFont, gRenderer);
    gButton[3].setDimensions(140, 40);

    gButton[5].setState(0);
    gButton[6].setState(0);
}

void gameWindow::close() {
    gBackgroundTexture.free();
    gSidebarTexture.free();
    gTextTexture.free();
    gBlack.free();
    gWhite.free();
    p1Score.free();
    p2Score.free();

    TTF_CloseFont(gFont);
    gFont = NULL;

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}