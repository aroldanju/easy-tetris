//
// Created by aroldan
//

#ifndef __Game_h__
#define __Game_h__

#include <cstdint>
#include <string>
#include <SDL2/SDL_render.h>
#include <queue>
#include <SDL2/SDL_ttf.h>

#include "Defs.h"
#include "Piece.h"

class Game {
public:
    Game();

    virtual ~Game();

    void setBox(uint32_t column, uint32_t row, Piece::Type_t type);
    Piece::Type_t getBox(uint32_t column, uint32_t row) const;

    void render(SDL_Renderer* renderer);
    void update();

    void rotateLeft();
    void rotateRight();
    void clearBox();
    void newPiece();
    void attachPiece(const Piece& piece);
    bool checkPieceCollisionWithBox(const Piece& piece);
    bool checkPieceCollision(const Piece& piece);
    void move();

    std::queue<uint32_t> checkLines();
    void moveBox();

    void addScore(uint32_t score);

    uint32_t getScoreToLevel(int level);

    void nextLevel();

    void left();
    void right();

    void hold();

    void setMovePieceDelay(uint32_t delay);

    uint32_t getScore() const;

    void setFont(TTF_Font* font);

    void skip();

    void reset();

    void pause();

    void place();

    void clearCounter();

private:
    Piece::Type_t   box[BOX_ROWS][BOX_COLUMNS];
    Piece           currentPiece;
    Piece           nextPiece;
    Piece           holdPiece;
    uint32_t        tick;
    uint32_t        movePieceDelay;
    int             level;
    uint32_t        score;
    uint32_t        lines;

    TTF_Font        *font;
    SDL_Texture     *textureScore;
    SDL_Surface     *surfaceScore;
    bool            updateScore;

    SDL_Texture     *textureLevel;
    SDL_Surface     *surfaceLevel;
    bool            updateLevel;

    SDL_Texture     *textureLines;
    SDL_Surface     *surfaceLines;
    bool            updateLines;

    SDL_Texture     *textureNext, *textureHold, *textureGameOver, *texturePause;
    SDL_Surface     *surfaceNext, *surfaceHold, *surfaceGameOver, *surfacePause;

    bool gameOver;
    bool paused;
    uint32_t ticksPause;

    uint32_t counter[MAX_PIECES];
    uint32_t counterTotal;

protected:
    void renderBox(SDL_Renderer* renderer);
    void renderCurrentPiece(SDL_Renderer* renderer);
    void renderNextPiece(SDL_Renderer* renderer);
    void renderHoldPiece(SDL_Renderer* renderer);
    void renderStaticLabels(SDL_Renderer *renderer);
    void renderScore(SDL_Renderer* renderer);
    void renderLevel(SDL_Renderer* renderer);
    void renderLines(SDL_Renderer* renderer);
    void renderGameOver(SDL_Renderer* renderer);
    void renderPause(SDL_Renderer* renderer);

    SDL_Surface* createTextSurface(const std::string& text);

};


#endif // __Game_h__
