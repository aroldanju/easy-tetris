//
// Created by aroldan
//

#include <iostream>
#include <sstream>
#include "Game.h"

Game::Game() :
    movePieceDelay(DEFAULT_MOVE_DELAY),
    level(1),
    score(0),
    lines(0),
    font(nullptr),
    textureScore(nullptr), textureLines(nullptr), textureLevel(nullptr),
    surfaceScore(nullptr), surfaceLines(nullptr), surfaceLevel(nullptr),
    updateScore(true), updateLines(true), updateLevel(true),
    surfaceNext(nullptr),textureNext(nullptr),surfaceHold(nullptr),textureHold(nullptr),
    surfaceGameOver(nullptr),textureGameOver(nullptr),
    surfacePause(nullptr),texturePause(nullptr),
    gameOver(false), paused(false)
{
    reset();
}

void Game::clearCounter() {
    this->counterTotal = 0;
    for (int i = 0; i < MAX_PIECES; i++) {
        this->counter[i] = 0;
    }
}

void Game::reset() {
    clearCounter();

    clearBox();

    this->currentPiece.createRandom();
    this->nextPiece.createNext();
    this->holdPiece.createHold(Piece::NONE);

    this->lines = 0;
    this->score = 0;
    this->level = 1;

    this->updateScore = this->updateLevel = this->updateLines = true;

    this->gameOver = false;
    this->paused = false;
}

Game::~Game() {
    if (textureScore) {
        SDL_DestroyTexture(textureScore);
    }

    if (surfaceScore) {
        SDL_FreeSurface(surfaceScore);
    }

    if (textureLines) {
        SDL_DestroyTexture(textureLines);
    }

    if (surfaceLines) {
        SDL_FreeSurface(surfaceLines);
    }

    if (textureLevel) {
        SDL_DestroyTexture(textureLevel);
    }

    if (surfaceLevel) {
        SDL_FreeSurface(surfaceLevel);
    }

    SDL_DestroyTexture(textureNext);
    SDL_DestroyTexture(textureHold);
    SDL_FreeSurface(surfaceNext);
    SDL_FreeSurface(surfaceHold);

    if (textureGameOver) {
        SDL_DestroyTexture(this->textureGameOver);
        SDL_FreeSurface(this->surfaceGameOver);
    }

    if (texturePause) {
        SDL_DestroyTexture(this->texturePause);
        SDL_FreeSurface(this->surfacePause);
    }
}

void Game::setBox(uint32_t column, uint32_t row, Piece::Type_t color)   {
    this->box[row][column] = color;
}

Piece::Type_t Game::getBox(uint32_t column, uint32_t row) const {
    return this->box[row][column];
}

void Game::skip() {
    if (gameOver) {
        reset();
    }
}

void Game::update() {
    if (this->gameOver || this->paused) return;

    if (SDL_GetTicks() - this->tick >= movePieceDelay) {
        this->tick = SDL_GetTicks();
        move();
    }
}

void Game::left() {
    if (this->gameOver || this->paused) return;

    this->currentPiece.left();

    if (checkPieceCollision(this->currentPiece)) {
        this->currentPiece.undoMove();
    }
}

void Game::renderGameOver(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_Rect rect = (SDL_Rect){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &rect);

    if (!this->textureGameOver) {
        this->surfaceGameOver = createTextSurface("Game Over");
        this->textureGameOver = SDL_CreateTextureFromSurface(renderer, this->surfaceGameOver);
    }

    rect = (SDL_Rect){ SCREEN_WIDTH / 2 - this->surfaceGameOver->w / 2, SCREEN_HEIGHT / 2 - this->surfaceGameOver->h / 2, this->surfaceGameOver->w, this->surfaceGameOver->h };
    SDL_RenderCopy(renderer, this->textureGameOver, nullptr, &rect);
}

void Game::renderPause(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_Rect rect = (SDL_Rect){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &rect);

    if (!this->texturePause) {
        this->surfacePause = createTextSurface("Pause");
        this->texturePause = SDL_CreateTextureFromSurface(renderer, this->surfacePause);
    }

    rect = (SDL_Rect){ SCREEN_WIDTH / 2 - this->surfacePause->w / 2, SCREEN_HEIGHT / 2 - this->surfacePause->h / 2, this->surfacePause->w, this->surfacePause->h };
    SDL_RenderCopy(renderer, this->texturePause, nullptr, &rect);
}

void Game::place() {
    if (this->gameOver || this->paused) return;

    const uint32_t currentScore = this->score;
    while (currentScore == this->score) {
        move();
    }
}

