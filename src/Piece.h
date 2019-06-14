//
// Created by aroldan
//

#ifndef __Piece_h__
#define __Piece_h__

#include <cstdint>
#include <SDL2/SDL.h>
#include "Defs.h"

class Piece {
public:
    enum Type_t {
        NONE, O, I, L, J, S, Z, T
    };

public:
    Piece();
    Piece(const Piece& src);

    virtual ~Piece() = default;

    void createNext();
    void createHold(const Type_t type);
    void createRandom();
    void create(const Type_t type);
    void rotateRight();
    void rotateLeft();

    void setType(const Type_t& type);

    void undoMove();
    void move();
    void left();
    void right();

    void render(SDL_Renderer* renderer);

    static SDL_Color getColorByType(const Type_t type);

    bool getMask(uint8_t x, uint8_t y) const;
    int32_t getX() const;
    int32_t getY() const;
    Type_t getType() const;
    int32_t getTopMask() const;
    int32_t getBottomMask() const;
    int32_t getLeftMask() const;
    int32_t getRightMask() const;

    static int32_t calculateLeftMask(const Piece& piece);
    static int32_t calculateRightMask(const Piece& piece);
    static int32_t calculateBottomMask(const Piece& piece);
    static int32_t calculateTopMask(const Piece& piece);

    void fixPosition();

private:
    Type_t type;
    bool mask[MASK_SIZE][MASK_SIZE];
    int8_t rotation;
    int32_t x, y;
    int32_t topMask, rightMask, bottomMask, leftMask;

    int32_t lastX, lastY;
    int8_t lastRotation;

    void createMask(const Type_t type, int32_t rotation);
    void setColorByType(const Type_t type);
};


#endif // __Piece_h__
