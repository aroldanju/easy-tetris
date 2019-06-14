//
// Created by aroldan
//

#include <iostream>
#include "Piece.h"

Piece::Piece() :
    type(O),
    rotation(0),
    x(0), y(0),
    topMask(NO_MASK), rightMask(NO_MASK), bottomMask(NO_MASK), leftMask(NO_MASK)
{
    this->mask[0][0] = false; this->mask[0][1] = false; this->mask[0][2] = false; this->mask[0][3] = false;
    this->mask[1][0] = false; this->mask[1][1] = false; this->mask[1][2] = false; this->mask[1][3] = false;
    this->mask[2][0] = false; this->mask[2][1] = false; this->mask[2][2] = false; this->mask[2][3] = false;
    this->mask[3][0] = false; this->mask[3][1] = false; this->mask[3][2] = false; this->mask[3][3] = false;
}

Piece::Piece(const Piece& src) :
    type(src.type),
    rotation(src.rotation),
    x(src.x),
    y(src.y)
{
    createMask(this->type, this->rotation);
}

void Piece::createNext()    {
    const Type_t type = (Type_t)(1 + (rand() % MAX_PIECES));
    create(type);
    this->x = POSITION_NEXT;
}

void Piece::createHold(const Type_t type) {
    create(type);
    this->x = POSITION_HOLD;
}

void Piece::createRandom() {
    const Type_t type = (Type_t)(rand() % 6);
    create(type);

    this->x = (BOX_COLUMNS / 2) - ((rightMask - leftMask) / 2);
    this->y = -this->topMask;
}

void Piece::setType(const Type_t& type)    {
    this->type = type;
    createMask(this->type, this->rotation);
}

void Piece::create(const Type_t type) {
    this->type = type;
    this->rotation = 0;
    this->x = (BOX_COLUMNS / 2) - (MASK_SIZE / 2);
    this->y = 0;

    createMask(this->type, this->rotation);

    this->x = (BOX_COLUMNS / 2) - ((rightMask - leftMask) / 2);
    this->y = -this->topMask;
}

void Piece::rotateRight()  {

    this->lastY = this->y;
    this->lastX = this->x;
    this->lastRotation = this->rotation;

    rotation++;
    if (rotation == 4) {
        rotation = 0;
    }

    createMask(this->type, this->rotation);
}

void Piece::rotateLeft()   {

    this->lastY = this->y;
    this->lastX = this->x;
    this->lastRotation = this->rotation;

    rotation--;
    if (rotation == -1) {
        rotation = 3;
    }

    createMask(this->type, this->rotation);
}