bool Game::checkPieceCollision(const Piece &piece) {
    for (int i = 0; i < MASK_SIZE; i++) {
        for (int j = 0; j < MASK_SIZE; j++) {
            if (piece.getMask(j, i)) {

                const int x = piece.getX() + j;
                const int y = piece.getY() + i;

                if (this->box[y][x] != Piece::NONE) {
                    return true;
                }
            }
        }
    }

    return false;
}

void Game::right() {
    if (this->gameOver || this->paused) return;

    this->currentPiece.right();

    if (checkPieceCollision(this->currentPiece)) {
        this->currentPiece.undoMove();
    }
}

void Game::move() {
    if (this->gameOver || this->paused) return;

    if (checkPieceCollisionWithBox(this->currentPiece)) {
        attachPiece(this->currentPiece);

        addScore(SCORE_PER_PIECE);

        std::queue<uint32_t> lines = checkLines();
        if (!lines.empty()) {

            this->lines += lines.size();
            this->updateLines = true;
            addScore(SCORE_PER_LINE * lines.size());

            while (!lines.empty()) {
                uint32_t row = lines.front();
                for (uint32_t i = 0; i < BOX_COLUMNS; i++) {
                    setBox(i, row, Piece::NONE);
                }
                lines.pop();
            }

            moveBox();
        }

        newPiece();
    }

    this->currentPiece.move();
}

void Game::moveBox() {
    for (int32_t o = 0; o < BOX_ROWS; o++) {    //??
        for (int32_t i = BOX_ROWS - 1; i >= 0; i--) {
            bool empty = true;

            for (uint32_t j = 0; j < BOX_COLUMNS; j++) {
                if (getBox(j, i) != Piece::NONE) {
                    empty = false;
                    break;
                }
            }

            if (empty && i > 0) {
                for (int32_t j = i; j >= 1; j--) {
                    for (uint32_t k = 0; k < BOX_COLUMNS; k++) {
                        setBox(k, j, getBox(k, j - 1));
                    }
                }
            }
        }
    }
}

void Game::renderStaticLabels(SDL_Renderer *renderer) {
    if (!this->textureNext) {
        this->surfaceNext = createTextSurface("Next:");
        this->textureNext = SDL_CreateTextureFromSurface(renderer, this->surfaceNext);
    }

    if (!this->textureHold) {
        this->surfaceHold = createTextSurface("Hold:");
        this->textureHold = SDL_CreateTextureFromSurface(renderer, this->surfaceHold);
    }

    SDL_Rect rect;

    rect = (SDL_Rect){LABEL_NEXT_LEFT, LABEL_NEXT_TOP, this->surfaceNext->w, this->surfaceNext->h};
    SDL_RenderCopy(renderer, this->textureNext, nullptr, &rect);

    rect = (SDL_Rect){LABEL_HOLD_LEFT, LABEL_HOLD_TOP, this->surfaceHold->w, this->surfaceHold->h};
    SDL_RenderCopy(renderer, this->textureHold, nullptr, &rect);
}

void Game::addScore(uint32_t score)   {
    this->updateScore = true;

    this->score += score;

#ifdef __DEBUG__
    std::cout << "Game::addScore(" << score << "): score = " << this->score << std::endl;
#endif  // __DEBUG__

    const uint32_t scoreToNextLevel = getScoreToLevel(this->level + 1);
    if (this->score >= scoreToNextLevel) {
        nextLevel();
    }
}

void Game::nextLevel() {
    this->updateLevel = true;

    this->level++;

    if (this->movePieceDelay > MINIMUM_MOVE_DELAY) {
        this->movePieceDelay -= REDUCTION_MOVE_DELAY_PER_LEVEL;
        if (this->movePieceDelay < MINIMUM_MOVE_DELAY) {
            this->movePieceDelay = MINIMUM_MOVE_DELAY;
        }
    }

#ifdef __DEBUG__
    std::cout << "Game::nextLevel: level = " << this->level << " movePieceDelay = " << this->movePieceDelay << std::endl;
#endif  // __DEBUG__

}

uint32_t Game::getScoreToLevel(int level) {
    return ((level * level) * 100);
}

bool Game::checkPieceCollisionWithBox(const Piece& piece) {

    Piece temp(piece);

    // With ground
    if (temp.getY() >= BOX_ROWS - MASK_SIZE) {
        for (int i = MASK_SIZE - 1; i >= 0; i--) {
            for (int j = 0; j < MASK_SIZE; j++) {
                if (temp.getMask(j, i) && temp.getY() + i >= BOX_ROWS - 1) {
                    return true;
                }
            }
        }
    }

    // Simulate movement
    temp.move();

    // With boxes
    for (int i = 0; i < MASK_SIZE; i++) {
        for (int j = 0; j < MASK_SIZE; j++) {
            if (temp.getMask(j, i)) {
                const int x = temp.getX() + j;
                const int y = temp.getY() + i;
                if (this->box[y][x] != Piece::NONE) {
                    return true;
                }
            }
        }
    }

    return false;
}

