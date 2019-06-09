// Simion 
// Cristian
// Grupa 135

#include "gameWindow.hpp"

#include <iostream>

Button::Button(int t, int w, int h) {
    fillRect = {0, 0, w, h};
    alpha = 128;
    type = t;
}

void Button::setPosition(int x, int y) {
    fillRect.x = x;
    fillRect.y = y;
}

void Button::setDimensions(int w, int h) {
    fillRect.w = w;
    fillRect.h = h;
}

void Button::setType(int t) {
    type = t;
}

void Button::setText(std::string text, TTF_Font *gFont, SDL_Renderer *rend) {
    SDL_Color textColor = {0, 0, 0};
    if (!bTexture.loadFromRenderedText(text, textColor, gFont, rend)) {
        std::cout << "Failed to render text texture!\n";
    }
}

void Button::setState(int i) {
    active = i;
}

void Button::handleEvent(SDL_Event *e, gameWindow &G) {
    if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        bool inside = true;

        if (x < fillRect.x + 800 || x > fillRect.x + fillRect.w + 800 || y < fillRect.y || y > fillRect.y + fillRect.h)
            inside = false;
        
        // If the mouse is not over the button, it gets transparent
        if (!inside) {
            alpha = 128;
        }
        else {
            switch(e->type) {
                // If the mouse moves over the button, it gets solid
                case SDL_MOUSEMOTION:
                    alpha = 255;
                    break;
                
                case SDL_MOUSEBUTTONUP:
                    // Exit game button
                    if (type == 1)
                        G.quit = true;
                    // Forfeit game button
                    else if (type == 2 && getState()) {
                        // Check which player surrendered
                        if (G.playerTurn == 1) {
                            G.player1Score++;
                            G.p1Score.loadFromRenderedText("Player 1: " + std::to_string(G.player1Score), {0, 0, 0}, G.gFont, G.gRenderer);
                        }
                        else {
                            G.player2Score++;
                            G.p2Score.loadFromRenderedText("Player 2: " + std::to_string(G.player2Score), {0, 0, 0}, G.gFont, G.gRenderer);
                        }
                        G.gTextTexture.loadFromRenderedText("Player " + std::to_string(2 - G.playerTurn) + " wins!", {(G.playerTurn == G.whoBlack) * 0xFF, (G.playerTurn == G.whoBlack) * 0xFF, (G.playerTurn == G.whoBlack) * 0xFF}, G.gFont, G.gRenderer);
                        G.setWin();
                    }
                    // Skip turn game button
                    else if (type == 3 && getState()) {
                        G.playerTurn = 1 - G.playerTurn;
                        G.gTextTexture.loadFromRenderedText("Player " + std::to_string(G.playerTurn + 1) + " turn", {(1 - (G.playerTurn == G.whoBlack)) * 0xFF, (1 - (G.playerTurn == G.whoBlack)) * 0xFF, (1 - (G.playerTurn == G.whoBlack)) * 0xFF}, G.gFont, G.gRenderer);
                    }
                    // Draw game button, after clicking on it it transforms into a "Yes" button for confirming the draw
                    else if (type == 0 && getState()) {
                        if (!G.inDraw) {
                            G.inDraw = true;

                            G.gTextTexture.loadFromRenderedText("Accept draw?", {0, 0, 0}, G.gFont, G.gRenderer);

                            G.gButton[3].setText("Yes", G.gFont, G.gRenderer);
                            G.gButton[3].setDimensions(65, 40);
                            
                            G.gButton[4].setState(1);
                            G.gButton[4].setPosition(105, 350);
                            G.dontPress = 1;

                            G.gButton[1].setState(0);
                            G.gButton[2].setState(0);
                        }
                        // If clicked again (this time it is transformed into a "Yes" button), the game ends in a draw
                        else {
                            G.gTextTexture.loadFromRenderedText("Draw!", {0, 0, 0}, G.gFont, G.gRenderer);
                            G.setWin();
                        }
                    }
                    // "No" button for refusing a draw proposal
                    else if (type == -1 && getState()) {
                        G.inDraw = false;

                        G.gTextTexture.loadFromRenderedText("Draw rejected!", {0, 0, 0}, G.gFont, G.gRenderer);
                        G.gButton[1].setState(1);
                        G.gButton[2].setState(1);

                        G.gButton[3].setState(1);
                        G.gButton[3].setText("Draw", G.gFont, G.gRenderer);
                        G.gButton[3].setDimensions(140, 40);

                        G.gButton[4].setState(0);
                    }
                    // "Swap" button, it has 2 functions:
                    // 1: Chooses the Swap option (over Swap2)
                    // 2: Makes the current player switch colors
                    else if (type == 4 && getState()) {
                        // 1st functionality
                        if (G.swapCount != 3 && G.swapCount != 5) {
                            G.swapType = 0;
                            G.swapBlock = false;
                            G.gTextTexture.loadFromRenderedText("In Swap", {0, 0, 0}, G.gFont, G.gRenderer);
                            G.gButton[5].setState(0);
                            G.gButton[6].setState(0);
                        }
                        // 2nd functionality
                        else {
                            // If the game is in Swap2 and the second player chose the third option,
                            // then the first player chose to swap colors
                            if (!G.inSwap2) {
                                G.whoBlack = 1;
                                G.playerTurn = 1 - G.playerTurn;
                                G.gTextTexture.loadFromRenderedText("Player 1 turn", {0xFF, 0xFF, 0xFF}, G.gFont, G.gRenderer);
                            }
                            // If the game is in Swap and the second player chose to swap colors
                            else {
                                G.whoBlack = 0;
                                G.playerTurn = 1 - G.playerTurn;
                                G.gTextTexture.loadFromRenderedText("Player 2 turn", {0xFF, 0xFF, 0xFF}, G.gFont, G.gRenderer);
                            }
                            G.exitSwap();
                        }
                    }
                    // "Swap2" button, it has 2 functions:
                    // 1: Chooses the Swap2 option (over Swap)
                    // 2: Chooses the third option if the Swap2 rule, which makes the second player
                    // place 2 alternating stones and lets the first player choose the color
                    else if (type == 5 && getState()) {
                        // 1st functionality
                        if (G.swapCount != 3) {
                            G.swapType = 1;
                            G.swapBlock = false;
                            G.gTextTexture.loadFromRenderedText("In Swap2", {0, 0, 0}, G.gFont, G.gRenderer);
                            G.gButton[5].setState(0);
                            G.gButton[6].setState(0);
                        }
                        // 2nd functionality
                        else {
                            G.inSwap2 = true;
                            G.gTextTexture.loadFromRenderedText("Player 2 turn", {0x00, 0x00, 0x00}, G.gFont, G.gRenderer);
                            G.gButton[5].setState(0);
                            G.gButton[6].setState(0);
                        }
                    }
                    break;
            }
        }
    }
}

void Button::render(gameWindow &G) {
    SDL_SetRenderDrawColor(G.gRenderer, 0xFF, 0xFF, 0xFF, alpha);
    SDL_RenderFillRect(G.gRenderer, &fillRect);

    // Offsets for centering the text on the button
    int offsetx = (fillRect.w - bTexture.getWidth()) / 2;
    int offsety = (fillRect.h - bTexture.getHeight()) / 2;

    bTexture.render(fillRect.x + offsetx, fillRect.y + offsety, G.gRenderer);
}