void Piece::createMask(const Type_t type, int32_t rotation) {
    switch (type) {
        case O:
            this->mask[0][0] = true; this->mask[0][1] = true; this->mask[0][2] = false; this->mask[0][3] = false;
            this->mask[1][0] = true; this->mask[1][1] = true; this->mask[1][2] = false; this->mask[1][3] = false;
            this->mask[2][0] = false; this->mask[2][1] = false; this->mask[2][2] = false; this->mask[2][3] = false;
            this->mask[3][0] = false; this->mask[3][1] = false; this->mask[3][2] = false; this->mask[3][3] = false;
            break;

        case I:
            if (rotation == 0 || rotation == 2) {
                this->mask[0][0] = true; this->mask[0][1] = false; this->mask[0][2] = false; this->mask[0][3] = false;
                this->mask[1][0] = true; this->mask[1][1] = false; this->mask[1][2] = false; this->mask[1][3] = false;
                this->mask[2][0] = true; this->mask[2][1] = false; this->mask[2][2] = false; this->mask[2][3] = false;
                this->mask[3][0] = true; this->mask[3][1] = false; this->mask[3][2] = false; this->mask[3][3] = false;
            }
            else if (rotation == 1 || rotation == 3) {
                this->mask[0][0] = true; this->mask[0][1] = true; this->mask[0][2] = true; this->mask[0][3] = true;
                this->mask[1][0] = false; this->mask[1][1] = false; this->mask[1][2] = false; this->mask[1][3] = false;
                this->mask[2][0] = false; this->mask[2][1] = false; this->mask[2][2] = false; this->mask[2][3] = false;
                this->mask[3][0] = false; this->mask[3][1] = false; this->mask[3][2] = false; this->mask[3][3] = false;
            }
            break;

        case S:
            if (rotation == 0 || rotation == 2) {
                this->mask[0][0] = false; this->mask[0][1] = true; this->mask[0][2] = true; this->mask[0][3] = false;
                this->mask[1][0] = true; this->mask[1][1] = true; this->mask[1][2] = false; this->mask[1][3] = false;
                this->mask[2][0] = false; this->mask[2][1] = false; this->mask[2][2] = false; this->mask[2][3] = false;
                this->mask[3][0] = false; this->mask[3][1] = false; this->mask[3][2] = false; this->mask[3][3] = false;
            }
            else if (rotation == 1 || rotation == 3) {
                this->mask[0][0] = true; this->mask[0][1] = false; this->mask[0][2] = false; this->mask[0][3] = false;
                this->mask[1][0] = true; this->mask[1][1] = true; this->mask[1][2] = false; this->mask[1][3] = false;
                this->mask[2][0] = false; this->mask[2][1] = true; this->mask[2][2] = false; this->mask[2][3] = false;
                this->mask[3][0] = false; this->mask[3][1] = false; this->mask[3][2] = false; this->mask[3][3] = false;
            }
            break;

        case Z:
            if (rotation == 0 || rotation == 2) {
                this->mask[0][0] = true; this->mask[0][1] = true; this->mask[0][2] = false; this->mask[0][3] = false;
                this->mask[1][0] = false; this->mask[1][1] = true; this->mask[1][2] = true; this->mask[1][3] = false;
                this->mask[2][0] = false; this->mask[2][1] = false; this->mask[2][2] = false; this->mask[2][3] = false;
                this->mask[3][0] = false; this->mask[3][1] = false; this->mask[3][2] = false; this->mask[3][3] = false;
            }
            else if (rotation == 1 || rotation == 3) {
                this->mask[0][0] = false; this->mask[0][1] = true; this->mask[0][2] = false; this->mask[0][3] = false;
                this->mask[1][0] = true; this->mask[1][1] = true; this->mask[1][2] = false; this->mask[1][3] = false;
                this->mask[2][0] = true; this->mask[2][1] = false; this->mask[2][2] = false; this->mask[2][3] = false;
                this->mask[3][0] = false; this->mask[3][1] = false; this->mask[3][2] = false; this->mask[3][3] = false;
            }
            break;

        case L:
            if (rotation == 0) {
                this->mask[0][0] = true; this->mask[0][1] = false; this->mask[0][2] = false; this->mask[0][3] = false;
                this->mask[1][0] = true; this->mask[1][1] = false; this->mask[1][2] = false; this->mask[1][3] = false;
                this->mask[2][0] = true; this->mask[2][1] = true; this->mask[2][2] = false; this->mask[2][3] = false;
                this->mask[3][0] = false; this->mask[3][1] = false; this->mask[3][2] = false; this->mask[3][3] = false;
            }
            else if (rotation == 1) {
                this->mask[0][0] = true; this->mask[0][1] = true; this->mask[0][2] = true; this->mask[0][3] = false;
                this->mask[1][0] = true; this->mask[1][1] = false; this->mask[1][2] = false; this->mask[1][3] = false;
                this->mask[2][0] = false; this->mask[2][1] = false; this->mask[2][2] = false; this->mask[2][3] = false;
                this->mask[3][0] = false; this->mask[3][1] = false; this->mask[3][2] = false; this->mask[3][3] = false;
            }
            else if (rotation == 2) {
                this->mask[0][0] = true; this->mask[0][1] = true; this->mask[0][2] = false; this->mask[0][3] = false;
                this->mask[1][0] = false; this->mask[1][1] = true; this->mask[1][2] = false; this->mask[1][3] = false;
                this->mask[2][0] = false; this->mask[2][1] = true; this->mask[2][2] = false; this->mask[2][3] = false;
                this->mask[3][0] = false; this->mask[3][1] = false; this->mask[3][2] = false; this->mask[3][3] = false;
            }
            else if (rotation == 3) {
                this->mask[0][0] = false; this->mask[0][1] = false; this->mask[0][2] = true; this->mask[0][3] = false;
                this->mask[1][0] = true; this->mask[1][1] = true; this->mask[1][2] = true; this->mask[1][3] = false;
                this->mask[2][0] = false; this->mask[2][1] = false; this->mask[2][2] = false; this->mask[2][3] = false;
                this->mask[3][0] = false; this->mask[3][1] = false; this->mask[3][2] = false; this->mask[3][3] = false;
            }
            break;

        case J:
            if (rotation == 0) {
                this->mask[0][0] = false; this->mask[0][1] = true; this->mask[0][2] = false; this->mask[0][3] = false;
                this->mask[1][0] = false; this->mask[1][1] = true; this->mask[1][2] = false; this->mask[1][3] = false;
                this->mask[2][0] = true; this->mask[2][1] = true; this->mask[2][2] = false; this->mask[2][3] = false;
                this->mask[3][0] = false; this->mask[3][1] = false; this->mask[3][2] = false; this->mask[3][3] = false;
            }
            else if (rotation == 1) {
                this->mask[0][0] = true; this->mask[0][1] = false; this->mask[0][2] = false; this->mask[0][3] = false;
                this->mask[1][0] = true; this->mask[1][1] = true; this->mask[1][2] = true; this->mask[1][3] = false;
                this->mask[2][0] = false; this->mask[2][1] = false; this->mask[2][2] = false; this->mask[2][3] = false;
                this->mask[3][0] = false; this->mask[3][1] = false; this->mask[3][2] = false; this->mask[3][3] = false;
            }
            else if (rotation == 2) {
                this->mask[0][0] = true; this->mask[0][1] = true; this->mask[0][2] = false; this->mask[0][3] = false;
                this->mask[1][0] = true; this->mask[1][1] = false; this->mask[1][2] = false; this->mask[1][3] = false;
                this->mask[2][0] = true; this->mask[2][1] = false; this->mask[2][2] = false; this->mask[2][3] = false;
                this->mask[3][0] = false; this->mask[3][1] = false; this->mask[3][2] = false; this->mask[3][3] = false;
            }
            else if (rotation == 3) {
                this->mask[0][0] = true; this->mask[0][1] = true; this->mask[0][2] = true; this->mask[0][3] = false;
                this->mask[1][0] = false; this->mask[1][1] = false; this->mask[1][2] = true; this->mask[1][3] = false;
                this->mask[2][0] = false; this->mask[2][1] = false; this->mask[2][2] = false; this->mask[2][3] = false;
                this->mask[3][0] = false; this->mask[3][1] = false; this->mask[3][2] = false; this->mask[3][3] = false;
            }
            break;

        case T:
            if (rotation == 0) {
                this->mask[0][0] = true; this->mask[0][1] = true; this->mask[0][2] = true; this->mask[0][3] = false;
                this->mask[1][0] = false; this->mask[1][1] = true; this->mask[1][2] = false; this->mask[1][3] = false;
                this->mask[2][0] = false; this->mask[2][1] = false; this->mask[2][2] = false; this->mask[2][3] = false;
                this->mask[3][0] = false; this->mask[3][1] = false; this->mask[3][2] = false; this->mask[3][3] = false;
            }
            else if (rotation == 1) {
                this->mask[0][0] = false; this->mask[0][1] = true; this->mask[0][2] = false; this->mask[0][3] = false;
                this->mask[1][0] = true; this->mask[1][1] = true; this->mask[1][2] = false; this->mask[1][3] = false;
                this->mask[2][0] = false; this->mask[2][1] = true; this->mask[2][2] = false; this->mask[2][3] = false;
                this->mask[3][0] = false; this->mask[3][1] = false; this->mask[3][2] = false; this->mask[3][3] = false;
            }
            else if (rotation == 2) {
                this->mask[0][0] = false; this->mask[0][1] = true; this->mask[0][2] = false; this->mask[0][3] = false;
                this->mask[1][0] = true; this->mask[1][1] = true; this->mask[1][2] = true; this->mask[1][3] = false;
                this->mask[2][0] = false; this->mask[2][1] = false; this->mask[2][2] = false; this->mask[2][3] = false;
                this->mask[3][0] = false; this->mask[3][1] = false; this->mask[3][2] = false; this->mask[3][3] = false;
            }
            else if (rotation == 3) {
                this->mask[0][0] = true; this->mask[0][1] = false; this->mask[0][2] = false; this->mask[0][3] = false;
                this->mask[1][0] = true; this->mask[1][1] = true; this->mask[1][2] = false; this->mask[1][3] = false;
                this->mask[2][0] = true; this->mask[2][1] = false; this->mask[2][2] = false; this->mask[2][3] = false;
                this->mask[3][0] = false; this->mask[3][1] = false; this->mask[3][2] = false; this->mask[3][3] = false;
            }
            break;

        case NONE:
            this->mask[0][0] = false; this->mask[0][1] = false; this->mask[0][2] = false; this->mask[0][3] = false;
            this->mask[1][0] = false; this->mask[1][1] = false; this->mask[1][2] = false; this->mask[1][3] = false;
            this->mask[2][0] = false; this->mask[2][1] = false; this->mask[2][2] = false; this->mask[2][3] = false;
            this->mask[3][0] = false; this->mask[3][1] = false; this->mask[3][2] = false; this->mask[3][3] = false;
            break;
    }

    // Calculate mask bounds
    this->leftMask = calculateLeftMask(*this);
    this->rightMask = calculateRightMask(*this);
    this->topMask = calculateTopMask(*this);
    this->bottomMask = calculateBottomMask(*this);

    // Fix position
    fixPosition();
}