void Game::render(SDL_Renderer *renderer) {
    renderBox(renderer);
    renderCurrentPiece(renderer);
    renderNextPiece(renderer);
    renderHoldPiece(renderer);
    renderScore(renderer);
    renderLines(renderer);
    renderLevel(renderer);
    renderStaticLabels(renderer);

    if (this->gameOver) {
        renderGameOver(renderer);
    }
    else {
        if (this->paused) {
            renderPause(renderer);
        }
    }
}

void Game::rotateLeft() {
    if (this->gameOver || this->paused) return;

    this->currentPiece.rotateLeft();

    if (checkPieceCollision(this->currentPiece)) {
        this->currentPiece.undoMove();
    }
}

void Game::rotateRight() {
    if (this->gameOver || this->paused) return;

    this->currentPiece.rotateRight();

    if (checkPieceCollision(this->currentPiece)) {
        this->currentPiece.undoMove();
    }
}

void Game::pause() {
    this->paused = !this->paused;
    if (this->paused) {
        this->ticksPause = SDL_GetTicks();
    }
    else {
        this->tick += (SDL_GetTicks() - this->ticksPause);
    }
}

uint32_t Game::getScore() const {
    return this->score;
}

void Game::setFont(TTF_Font *font) {
    this->font = font;
}

void Game::renderBox(SDL_Renderer* renderer)  {

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 64);
    for (int i = 0; i< BOX_COLUMNS+1; i++) {
        SDL_RenderDrawLine(renderer, BOX_LEFT + i * CELL_SIZE, BOX_TOP, BOX_LEFT + i * CELL_SIZE, BOX_TOP + CELL_SIZE * BOX_ROWS);
    }
    for (int i = 0; i< BOX_ROWS+1; i++) {
        SDL_RenderDrawLine(renderer, BOX_LEFT, BOX_TOP + i * CELL_SIZE, BOX_LEFT + CELL_SIZE * BOX_COLUMNS, BOX_TOP + i * CELL_SIZE);
    }

    for (int i = 0; i < BOX_ROWS; i++) {
        for (int j = 0; j < BOX_COLUMNS; j++) {
            if (box[i][j] != Piece::NONE) {
                SDL_Color color = Piece::getColorByType(box[i][j]);
                SDL_SetRenderDrawColor(renderer, color.r,  color.g,  color.b, 255);
                SDL_Rect rect = (SDL_Rect){ BOX_LEFT + j * CELL_SIZE, BOX_TOP + i * CELL_SIZE, CELL_SIZE, CELL_SIZE };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

}

void Game::renderCurrentPiece(SDL_Renderer *renderer) {
    this->currentPiece.render(renderer);
}

void Game::renderNextPiece(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 64);
    for (int i = 0; i< MASK_SIZE + 1; i++) {
        SDL_RenderDrawLine(renderer, LEFT_NEXT + i * CELL_SIZE, TOP_NEXT, LEFT_NEXT + i * CELL_SIZE, TOP_NEXT + CELL_SIZE * MASK_SIZE);
    }
    for (int i = 0; i< MASK_SIZE + 1; i++) {
        SDL_RenderDrawLine(renderer, LEFT_NEXT, TOP_NEXT + i * CELL_SIZE, LEFT_NEXT + CELL_SIZE * MASK_SIZE, TOP_NEXT + i * CELL_SIZE);
    }

    this->nextPiece.render(renderer);
}

void Game::renderHoldPiece(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 64);
    for (int i = 0; i< MASK_SIZE + 1; i++) {
        SDL_RenderDrawLine(renderer, LEFT_HOLD + i * CELL_SIZE, TOP_HOLD, LEFT_HOLD + i * CELL_SIZE, TOP_HOLD + CELL_SIZE * MASK_SIZE);
    }
    for (int i = 0; i< MASK_SIZE + 1; i++) {
        SDL_RenderDrawLine(renderer, LEFT_HOLD, TOP_HOLD + i * CELL_SIZE, LEFT_NEXT + CELL_SIZE * MASK_SIZE, TOP_HOLD + i * CELL_SIZE);
    }

    this->holdPiece.render(renderer);
}

void Game::clearBox() {
    for (int i = 0; i < BOX_ROWS; i++) {
        for (int j = 0; j < BOX_COLUMNS; j++) {
            box[i][j] = Piece::NONE;
        }
    }
}