int32_t Piece::getTopMask() const  {
    return this->topMask;
}

int32_t Piece::getBottomMask() const   {
    return this->bottomMask;
}

int32_t Piece::getLeftMask() const {
    return this->leftMask;
}

int32_t Piece::getRightMask() const    {
    return this->rightMask;
}

int32_t Piece::calculateLeftMask(const Piece& piece) {
    for (uint8_t col = 0; col < MASK_SIZE; col++) {
        for (uint8_t row = 0; row < MASK_SIZE; row++) {
            if (piece.getMask(col, row)) {
                return col;
            }
        }
    }

    return NO_MASK;
}

int32_t Piece::calculateRightMask(const Piece &piece){
    for (uint8_t col = MASK_SIZE - 1; col >= 0; col--) {
        for (uint8_t row = 0; row < MASK_SIZE; row++) {
            if (piece.getMask(col, row)) {
                return col;
            }
        }
    }

    return NO_MASK;
}

int32_t Piece::calculateBottomMask(const Piece &piece){
    for (uint8_t row = MASK_SIZE - 1; row >= 0; row--) {
        for (uint8_t col = 0; col < MASK_SIZE; col++) {
            if (piece.getMask(col, row)) {
                return row;
            }
        }
    }

    return NO_MASK;
}

int32_t Piece::calculateTopMask(const Piece& piece) {
    for (uint8_t row = 0; row < MASK_SIZE; row++) {
        for (uint8_t col = 0; col < MASK_SIZE; col++) {
            if (piece.getMask(col, row)) {
                return row;
            }
        }
    }

    return NO_MASK;
}

void Piece::move() {
    this->lastY = this->y;
    this->lastX = this->x;
    this->lastRotation = this->rotation;

    this->y++;
}

void Piece::left() {

    this->lastY = this->y;
    this->lastX = this->x;
    this->lastRotation = this->rotation;

    this->x--;

    fixPosition();
}

void Piece::undoMove() {
    this->x = this->lastX;
    this->y = this->lastY;

    if (this->rotation != this->lastRotation) {
        this->rotation = this->lastRotation;
        createMask(this->type, this->rotation);
    }
}

void Piece::fixPosition() {
    if (this->x > BOX_COLUMNS - 1 - this->rightMask) {
        this->x = BOX_COLUMNS - 1 - this->rightMask;
    }
    else if (this->x < -this->leftMask) {
        this->x = -this->leftMask;
    }
}

void Piece::right() {

    this->lastY = this->y;
    this->lastX = this->x;
    this->lastRotation = this->rotation;

    this->x++;

    fixPosition();
}

void Piece::render(SDL_Renderer* renderer) {

    SDL_Color color = getColorByType(this->type);
    SDL_SetRenderDrawColor(renderer, color.r,  color.g,  color.b, 255);

    for (uint8_t i = 0; i < MASK_SIZE; i++) {
        for (uint8_t j = 0; j < MASK_SIZE; j++) {
            if (this->mask[i][j]) {
                SDL_Rect rect = (SDL_Rect){0, 0, CELL_SIZE, CELL_SIZE};
                if (this->x >= 0 && this->y >= 0 && this->x < BOX_COLUMNS && this->y < BOX_ROWS) {
                    rect.x = BOX_LEFT + this->x * CELL_SIZE + j * CELL_SIZE;
                    rect.y = BOX_TOP + this->y * CELL_SIZE + i * CELL_SIZE;
                }
                else if (this->x == POSITION_NEXT){
                    rect.x = LEFT_NEXT + j * CELL_SIZE;
                    rect.y = TOP_NEXT + i * CELL_SIZE;
                }
                else if (this->x == POSITION_HOLD) {
                    rect.x = LEFT_HOLD + j * CELL_SIZE;
                    rect.y = TOP_HOLD + i * CELL_SIZE;
                }

                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}


SDL_Color Piece::getColorByType(const Type_t type)  {
    SDL_Color color = (SDL_Color){ 255, 255, 255, 255 };

    switch (type) {
        case O: color = (SDL_Color){255, 255, 0, 255}; break;
        case I: color = (SDL_Color){0, 255, 255, 255}; break;
        case L: color = (SDL_Color){255, 128, 0, 255}; break;
        case J: color = (SDL_Color){0, 0, 255, 255}; break;
        case S: color = (SDL_Color){0, 255, 0, 255}; break;
        case Z: color = (SDL_Color){255, 0, 0, 255}; break;
        case T: color = (SDL_Color){128, 0, 128, 255}; break;
    }

    return color;
}

bool Piece::getMask(uint8_t x, uint8_t y) const    {
    return this->mask[y][x];
}

int32_t Piece::getX() const {
    return this->x;
}

int32_t Piece::getY() const {
    return this->y;
}

Piece::Type_t Piece::getType() const {
    return this->type;
}