void Game::hold() {
    if (this->gameOver || this->paused) return;

    if (this->holdPiece.getType() == Piece::NONE) {
        this->holdPiece.createHold(this->currentPiece.getType());
        newPiece();
    }
    else {
        const Piece::Type_t type = this->holdPiece.getType();
        this->holdPiece.createHold(this->currentPiece.getType());
        this->currentPiece.setType(type);
    }
}

void Game::attachPiece(const Piece& piece)    {
    for (int i = 0; i < MASK_SIZE; i++) {
        for (int j = 0; j < MASK_SIZE; j++) {
            if (piece.getMask(j, i)) {
                //this->box[piece.getY() + i][piece.getX() + j] = piece.getType();
                setBox(piece.getX() + j, piece.getY() + i, piece.getType());
            }
        }
    }
}

void Game::newPiece() {
    this->currentPiece.create(this->nextPiece.getType());
    this->nextPiece.createNext();

    if (checkPieceCollisionWithBox(this->currentPiece)) {
        this->gameOver = true;

#ifdef __DEBUG__
        for (int i = 0; i < MAX_PIECES; i++) {
            std::cout << "Piece #" << i << ": " << this->counter[i] << " times (" << (((float)this->counter[i]/(float)this->counterTotal)*100.0f) << "%)." << std::endl;
        }
#endif

    }

    this->counter[this->currentPiece.getType() - 1]++;
    this->counterTotal++;
}

void Game::setMovePieceDelay(uint32_t delay)  {
    this->movePieceDelay = delay;
}

std::queue<uint32_t> Game::checkLines() {
    std::queue<uint32_t> lines;

    for (int32_t i = BOX_ROWS - 1; i >= 0; i--) {
        bool line = true;
        for (uint32_t j = 0; j < BOX_COLUMNS; j++) {
            if (getBox(j, i) == Piece::NONE) {
                line = false;
                continue;
            }
        }

        if (line) {
            lines.push(i);
        }
    }

    return lines;
}

void Game::renderScore(SDL_Renderer* renderer)    {
    if (this->updateScore) {
        if (textureScore) {
            SDL_DestroyTexture(textureScore);
        }

        if (this->surfaceScore) {
            SDL_FreeSurface(this->surfaceScore);
        }

        std::stringstream textS;
        textS << "Score: " << this->score;
        this->surfaceScore = createTextSurface(textS.str());
        this->textureScore = SDL_CreateTextureFromSurface(renderer, this->surfaceScore);

        this->updateScore = false;
    }

    SDL_Rect rect = (SDL_Rect){ LABEL_SCORE_LEFT, LABEL_SCORE_TOP, this->surfaceScore->w, this->surfaceScore->h};
    SDL_RenderCopy(renderer, this->textureScore, nullptr, &rect);
}

void Game::renderLevel(SDL_Renderer* renderer)    {

    if (this->updateLevel) {
        if (textureLevel) {
            SDL_DestroyTexture(textureLevel);
        }

        if (this->surfaceLevel) {
            SDL_FreeSurface(this->surfaceLevel);
        }

        std::stringstream textLevel;
        textLevel << "Level: " << this->level;
        this->surfaceLevel = createTextSurface(textLevel.str());
        this->textureLevel = SDL_CreateTextureFromSurface(renderer, this->surfaceLevel);

        this->updateLevel = false;
    }

    SDL_Rect rect = (SDL_Rect){ LABEL_LEVEL_LEFT, LABEL_LEVEL_TOP, this->surfaceLevel->w, this->surfaceLevel->h };
    SDL_RenderCopy(renderer, this->textureLevel, nullptr, &rect);
}

void Game::renderLines(SDL_Renderer* renderer)    {
    if (this->updateLines) {
        if (this->textureLines) {
            SDL_DestroyTexture(this->textureLines);
        }

        if (this->surfaceLines) {
            SDL_FreeSurface(this->surfaceLines);
        }

        std::stringstream textS;
        textS << "Lines: " << this->lines;
        this->surfaceLines = createTextSurface(textS.str());
        this->textureLines = SDL_CreateTextureFromSurface(renderer, this->surfaceLines);

        this->updateLines = false;
    }

    SDL_Rect rect = (SDL_Rect){ LABEL_LINES_LEFT, LABEL_LINES_TOP, this->surfaceLines->w, this->surfaceLines->h };
    SDL_RenderCopy(renderer, this->textureLines, nullptr, &rect);
}

SDL_Surface* Game::createTextSurface(const std::string& text) {
    return TTF_RenderText_Blended(this->font, text.c_str(), (SDL_Color){255, 255, 255});
}
